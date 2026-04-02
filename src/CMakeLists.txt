cmake_minimum_required(VERSION 3.25)

add_executable(search_engine
        engine.cpp
        ConverterJSON.cpp
        InvertedIndex.cpp
        SearchServer.cpp
        RelativeIndex.cpp
        Entry.cpp)

target_include_directories(search_engine PUBLIC ${INCLUDE_DIR})

target_link_libraries(search_engine  PRIVATE nlohmann_json::nlohmann_json)

target_compile_definitions(search_engine PRIVATE
        RESOURCES_PATH="${RESOURCES_DIR}"
        CONFIG_PATH="${CONFIG_DIR}"
        )