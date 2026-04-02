cmake_minimum_required(VERSION 3.25)
project(run_tests)

include(FetchContent)

FetchContent_Declare(googletest
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip)

set(gtest_disable_pthreads ON)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

include_directories(${googletest_SOURCE_DIR}/googletest/include)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../config.json
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/..)

add_executable(run_tests
        test_inverted_index.cpp
        test_search_server.cpp
        ${CMAKE_SOURCE_DIR}/src/InvertedIndex.cpp
        ${CMAKE_SOURCE_DIR}/src/SearchServer.cpp
        ${CMAKE_SOURCE_DIR}/src/Entry.cpp
        ${CMAKE_SOURCE_DIR}/src/RelativeIndex.cpp
        ${CMAKE_SOURCE_DIR}/src/ConverterJSON.cpp
        )

target_include_directories(run_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
        )

target_compile_definitions(run_tests PRIVATE
        CONFIG_PATH="${CONFIG_DIR}"
        RESOURCES_PATH="${RESOURCES_DIR}"
        )

target_link_libraries(run_tests PRIVATE
        gtest_main
        gtest
        nlohmann_json::nlohmann_json
        )

enable_testing()

include(GoogleTest)
gtest_discover_tests(run_tests)