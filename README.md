# Search Engine

## Project Description

This project is a prototype of a search engine.
It implements search for relevant responses to user queries. 
The search is performed in a list of text documents located in the project's resources folder.

## Technology Stack

1. C++17 standard
2. IDE: CLion 2023
3. Git ver. 2.53.0.2
4. CMake ver. 3.25+
5. Google Test
6. MinGW compiler

## Requirements

1. CMake 3.25+
2. Compiler with C++14 / C++17 support
3. Git

## How to Build and Run

Download the project source files.

### Build without unit tests (MSVC compiler)

 cd search_engine
 mkdir build
 cd build
 cmake -DBUILD_TESTS=OFF ..
 cmake --build .

### Build with unit tests (MSVC compiler)

 cd search_engine
 mkdir build
 cd build
 cmake -DBUILD_TESTS=ON ..
 cmake --build .

### Build without unit tests (MinGW compiler)

 cd search_engine
 mkdir build
 cd build
 cmake -G "MinGW Makefiles" -DBUILD_TESTS=OFF ..
 mingw32-make

### Build with unit tests (MinGW compiler)
 cd search_engine
 mkdir build
 cd build
 cmake -G "MinGW Makefiles" -DBUILD_TESTS=ON ..
 mingw32-make

text

## Configuration Files

 Before running the executable, configure the following files:

### requests.json - contains an array of search queries

 Example:

 {
   "requests": [
   "first query",
   "second query"
   ]
 }

### config.json - contains paths to documents and search settings

 By default, it is correctly filled; changes are optional.

 Example:

 {
   "config": {
   "name": "project name",
   "version": "0.1",
   "max_responses": 5
 },
   "files": [
   "../path/to/document.txt"
  ]
}

## Run Commands
 cd build
 ./search_engine

 For Windows:
 cd build
 search_engine.exe

## Run Tests

 cd build
 ./tests/run_tests

 For Windows:
 cd build
 tests\run_tests.exe


## Output

 The search result is saved to answers.json in the project root directory.

## Project Structure

search_engine/
├── include/
│   ├── ConverterJSON.h
│   ├── Entry.h
│   ├── InvertedIndex.h
│   ├── RelativeIndex.h
│   └── SearchServer.h
├── src/
│   ├── converter_json.cpp
│   ├── entry.cpp
│   ├── inverted_index.cpp
│   ├── relative_index.cpp
│   ├── search_server.cpp
│   └── engine.cpp
├── tests/
│   ├── test_inverted_index.cpp
│   └── test_search_server.cpp
├── config.json
├── requests.json
├── CMakeLists.txt
└── README.md

## Author

Sultanov Murad
Email: S.Murad.G@mail.ru