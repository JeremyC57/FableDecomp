// std::__partial_sort(pair<unsigned long, CCountedPointer<CFaction> >* first, pair<..>* middle, pair<..>* last, pair<..>*, CKeyPairCompareLess comp) -- STLport partial sort of the faction vector map: heap the prefix, sift in every smaller key from the tail, sort the heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BADB0, 102 bytes (+ int3 padding), `ret 0Ch`.
// Callers: 0x006BAF9C and 0x006BB12E (the introsort depth-limit fallback `partial_sort(first, last, last, comp)`).
// Callees: __make_heap @ 0x006BA230 (catalog "Heap_BuildHeap"), __pop_heap @ 0x006BA180 (`ret 18h`), sort_heap
//   @ 0x006BA830 (landed).
// Types: pair<unsigned long, CCountedPointer<CFaction> > (12 bytes: key +0, Data +4, Info +8) and the empty key-only
//   CKeyPairCompareLess, as the landed sort_heap 0x006BA830.
// Shape: STLport 4.x `__partial_sort(first, middle, last, _Tp*, comp)` from a /Gr TU (`__fastcall`: first in ecx,
//   middle in edx, last + the null value-type pointer + the comparator slot on the stack, `ret 0Ch`): `make_heap(first,
//   middle, comp)` (the inline front end: `__make_heap(first, middle, comp, (_Tp*)0, (int*)0)`); `for (i = middle; i <
//   last; ++i) if (comp(*i, *first)) __pop_heap(first, middle, i, _Tp(*i), comp, (int*)0);` -- the value copy built in
//   the argument area with the counted pointer's add-ref -- then `sort_heap(first, middle, comp)`.
// Label correction: the catalog `Heap_SiftDown` is descriptive.
struct CCPPointerInfo {                                 // 12 bytes (Ego_r.pdb)
    unsigned long RefCount;                             // +0x00
    void (__fastcall* DeleteFunc)(void*);               // +0x04
    void*         Data;                                 // +0x08
};

class CFaction;

template <class T>
struct CCountedPointer {                                // 8 bytes
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
};

namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const pair& o) : first(o.first), second(o.second) { }
};
}

typedef std::pair<unsigned long, CCountedPointer<CFaction> > CFactionEntry;

template <class K, class V>
struct CKeyPairCompareLess {                            // empty, passed by value
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
template <class RandomAccessIter, class Compare, class T, class Distance>
void __fastcall __make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp, T*, Distance*);            // 0x006BA230

template <class RandomAccessIter, class T, class Compare, class Distance>
void __fastcall __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, T value,
                           Compare comp, Distance*);                                                                // 0x006BA180

template <class RandomAccessIter, class Compare>
void __fastcall sort_heap(RandomAccessIter first, RandomAccessIter last, Compare comp);                             // 0x006BA830

template <class RandomAccessIter, class Compare>
inline void make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    __make_heap(first, last, comp, (CFactionEntry*)0, (int*)0);
}

template <class RandomAccessIter, class T, class Compare>
void __fastcall __partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, T*, Compare comp)
{
    make_heap(first, middle, comp);
    for (RandomAccessIter i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, T(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}

template void __fastcall __partial_sort<CFactionEntry*, CFactionEntry, CKeyPairCompareLess<unsigned long, CCountedPointer<CFaction> > >(
    CFactionEntry*, CFactionEntry*, CFactionEntry*, CFactionEntry*, CKeyPairCompareLess<unsigned long, CCountedPointer<CFaction> >);
}