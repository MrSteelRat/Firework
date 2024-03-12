# Устанавливаем минимально требуемую версию CMake
cmake_minimum_required(VERSION 3.25)

# Задаем название проекта и язык программирования
project(Firework CXX)

# Устанавливаем стандарт языка и параметры компиляции
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Ищем установленный Git
find_package(Git REQUIRED)
execute_process(
  COMMAND "${GIT_EXECUTABLE}" submodule update --init --recursive
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)

# Добавляем поддиректорию для SFML (исключаем из сборки)
add_subdirectory(deps/SFML EXCLUDE_FROM_ALL)

# Определяем цель приложения
file(GLOB_RECURSE APP_SOURCES "src/*.cpp" "src/*.h")
add_executable(app ${APP_SOURCES})
# Копирование текста 
file(GLOB data data/*)
file(COPY ${data} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# Линкуем библиотеки и указываем директории включений
target_link_libraries(app PRIVATE sfml-graphics)
target_include_directories(app PRIVATE src)