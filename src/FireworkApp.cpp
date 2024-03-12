#include "FireworkApp.h"

#include <execution>
#include <iostream>
#include <ranges>
#include <span>
#include <sstream>  // For string manipulation

#include "MenuWindow.h"

namespace
{
std::atomic<float> globalTime;
std::atomic<bool> workerMustExit;
// std::atomic<size_t> gEffectSCount;
// std::atomic<size_t> gParticlesCount;

constexpr size_t MAX_EFFECTS = 2048;
constexpr size_t MAX_PARTICLES_PER_EFFECT = 64;

bool isParticleDead(const Particle& particle) { return !particle.isAlive(); }
bool areFireworksDead(const std::span<Particle> fireworks)
{
  return fireworks.empty();
}
}  // namespace

FireworksApp::FireworksApp(int width, int height)
    : window(sf::VideoMode(width, height), "Fireworks"),
      renderFireworksPtr1(&renderFireworks1),
      renderFireworksPtr2(&renderFireworks2),
      renderFireworksPtr3(&renderFireworks3)
{
  window.setFramerateLimit(60);

  // Load font for displaying FPS
  if (!font.loadFromFile("arial.ttf"))
  {
    std::cerr << "Arial_not found" << '\n';
  }
  fpsText.setFont(font);
  fpsText.setCharacterSize(20);
  fpsText.setFillColor(sf::Color::White);
  fpsText.setPosition(10.f, 10.f);

  std::thread workerThread([this] { mainUpdateLoop(); });
  workerThread.detach();

  updateFireworks.reserve(MAX_PARTICLES_PER_EFFECT * MAX_EFFECTS);
  renderFireworks1.reserve(MAX_PARTICLES_PER_EFFECT * MAX_EFFECTS);
  renderFireworks2.reserve(MAX_PARTICLES_PER_EFFECT * MAX_EFFECTS);
  renderFireworks3.reserve(MAX_PARTICLES_PER_EFFECT * MAX_EFFECTS);
}

void FireworksApp::run()
{
  while (window.isOpen())
  {
    MenuWindow menu(window);
    MenuWindow::MenuResult result = menu.show();

    if (result == MenuWindow::MenuResult::Continue)
    {
      while (window.isOpen())  // Game loop
      {
        const float deltaTime = clock.restart().asSeconds();
        globalTime.fetch_add(deltaTime);
        processEvents();
        render(deltaTime);
        std::this_thread::yield();
      }
    }
    else if (result == MenuWindow::MenuResult::Exit)
    {
      window.close();
    }
  }
}

void FireworksApp::processEvents()
{
  sf::Event event;
  std::vector<sf::Vector2f> newParticles;
  while (window.pollEvent(event))
  {
    if (event.type == sf::Event::Closed)
    {
      window.close();
    }
    else if (event.type == sf::Event::MouseButtonPressed)
    {
      if (event.mouseButton.button == sf::Mouse::Left)
      {
        newParticles.emplace_back(static_cast<float>(event.mouseButton.x),
                                  static_cast<float>(event.mouseButton.y));
      }
    }
    else if (event.type == sf::Event::KeyPressed)
    {
      if (event.key.code == sf::Keyboard::Escape)
      {
        MenuWindow menu(window);
        MenuWindow::MenuResult result = menu.show();

        if (result == MenuWindow::MenuResult::Continue)
        {
          // Continue game
        }
        else if (result == MenuWindow::MenuResult::Exit)
        {
          window.close();
        }
      }
    }
  }

  addParticles(newParticles);
}

void FireworksApp::update(float deltaTime)
{
  if (newParticleMutex.try_lock())
  {  // add new particles safely
    auto newParticlePositions = mNewParticlePositions;
    mNewParticlePositions.clear();
    newParticleMutex.unlock();

    for (const auto& position :
         newParticlePositions |
             std::views::take_while(
                 [this](auto) { return updateFireworks.size() < MAX_EFFECTS; }))
    {
      std::vector<Particle> particles;
      particles.reserve(MAX_PARTICLES_PER_EFFECT);
      for (size_t i = 0; i < MAX_PARTICLES_PER_EFFECT; ++i)
      {
        particles.emplace_back(position.x, position.y);
      }
      updateFireworks.emplace_back(std::move(particles));
    }
  }

  std::for_each(std::execution::par_unseq,
                updateFireworks.begin(),
                updateFireworks.end(),
                [deltaTime](auto& firework)
                {
                  for (auto& particle : firework)
                  {
                    particle.update(deltaTime);
                  }

                  const auto ret =
                      std::ranges::remove_if(firework, isParticleDead);
                  firework.erase(ret.begin(), ret.end());
                });

  const auto ret = std::ranges::remove_if(updateFireworks, areFireworksDead);
  updateFireworks.erase(ret.begin(), ret.end());

  renderFireworksPtr1->assign(updateFireworks.begin(), updateFireworks.end());

  if (swapFireworksMutex.try_lock())
  {
    std::swap(renderFireworksPtr1, renderFireworksPtr2);
    swapFireworksMutex.unlock();
  }
}

void FireworksApp::mainUpdateLoop()
{
  while (!workerMustExit)
  {
    try
    {
      static float lastTime = 0;
      const auto time = globalTime.load();
      const float delta = time - lastTime;
      lastTime = time;

      if (delta > 0)
      {
        update(delta);
      }

      const int deltaMs = delta / 1000;

      static constexpr int MIN_UPDATE_PERIOD_MS = 10;
      if (deltaMs < MIN_UPDATE_PERIOD_MS)
      {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(MIN_UPDATE_PERIOD_MS - deltaMs));
      }
    }
    catch (std::exception& e)
    {
      std::stringstream stream;
      stream << e.what() << '\n';
      std::cerr << stream.str();
    }
    catch (...)
    {
      // Что-то пошло не так
    }
  }
}

void FireworksApp::render(float deltaTime)
{
  window.clear();
  // std::unique_lock lock(swapFireworksMutex);
  // std::swap(renderFireworksPtr2, renderFireworksPtr3);
  // lock.unlock();
  if (swapFireworksMutex.try_lock())
  {
    std::swap(renderFireworksPtr2, renderFireworksPtr3);
    swapFireworksMutex.unlock();
  }
  for (const auto& firework : *renderFireworksPtr3)
  {
    for (const auto& particle : firework)
    {
      sf::CircleShape dot(1.f);
      dot.setFillColor(particle.getColor());
      dot.setPosition(particle.getPosition());
      window.draw(dot);

      for (const auto& [position, color] : particle.getTail())
      {
        dot.setFillColor(color);
        dot.setPosition(position);
        window.draw(dot);
      }
    }
  }
  // lock.unlock();

  // Calculate FPS and update FPS text
  float fps = 1.f / deltaTime;
  std::wstringstream ss;
  ss << L"FPS: " << static_cast<int>(fps) << "\n";
  ss << L"Firework: " << renderFireworksPtr3->size() << "\n";

  auto particlesCount = std::transform_reduce(std::execution::par_unseq,
                                              renderFireworksPtr3->begin(),
                                              renderFireworksPtr3->end(),
                                              size_t(0),
                                              std::plus{},
                                              [](std::vector<Particle>& val)
                                              { return val.size(); });
  ss << L"Particle: " << particlesCount << "\n";
  fpsText.setString(ss.str());

  window.draw(fpsText);  // Draw FPS counter
  window.display();
}

void FireworksApp::addParticles(std::span<sf::Vector2f> newParticlePositions)
{
  std::unique_lock lock(newParticleMutex);
  mNewParticlePositions.insert(mNewParticlePositions.end(),
                               newParticlePositions.begin(),
                               newParticlePositions.end());
}
