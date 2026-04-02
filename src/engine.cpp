#include <iostream>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main() {

   InvertedIndex inverter;

   inverter.UpdateDocumentBase(ConverterJSON::GetTextDocuments());

   SearchServer srv(inverter);

   vector<vector<RelativeIndex>> res=srv.Search(ConverterJSON::GetRequest());

   vector<vector<pair<int,float>>> vec_for_print=SearchServer::MakeVecForAnswer(res);

   ConverterJSON::PutAnswers(vec_for_print);

   string str=ConverterJSON::GetNameProject();

   cout<<"-----------------------------"<<"\n";
   cout<<"Start\t"<<"< "<<str<<" >"<<"\t"<<"\n";
   cout<<"-----------------------------"<<"\n";

   return 0;
}





