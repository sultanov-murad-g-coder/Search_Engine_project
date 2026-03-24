#pragma once
#include <iostream>
struct RelativeIndex
{
    size_t doc_id;
    float rank;

    RelativeIndex()=default;
    RelativeIndex(size_t doc_id, float rank);


    bool operator == (const RelativeIndex &other) const;

};

#ifndef REQUEST_JSON_RELATIVEINDEX_H
#define REQUEST_JSON_RELATIVEINDEX_H

#endif //REQUEST_JSON_RELATIVEINDEX_H
