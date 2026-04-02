#include "Entry.h"

  Entry::Entry(size_t doc_id, size_t count)
  {
    this->doc_id=doc_id;
    this->count=count;
  }

  bool Entry::operator==( const Entry &other) const
  {
    return (doc_id==other.doc_id && count==other.count);
  }

