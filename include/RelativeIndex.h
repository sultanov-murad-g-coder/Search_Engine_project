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


