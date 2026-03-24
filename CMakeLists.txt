cmake_minimum_required(VERSION 3.25)

project(search_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)

include(FetchContent)
FetchContent_Declare(googletest
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip)

include_directories(${gtest_SOURCE_DIR}/include;${gtest_SOURCE_DIR})

set(gtest disable pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(search_engine
        src/engine.cpp
        src/converter_json.cpp
        src/entry.cpp
        src/inverter_index.cpp
        src/relative_index.cpp
        src/search_server.cpp

        include/ConverterJSON.h
        include/InverterIndex.h
        include/SearchServer.h
        include/Entry.h
        include/RelativeIndex.h)

target_link_libraries(search_engine  PRIVATE gtest_main )
target_link_libraries(search_engine  PRIVATE nlohmann_json::nlohmann_json)

enable_testing()

include(GoogleTest)
gtest_discover_tests(search_engine)





