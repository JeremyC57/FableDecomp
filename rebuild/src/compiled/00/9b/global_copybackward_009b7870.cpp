// _global::__copy_backward(PairULongDefClassInfo* first, PairULongDefClassInfo* last, PairULongDefClassInfo* result, const random_access_iterator_tag&, long*) -- SGI random-access copy_backward over (unsigned long, CDefinitionManager::CDefClassInfo) pairs: assign each element from the back, return the new start of the destination
// Leaf: __copy_backward
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B7870, length 115 bytes, `ret 0Ch` (result, the iterator-tag reference and the
//   distance-type pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr). The row's
//   `_Move_backward<pair<unsigned long,CDefClassInfo>*,...>` label is right on the element type; the function is STLport's
//   __copy_backward (copy_backward / the vector insert path), which has no _Move_backward.
// Layout: the element is 24 bytes (`imul 0x2AAAAAAB ; sar edx,2`): pair = { unsigned long first +0 ; CDefClassInfo second +4 },
//   CDefinitionManager::CDefClassInfo (Ego_r.pdb) = { CCharString Name +0 ; AllocFunc +4 ; GlobalIndices +8 (12 bytes) }.
// Callees: CCharString::operator= @ 0x0099EFB0, GlobalIndices' container operator= @ 0x0045BC09 (an out-of-line STLport
//   vector<unsigned long>::operator=; thiscall, `ret 4`).
// Shape: STLport
//     for (_Distance __n = __last - __first; __n > 0; --__n) *--__result = *--__last; return __result;
//   with the pair's and CDefClassInfo's implicit assignments inlined member by member (key word, Name by call, AllocFunc word,
//   GlobalIndices by call); the trip count lives in the dead `result` argument slot.
// Name fix (NF387, T3 nf383 0x009B7870 pilot): GlobalIndices' assignment binds STLport's
//   ??4?$vector@KV?$allocator@K@std@@@std@@QAEAAV01@ABV01@@Z at 0x0045BC09 (14 vector-context FableWin callers, NF383 R2) through the shared
//   CArray model; was the body-less local CArray<unsigned long>::operator=.
class CCharString {
public:
    void* PStringData;
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

// CArray<T> is the shared engine/CArray.h model (Lionhead's CArray<T> : public STLport vector<T>, no fields of its own; the
// assignment is the generated one). FABLE_RETAIL_STLP_VECTOR_ASSIGN declares STLport's vector::operator= out of line, as retail
// calls it (vector<unsigned long>::operator= 0x0045BC09), so the generated CArray / CDefClassInfo assignments expand to that call.
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "engine/CArray.h"

class CBaseObject;
typedef CBaseObject* (*TDefAllocFunc)();

namespace CDefinitionManager {
class CDefClassInfo { public:                                  // Ego_r.pdb (24 B there: its CArray is 16); retail 20 bytes
    CCharString           Name;                         // +0x00
    TDefAllocFunc         AllocFunc;                    // +0x04
    CArray<unsigned long> GlobalIndices;                // +0x08
};
}

struct PairULongDefClassInfo {                          // std::pair<unsigned long, CDefinitionManager::CDefClassInfo>, 24 bytes
    unsigned long                     first;            // +0x00
    CDefinitionManager::CDefClassInfo second;           // +0x04
};

struct random_access_iterator_tag { };

PairULongDefClassInfo* __fastcall __copy_backward(PairULongDefClassInfo* first, PairULongDefClassInfo* last,
                                                   PairULongDefClassInfo* result, const random_access_iterator_tag&, long*)
{
    for (long n = long(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}