#pragma once
#include <iostream>

struct Entry
{
    size_t doc_id,count;

    Entry()=default;
    Entry(size_t doc_id, size_t count);

    bool operator==( const Entry &other) const;
};

