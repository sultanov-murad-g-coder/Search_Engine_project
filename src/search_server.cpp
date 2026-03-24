#include "..\include\SearchServer.h"

 SearchServer::SearchServer(InvertedIndex &idx) : _index(idx){};

 vector<RelativeIndex> SearchServer::SortFunc(const vector<RelativeIndex> &vector_in)
{
    vector<RelativeIndex> vec_final,vec_manip,vec_final_cut;

    for(size_t i=0; i<vector_in.size(); i++)
    {
        if ( i+1<vector_in.size() && vector_in[i].rank==vector_in[i+1].rank)
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

 vector<vector<RelativeIndex>> SearchServer::Search (const vector<string> &queries_input)
 {
    vector<vector<RelativeIndex>> res_search; // вектор возвращаемый функцией

    vector<Entry> vec_entry; // вектор частот

    vector<vector<string>> vec_for_work; // вектор векторов для хранения слов по каждому запросу

    vec_for_work.reserve(queries_input.size());// резервируем память для вектора размером с количество запросов

    //Разбиваем каждый запрос на отдельные слова и записываем в vec_for_work
    for(auto &request_:queries_input)
     {
       vec_for_work.emplace_back(InvertedIndex::MakeVector(request_));
     }

    for(auto &vec_in:vec_for_work)
     {
        vec_in = EraseElement(vec_in);// удаление из каждого запроса повторяющихся слов
        vector<Entry> vec_for_map;
        size_t max_id_doc=0;
        vector <size_t> vec_id_doc;

        for (auto &word:vec_in)
        {
            vec_entry = _index.GetWordCount(word); // получаем вектор частот по каждому слову из запроса
            if (!vec_entry.empty()) // Если вектор не пуст добавляем в новый частотный вектор все частоты
            {
              for(auto &el:vec_entry)
                {
                  vec_for_map.emplace_back(el);

                  if (vec_id_doc.empty()){ vec_id_doc.emplace_back(el.doc_id);} // создаем вектор состоящий из id документов без повторений id

                  auto it= find(vec_id_doc.begin(), vec_id_doc.end(),el.doc_id);

                  if (it==vec_id_doc.end())
                    {
                       vec_id_doc.emplace_back(el.doc_id);
                    }
                }
            }
            else { continue;}
        }

        if (!vec_for_map.empty())
        {
            sort(vec_id_doc.begin(),vec_id_doc.end()); // сортируем вектор состоящий из id документов
            max_id_doc=vec_id_doc.back()+1; // берем элемент вектора с максимальным значением id и добавялем 1
            vector<size_t> sum_vec(max_id_doc); // собираем вектор сумм

            for(auto &f:vec_for_map) // вектор vec_for_map содержит все частоты каждого слова из запроса
             {
                for(auto i=0; i<max_id_doc; i++)
                {
                    if (f.doc_id!=i) {continue;}
                    else {sum_vec[i]+=f.count; } // находим сумму частот из слов запроса по каждому документу
                }
             }
          
            // ищем максимальный элемент в векторе sum_vec

            vector<size_t> vec_until_sort = sum_vec;

            sort(sum_vec.begin(),sum_vec.end());
            // max element
            auto max_in_sum_vec = sum_vec.back();

            sum_vec.erase(remove(sum_vec.begin(), sum_vec.end(), 0),sum_vec.end());

            vector<RelativeIndex> prom_vec_relative;

            for(size_t pos=0; pos<vec_until_sort.size(); pos++)
             {
               if (vec_until_sort[pos]==0) { continue;}
               else
                {
                 prom_vec_relative.emplace_back(pos,(float)vec_until_sort[pos]/(float)max_in_sum_vec);
                }
             }

            sort(prom_vec_relative.begin(),prom_vec_relative.end(),[](const RelativeIndex &a,const RelativeIndex &b)->bool{
                return a.rank>b.rank;
            });

            res_search.emplace_back(SortFunc(prom_vec_relative));// добавляем результат по запросу

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

 vector<vector<pair<int,float>>> SearchServer::MakeVecForAnswer(const vector<vector<RelativeIndex>> &res_search)
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

 vector<string> SearchServer::EraseElement(vector<string> &input_vec)
 {
    vector<string> vec_transformed;

    sort(input_vec.begin(),input_vec.end());

    for(size_t i=0; i<input_vec.size(); i++)
     {
        if (input_vec[i]==input_vec[i+1] && i+1<input_vec.size()) { continue;}

        else {vec_transformed.emplace_back(input_vec[i]);}
     }

    return vec_transformed;
 }