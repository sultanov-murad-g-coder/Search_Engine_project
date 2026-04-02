#include "InvertedIndex.h"
#include <thread>
#include <algorithm>

 vector<string>InvertedIndex::MakeVector(const string &in_str)
 {
    string word_str;
    word_str.clear();
    vector<string> vec_words;

    for(auto i=0; i<in_str.length(); i++)
    {
        if((in_str[i]>='a' && in_str[i] <='z') || (in_str[i]>='A' && in_str[i] <='Z'))
        {
            word_str+=in_str[i];
            if (in_str.length()-1==i && !word_str.empty()) {vec_words.emplace_back(word_str); word_str.clear();}
        }
        else if(in_str[i]==' ' && !word_str.empty())
        {
            vec_words.emplace_back(word_str);
            word_str.clear();
        }
    }

    return vec_words;
 }

 map<string,Entry>InvertedIndex::MakeMap(const vector<string> &input_docs,size_t pos)
 {
    vector<string> vec_words;  map<string,Entry> freq_dictionary_map;   size_t count=1;

    vec_words = MakeVector(input_docs[pos]);

    sort(vec_words.begin(), vec_words.end());

    for (size_t i=0; i<vec_words.size(); i++)
    {
        if (i+1<vec_words.size() && vec_words[i]==vec_words[i+1]) { count++; }

        else
        {
            freq_dictionary_map[vec_words[i]]={pos,count};
            count=1;
        }
    }

    return freq_dictionary_map;
 }

 void InvertedIndex::UpdateDocumentBase(const vector<string> &input_docs)
 {
    vector<thread> t_threads;
    vector<map<string,Entry>> freq_dictionary_all_map(input_docs.size());
    docs=input_docs;

    for(size_t f=0; f<input_docs.size(); f++)
    {
        this_thread::sleep_for(chrono::milliseconds (15));
        t_threads.emplace_back([f,&input_docs,&freq_dictionary_all_map]()
        {
            freq_dictionary_all_map[f] = MakeMap(input_docs,f);
        });
    }

    for(auto &t:t_threads) { t.join(); }

    for(auto & iter : freq_dictionary_all_map)
    {
        for(auto & it : iter)
        {
            freq_dictionary[it.first].emplace_back(it.second);
        }
    }
 }

 vector<Entry> InvertedIndex::GetWordCount(const string &word)
 {
    auto iter=freq_dictionary.find(word);

    if(iter!=freq_dictionary.end()) { return iter->second; }

    else { return {}; }

 }

 map<string,vector<Entry>> InvertedIndex::GetDict()
 {
    return freq_dictionary;
 }
