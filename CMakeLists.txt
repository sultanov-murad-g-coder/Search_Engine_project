cmake_minimum_required(VERSION 3.25)

project(search_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(RESOURCES_DIR ${CMAKE_SOURCE_DIR}/resources)
set(CONFIG_DIR ${CMAKE_SOURCE_DIR})
set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)
add_subdirectory(src)
add_subdirectory(tests)




