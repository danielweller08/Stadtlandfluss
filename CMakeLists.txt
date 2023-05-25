## Konfiguration des Buildsystems CMake ##

# Minimale Version des Buildsystems
cmake_minimum_required(VERSION 3.14)
# Name des Projekts
project(StadtLandFluss)

## Optionen ##

# Setzte verwendeten C++-Standard auf C++17
set(CMAKE_CXX_STANDARD 17)

# Prüfe ob Heapspeicher gefunden wird,
# der nicht freigegeben wurde
# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

# Füge selbsgeschriebene Includes hinzu
include_directories(include)
# Füge externe Includes hinzu
include_directories(external)

# Sammle die Quelldateien
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Baue die demo-Programme
add_executable(demo_cpp
  examples/demo_cpp.cpp
  ${SRC_FILES}
)

# Use CMakes FetchContent
include(FetchContent)
# Workaround for CMake >= 3.24
# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
  cmake_policy(SET CMP0135 NEW)
endif()

# Download GoogleTest (we use version 1.12.1)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# Workaround for Windows:
# Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Include CMake configuration of GoogleTest
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# Enable GoogleTest
enable_testing()

# Declare test program testController
add_executable(testController unittests/testController.cpp ${SRC_FILES})
target_link_libraries(testController GTest::gtest_main)
gtest_discover_tests(testController)
