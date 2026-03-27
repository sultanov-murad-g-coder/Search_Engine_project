cmake_minimum_required(VERSION 3.25)

project(search_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build unit tests" OFF)

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(nlohmann_json)

add_executable(search_engine
        src/engine.cpp
        src/converter_json.cpp
        src/entry.cpp
        src/inverted_index.cpp
        src/relative_index.cpp
        src/search_server.cpp
        include/ConverterJSON.h
        include/InvertedIndex.h
        include/SearchServer.h
        include/Entry.h
        include/RelativeIndex.h
        )
target_link_libraries(search_engine  PRIVATE nlohmann_json::nlohmann_json)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()





