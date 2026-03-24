#include <iostream>
#include <gtest/gtest.h>
#include "..\include\ConverterJSON.h"
#include "..\include\InverterIndex.h"
#include "..\include\SearchServer.h"


/*
struct Entry
{
   size_t doc_id,count;

   Entry()=default;
   Entry(size_t doc_id, size_t count)
   {
     this->doc_id=doc_id;
     this->count=count;
   }

   bool operator==( const Entry &other) const
   {
      return (doc_id==other.doc_id && count==other.count);
   }
};

struct RelativeIndex
 {
    size_t doc_id;
    float rank;

    RelativeIndex()=default;
    RelativeIndex(size_t doc_id, float rank)
     {
        this->doc_id=doc_id;
        this->rank=rank;
     }

    bool operator == (const RelativeIndex &other) const
     {
         return (doc_id==other.doc_id && rank==other.rank);
     }
 };

class ConverterJSON
{

public:
  ConverterJSON()=default;

  static vector<string> GetTextDocuments()
   {
      vector<string> vector_all_data;
      string str_line,doc_txt;
      nlohmann::json json_file;
      ifstream json_file_config,doc_config;

      json_file_config.open("..//config.json");

      if (json_file_config.is_open())
       {
          json_file_config >> json_file;

          for(auto &i:json_file["files"])
           {
              doc_config.open(i);
              if (doc_config.is_open())
               {
                   while(!doc_config.eof())
                    {
                       str_line.clear();
                       getline(doc_config,str_line);
                       doc_txt+=(" "+str_line);
                    }

                   vector_all_data.emplace_back(doc_txt);
                   doc_txt.clear();
                   doc_config.close();
               }
              else
               {
                  cout<< "error open resource file " << "\n";
               }
           }

          json_file_config.close();
       }

      else
      {
          cout<< "error open config file";
      }

      return vector_all_data;
   }

  static int GetResponsesLimit()
   {
       ifstream file("..//config.json");
       json dict;
       file>>dict;
       return dict["config"]["max_responses"];
   }

  static vector<string> GetRequest()
   {
       vector<string> request;
       ifstream file("..//request.json");
       nlohmann::json dict;

       file>>dict;

       for (auto &i:dict["request"])
        {
          request.emplace_back(i);
        }

       return  request;
   }

  static void PutAnswers(vector<vector<pair<int,float>>> &answers)
   {
     string symbol;
     ofstream file;
     file.open("..//answers.json");
     file.clear();
     file << "{"<<"\n";
     file << "\""<<"answers"<<"\""<<":"<<"{"<<"\n";

     for(int i=0; i<answers.size(); ++i)
      {
         string b_str;
         bool empty_data;
         char symbol_result;

         if (i+1<=9) { b_str="request00";}
         else if(i+1>=10 && i+1<=99) {b_str="request0";}
         else {b_str="request";}

         answers[i].empty() ? empty_data=true : empty_data=false; // провервка вектора на наличие данных

         file<<setw(3)<<"\""<<b_str+to_string(i+1)<<"\""<<":"<<"{"<<"\n";

         empty_data ?  symbol_result=' ' : symbol_result=',';

         file<<setw(5)<<"\""<<"result"<<"\""<<":"<<"\""<<boolalpha<<(!empty_data)<<"\""<<symbol_result<<"\n";

         if(empty_data)
          {
              ((i+1)<answers.size()) ? symbol="}," : symbol="}";
              file<<setw(6)<<symbol<<"\n";
              continue;
          }

         file<<setw(5)<<"\"" <<"relevance"<<"\":["<<"\n";

         int count=0;
         for(auto &j:answers[i])
         {
             count++;
             (count<answers[i].size()) ? symbol="},":symbol="}";

             file <<setw(10)<<"{"<<"\""<<"docID"<<"\""<< ":" <<j.first<<","
                  <<"\t"<<"\""<<"rank"<<"\""<<":" <<j.second<<symbol<<"\n";
         }

         file<<setw(8)<<"]"<<"\n";

         ((i+1)<answers.size()) ? symbol="},": symbol="}";
         file<<setw(6)<<symbol<<"\n";

      }

     file<<setw(4)<<"}"<<"\n";
     file<<"}";
     file.close();
   }

};

class InvertedIndex
 {

private:
    vector<string> docs;
    map<string, vector<Entry>> freq_dictionary;
public:
  InvertedIndex()=default;

  static vector<string> MakeVector(const string &in_str)
     {
        string n_str;
        n_str.clear();
        vector<string> vec_words;
        for(auto i=0; i<in_str.length(); i++)
        {
            if((in_str[i]>='a' && in_str[i] <='z') || (in_str[i]>='A' && in_str[i] <='Z'))
             {
                n_str+=in_str[i];
                if (in_str.length()-1==i && !n_str.empty()) {vec_words.emplace_back(n_str); n_str.clear();}

             }
            else if(in_str[i]==' ' && !n_str.empty())
             {
                vec_words.emplace_back(n_str);
                n_str.clear();
             }
        }
        return vec_words;
     }

  static map<string,Entry> MakeMap(const vector<string> &input_docs,size_t pos)
   {
       vector<string> _vec;  map<string,Entry> freq_dictionary_map;   size_t count=1;

       _vec = MakeVector(input_docs[pos]);

       sort(_vec.begin(), _vec.end());

       for (size_t i=0; i<_vec.size(); i++)
       {
         if (i+1<_vec.size() &&_vec[i]==_vec[i+1])
           {
               count++;
           }
         else
           {
              freq_dictionary_map[_vec[i]]={pos,count};
              count=1;
           }
       }

       return freq_dictionary_map;
   }

  void UpdateDocumentBase(const vector<string> &input_docs)
   {
      vector<thread> t_threads;
      vector<map<string,Entry>> freq_dictionary_all_map(input_docs.size());

      docs=input_docs;

      for(size_t f=0; f<input_docs.size(); f++)
       {
          this_thread::sleep_for(chrono::milliseconds (15));
          t_threads.emplace_back([f,&input_docs,&freq_dictionary_all_map]()
           {
              freq_dictionary_all_map[f]=MakeMap(input_docs,f);
           });
       }

      for(auto &t:t_threads)
       {
           t.join();
       }

      for(auto & iter : freq_dictionary_all_map)
        {
          for(auto & it : iter)
            {
               freq_dictionary[it.first].emplace_back(it.second);
            }
        }
   }

  vector<Entry> GetWordCount(const string &word)
   {
       auto iter=freq_dictionary.find(word);

       if(iter!=freq_dictionary.end())
        {
            return iter->second;
        }
       else
        {
           freq_dictionary[word].emplace_back(0,1);
           vector<Entry> vec;
           return vec;
        }

   }

  void PrintData()
   {
       for(int i=0; i<docs.size(); i++)
       {
          cout << i+1 << "  line" << "\n";
          cout << "-------------------------------------------" <<"\n";
          cout << docs[i] <<"\n";
          cout << "-------------------------------------------" <<"\n";
       }
   }

  void PrintMap()
   { int count=1;
       for (auto & iter : freq_dictionary)
        {
          cout<<"# "<<count<<" " << iter.first << " ";
           for(auto & it : iter.second)
            {
               cout<< "{"<<it.doc_id<<","<<it.count<<"} ";
            }
           count++;
           cout<<"\n";
        }
   }

  void PrintFreqWord(vector<string> request)
   {
      for(auto &l:request)
      {
        if(!GetWordCount(l).empty())
           {
              cout << l << " ";
              for (auto &h: GetWordCount(l))
               {
                  cout<<"{"<< h.doc_id << "," <<h.count<<"}";
               }
              cout << "\n";
           }
      }

   }
 };

class SearchServer
{
  private:
    InvertedIndex _index;
  public:
   SearchServer()=default;

   explicit SearchServer(InvertedIndex &idx) : _index(idx) {};

   vector<vector<RelativeIndex>> Search (const vector<string> &queries_input)
    {
       vector<vector<RelativeIndex>> res_search;

       vector<Entry> vec_entry;

       map<string,vector<Entry>> map_in;

       vector<vector<string>> vec_for_work;

       vec_for_work.reserve(queries_input.size());

       for(auto &request_:queries_input)
        {
           vector<string> vec=InvertedIndex::MakeVector(request_);
           vec_for_work.emplace_back(vec);
        }

       for(auto &vec_in:vec_for_work)
         {
              vector<Entry> vec_for_map;
              size_t count_doc=0;
              vector <size_t> id_doc;
              for (auto &word:vec_in)
                {
                   vec_entry = _index.GetWordCount(word);
                   if (!vec_entry.empty())
                    {
                      for(auto &el:vec_entry)
                        {
                          vec_for_map.emplace_back(el);

                          if (id_doc.empty()){ id_doc.emplace_back(el.doc_id);}

                          auto it= std::find(id_doc.begin(), id_doc.end(),el.doc_id);

                          if (it==id_doc.end())
                           {
                             id_doc.emplace_back(el.doc_id);
                           }

                         }
                    }
                   else { continue;}
                }

              if (!vec_for_map.empty())
               {
                   count_doc=id_doc.size();
                   vector<size_t> sum_vec(count_doc); // собираем вектор сумм

                   for(auto &f:vec_for_map)
                   {
                       for(auto i=0; i<count_doc; i++)
                        {
                          if (f.doc_id==i) {sum_vec[i]+=f.count;}
                        }

                   }
                   // ищем максимальный элемент в векторе sum_vec

                   vector<size_t> vec_until_sort = sum_vec;
                   auto it=find(vec_until_sort.begin(), vec_until_sort.end(),0);
                   if (it!=vec_until_sort.end()) {vec_until_sort.erase(it);}

                   sort(sum_vec.begin(),sum_vec.end());
                   // max element
                   auto max_in_sum_vec = sum_vec.back();

                   size_t pos=0;

                   vector<RelativeIndex> prom_vec_relative(vec_until_sort.size());

                   for(auto &rel_el:prom_vec_relative)
                   {
                       rel_el.doc_id=pos; rel_el.rank=(float)vec_until_sort[pos]/(float)max_in_sum_vec;
                       pos++;
                   }

                   sort(prom_vec_relative.begin(),prom_vec_relative.end(),[](const RelativeIndex &a,const RelativeIndex &b)->bool{
                       return a.rank>b.rank;
                   });

                   res_search.emplace_back(SortFunc(prom_vec_relative));

                   map_in.clear();
                   vec_for_map.clear();
               }
              else
               {
                  vector<RelativeIndex> prom_vec_relative;
                  res_search.emplace_back(prom_vec_relative);
               }
          }

       return res_search;
    }

   static vector<RelativeIndex> SortFunc(const vector<RelativeIndex> &vector_in)
    {
      vector<RelativeIndex> vec_final,vec_manip,vec_final_cut;

      for(size_t i=0; i<vector_in.size(); i++)
      {
         if (vector_in[i].rank==vector_in[i+1].rank)
         {
           vec_manip.emplace_back(vector_in[i]);
         }
         else
         {
           vec_manip.emplace_back(vector_in[i]);
           sort(vec_manip.begin(),vec_manip.end(),[](const RelativeIndex &a, const RelativeIndex &b)->bool{
               return a.doc_id<b.doc_id;
           });
           for(auto & t : vec_manip)
            {
               vec_final.emplace_back(t);
            }
           vec_manip.clear();
         }
      }

      auto count_size = vec_final.size();
      auto max_responses  = ConverterJSON::GetResponsesLimit();
      if (count_size>max_responses) {count_size=max_responses;}

      for(auto j=0; j<count_size; j++)
      {
         vec_final_cut.emplace_back(vec_final[j]);
      }
      return vec_final_cut;
    }

   static vector<vector<pair<int,float>>> MakeVectorForAnswer(const vector<vector<RelativeIndex>> &res_search)
    {
        vector<vector<pair<int,float>>> vec_for_answer;
        for(auto & el_vector : res_search)
        {
            vector<pair<int,float>> vec;
            for(auto & el_pair : el_vector)
            {
                pair<int,float> _pair;
                _pair.first=(int)el_pair.doc_id;
                _pair.second=el_pair.rank;
                vec.emplace_back(_pair);
            }
            vec_for_answer.emplace_back(vec);
        }
        return vec_for_answer;
    }

   static void PrintResSearch(const vector<vector<RelativeIndex>> &vec_for_print)
   {
      for(auto &el:vec_for_print)
      {
         for(auto &elem:el)
          {
             cout << "{"<<elem.doc_id <<","<<elem.rank<<"} "<<"\n";
          }
         cout << "-----------------------------"<<"\n";
      }
   }

};

*/

/*void  TestInvertedIndexFunctionality (const vector<string> &docs,const vector<string> &requests, const vector<vector<Entry>> &expected)
{
   vector<vector<Entry>> result;
   InvertedIndex idx;
   idx.UpdateDocumentBase(docs);
   for(auto &request:requests)
    {
       vector<Entry> word_count=idx.GetWordCount(request);
        result.push_back(word_count);
    }
   ASSERT_EQ(result,expected);
}

TEST (TestCaseInvertedIndex,TestBasic)
{
    const vector<string> docs  =  {
            "london is the capital of great britain",
            "big ben is the nick name for the Great bell of the striking clock"
    };
    const vector<string> requests ={"london","the"};
    const vector<vector<Entry>> expected ={
            {
                    {0,1}
            },{
                {0,1},{1,3}
            }
                                          };
    TestInvertedIndexFunctionality(docs,requests,expected);
}

TEST(TestCaseInvertedIndex,TestBasic2){
    const vector<string> docs ={
            "milk milk milk milk water water","milk water water",
            "milk milk milk milk milk water water water water water","americano cappuccino"
    };
    const vector<string> requests ={"milk","water","cappuccino"};
    const vector<vector<Entry>> expected ={
            {
                    {0,4},{1,1},{2,5}
            },{
                    {0,2},{1,2},{2,5}
            },{
                    {3,1}
            }
    };
    TestInvertedIndexFunctionality(docs,requests,expected);
}

TEST(TestCaseInvertedIndex,TestInvertedIndexMissingWord){
    const vector<string> docs ={
            "abcdefghijkl","statement"
    };
    const vector<string> requests ={"m","statement"};
    const vector<vector<Entry>> expected ={
            {
            },{  {1,1}

            }
    };
    TestInvertedIndexFunctionality(docs,requests,expected);
}

TEST(TestCaseSearchServer,TestSimple)
 {
    const vector<string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk  water water water water water",
            "americano cappuccino"
     };

    const vector<string> request = {"milk water", "sugar"};

    const vector<vector<RelativeIndex>> expected = {
            {
                    {2,1},
                    {0,0.7},
                    {1,0.3}
            },
            {

            }

     };

    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);

   vector<vector<RelativeIndex>> result = srv.Search(request);

   ASSERT_EQ(result, expected);
 }

TEST(TestCaseSearchServer, TestTop5)
 {
    const vector<string> docs = {
            "london is the capital of great britain",
            "paris is the capital of france",
            "berlin is the capital of germany",
            "rome is the capital of italy",
            "madrid is the capital of spain",
            "lisbon is the capital of portugal",
            "bern is the capital switzerland",
            "moscow is the capital of russia",
            "kiev is the capital of ukraine",
            "minsk is the capital of belarus",
            "astana is the capital of kazakhstan",
            "beijing is the capital of china",
            "tokyo is the capital of japan",
            "bangkok is the capital of thailand",
            "welcome to moscow the capital of russia the third rome",
            "amsterdam is the capital of netherlands",
            "helsinki is the capital of finland",
            "oslo is the capital of norway",
            "stockholm is capital of sweden",
            "riga is the capital of latvia",
            "tallinn is the capital estonia",
            "warsaw is the capital of poland"
    };

    const vector<string> request = {{"moscow is the capital of russia"}};

    const vector<vector<RelativeIndex>> expected = {
            {
                    {7,1},
                    {14,1},
                    {0,0.666666687},
                    {1,0.666666687},
                    {2,0.666666687}
            }

    };

    InvertedIndex idx_;
    idx_.UpdateDocumentBase(docs);

   SearchServer srv(idx_);
   vector<vector<RelativeIndex>> result = srv.Search(request);

   ASSERT_EQ(result,expected);

 }*/


int main() {

   InvertedIndex inverter;

   inverter.UpdateDocumentBase(ConverterJSON::GetTextDocuments());

   SearchServer srv(inverter);

   vector<vector<RelativeIndex>> res=srv.Search(ConverterJSON::GetRequest());

   vector<vector<pair<int,float>>> vec_for_print;

   vec_for_print=SearchServer::MakeVecForAnswer(res);

   ConverterJSON::PutAnswers(vec_for_print);

   return 0;
}





