include(FetchContent)

FetchContent_Declare(googletest
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
        )

set(gtest_disable_pthreads OFF CACHE BOOL "" FORCE)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(run_tests
        test_inverted_index.cpp
        test_search_server.cpp
        ../src/converter_json.cpp
        ../src/entry.cpp
        ../src/inverted_index.cpp
        ../src/relative_index.cpp
        ../src/search_server.cpp
        )

target_link_libraries(search_engine  PRIVATE gtest_main )
target_include_directories(run_tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
        )

include(GoogleTest)
gtest_discover_tests(run_tests)