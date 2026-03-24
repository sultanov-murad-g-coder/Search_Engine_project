#pragma once
#include <iostream>

using namespace std;

struct Entry
{
    size_t doc_id,count;

    Entry()=default;
    Entry(size_t doc_id, size_t count);

    bool operator==( const Entry &other) const;
};

#ifndef REQUEST_JSON_ENTRY_H
#define REQUEST_JSON_ENTRY_H

#endif //REQUEST_JSON_ENTRY_H
