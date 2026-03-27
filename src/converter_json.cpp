#include "..\include\ConverterJSON.h"

 vector<string> ConverterJSON::GetTextDocuments()
 {
    vector<string> vector_all_data;
    try
     {
        ifstream json_file_config,doc_config;

        json_file_config.open("..//config.json");

        if (json_file_config.is_open())
          {
             json json_file;

             json_file_config >> json_file;

             try
              {
                auto iterator_json = json_file.find("config");

                if (iterator_json!=json_file.end())
                   {
                      for(auto &path_file:json_file["files"])
                      {
                          try
                           {
                              doc_config.open(path_file);
                              if (doc_config.is_open())
                               {
                                  string str_line,doc_txt;
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
                                  throw runtime_error("Error opening source_file " + (string)path_file);
                               }
                           }
                          catch(const runtime_error &ex)
                           {
                              cout << ex.what() << "\n";
                              continue;
                           }
                          catch(...)
                           {
                              cout << "Error unknown ";
                              continue;
                           }
                      }

                      json_file_config.close();
                   }
                else
                   {
                      throw runtime_error("config file is empty");
                   }
              }
             catch(const runtime_error &ex)
              {
                cout << ex.what();
              }
             catch(...)
              {
                cout<< "Error unknown";
              }
          }

        else
          {
             throw runtime_error("config file is missing");
          }
     }

    catch(const runtime_error &ex)

      {
         cout << ex.what() <<"\n";
      }

    catch (...)
      {
         cout << "Error unknown" << "\n";
      }

    return vector_all_data;
 }

 int ConverterJSON::GetResponsesLimit()
  {
    json dict;

    try
     {
       ifstream file_config;
       file_config.open("..//config.json");

       if (file_config.is_open())
        {
           file_config >> dict;
           return dict["config"]["max_responses"];
        }

       else  {throw runtime_error("config file is missing");}
     }

    catch (const runtime_error &ex)
     {
       cout << ex.what()<<"\n";
     }

    catch(...)
     {
       cout << "error unknown"<<"\n";
     }

    return 0;
  }

 vector<string> ConverterJSON::GetRequest()
  {
     try
      {

         ifstream file_request("..//request.json");

         if (file_request.is_open())
          {
            json dict;
            file_request >> dict;
            vector<string> request;
            for (auto &el:dict["request"])
             {
                request.emplace_back(el);
             }
            return  request;
          }

         else {throw runtime_error("request file is missing");}
      }

     catch (const runtime_error &ex)
     {
       cout << ex.what() << "\n";
     }

     catch (...)
     {
       cout << "error unknown"<<"\n";
     }

     return {};
  }

 void ConverterJSON::PutAnswers (vector<vector<pair<int,float>>> &answers)
    {
        string symbol;
        ofstream file_for_write;

        file_for_write.open("..//answers.json");
        file_for_write.clear();
        file_for_write<< "{"<<"\n";
        file_for_write << "\""<<"answers"<<"\""<<":"<<"{"<<"\n";

        for(int i=0; i<answers.size(); ++i)
         {
            string b_str;
            bool empty_data;
            char symbol_result;

            if (i+1<=9) { b_str="request00";}
            else if(i+1>=10 && i+1<=99) {b_str="request0";}
            else {b_str="request";}

            answers[i].empty() ? empty_data=true : empty_data=false; // провервка вектора на наличие данных

             file_for_write << setw(3)<<"\""<<b_str+to_string(i+1)<<"\""<<":"<<"{"<<"\n";

            empty_data ?  symbol_result=' ' : symbol_result=',';

             file_for_write << setw(5)<<"\""<<"result"<<"\""<<":"<<"\""<<boolalpha<<(!empty_data)<<"\""<<symbol_result<<"\n";

            if(empty_data)
             {
               ((i+1)<answers.size()) ? symbol="}," : symbol="}";
                 file_for_write << setw(6)<<symbol<<"\n";
               continue;
             }

             file_for_write << setw(5)<<"\"" <<"relevance"<<"\":["<<"\n";

             int count=0;
             for(auto &j:answers[i])
              {
                count++;
                (count<answers[i].size()) ? symbol="},":symbol="}";

                  file_for_write << setw(10)<<"{"<<"\""<<"docID"<<"\""<< ":" <<j.first<<","
                <<"\t"<<"\""<<"rank"<<"\""<<":" <<j.second<<symbol<<"\n";
              }

             file_for_write << setw(8)<<"]"<<"\n";

             ((i+1)<answers.size()) ? symbol="},": symbol="}";
             file_for_write << setw(6)<<symbol<<"\n";

         }

        file_for_write << setw(4)<<"}"<<"\n";
        file_for_write <<"}";
        file_for_write.close();
    }

 string ConverterJSON::GetNameProject()
  {
    json dict;
    ifstream file_config;

    try
     {
       file_config.open("..//config.json");
        if(file_config.is_open())
        {
            file_config >> dict;
            return dict["config"]["name"];
        }
        else {throw runtime_error("config file is missing or error opening");}
     }

    catch (const runtime_error &ex)
    {
       cout << ex.what() << "\n";
    }

    catch (...)
    {
       cout<<"unknown error in method GetName";
    }

   return {};
  }