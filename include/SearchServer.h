#pragma once
#include "..\include\InverterIndex.h"
#include "..\include\RelativeIndex.h"
#include "..\include\ConverterJSON.h"

using namespace std;

class SearchServer
{
  private:
    InvertedIndex _index;
  public:
    SearchServer() = default;

    explicit SearchServer(InvertedIndex &idx);

    vector<vector<RelativeIndex>> Search (const vector<string> &queries_input);

    static vector<RelativeIndex> SortFunc(const vector<RelativeIndex> &vector_in);

    static vector<string> EraseElement(vector<string> &input_vec);

    static vector<vector<pair<int,float>>> MakeVecForAnswer(const vector<vector<RelativeIndex>> &res_search);
};
#ifndef REQUEST_JSON_SEARCHSERVER_H
#define REQUEST_JSON_SEARCHSERVER_H

#endif //REQUEST_JSON_SEARCHSERVER_H
