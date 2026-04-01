#include "..\include\RelativeIndex.h"


    RelativeIndex::RelativeIndex(size_t doc_id, float rank)
    {
        this->doc_id=doc_id;
        this->rank=rank;
    }

    bool RelativeIndex::operator == (const RelativeIndex &other) const
    {
        return (doc_id==other.doc_id && rank==other.rank);
    }


