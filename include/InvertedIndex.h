#pragma once
#include <iostream>
#include <vector>
#include "..\include\Entry.h"
#include <map>

using namespace std;

class InvertedIndex
 {
private:
    vector<string> docs;
    map<string, vector<Entry>> freq_dictionary;
public:
    InvertedIndex()=default;

    static vector<string> MakeVector(const string &in_str);

    static map<string,Entry> MakeMap(const vector<string> &input_docs,size_t pos);

    void UpdateDocumentBase(const vector<string> &input_docs);

    vector<Entry> GetWordCount(const string &word);

    map<string, vector<Entry>> GetDict();
 };



