#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include <stdexcept>

using namespace std;
using namespace nlohmann;

class ConverterJSON
 {
 public:
    ConverterJSON()=default;

    static vector<string> GetTextDocuments();

    static int GetResponsesLimit();

    static vector<string> GetRequest();

    static void PutAnswers(vector<vector<pair<int,float>>> &answers);
 };


