// std::__partial_sort(pair<long, CMusicManager::CRegisteredMusicEntry>* first, pair<..>* middle, pair<..>* last, pair<..>*, CKeyPairCompareLess comp) -- STLport partial sort of the registered-music map: heap the prefix, sift in every smaller ID from the tail, sort the heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BF340, 183 bytes, `ret 0Ch`.
// Callers: the registered-music introsort depth-limit fallback (`partial_sort(first, last, last, comp)`).
// Callees: __make_heap @ 0x006BEE80, CWideString::CWideString(const CWideString&) @ 0x0099B720 (the by-value copy's
//   strings), __pop_heap @ 0x006BEDC0, sort_heap @ 0x006BF0E0 (landed).
// Types (Ego_r.pdb): pair<long, CMusicManager::CRegisteredMusicEntry> (24 bytes: key +0, SrcFileName +4, DstFileName +8,
//   Gain +0xC, Offset +0x10, CacheToHD +0x14) and the empty CKeyPairCompareLess<long, CRegisteredMusicEntry> (signed).
// Shape: STLport 4.x `__partial_sort(first, middle, last, _Tp*, comp)` from a /Gr TU (`__fastcall`: first in ecx,
//   middle in edx, last + the null value-type pointer + the comparator slot on the stack, `ret 0Ch`): `make_heap(first,
//   middle, comp)`; `for (i = middle; i < last; ++i) if (comp(*i, *first)) __pop_heap(first, middle, i, _Tp(*i), comp,
//   (int*)0);` then `sort_heap(first, middle, comp)`. With every register taken VC7.1 turns the pointer loop into a
//   trip count `(last - middle - 1) / 24 + 1` kept in the dead `last` argument slot, first in the `push ecx` word.
//   Same source as the CFaction twin 0x006BADB0.
// Label correction: the catalog `Heap_SiftDownMusic` is descriptive.
class CWideString {
public:
    void* PStringData;                                  // +0x00
    CWideString(const CWideString& other);              // 0x0099B720
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    ~CWideString();                                     // 0x0099B510
};

class CMusicManager {
public:
    class CRegisteredMusicEntry {                       // 20 bytes
    public:
        CWideString SrcFileName;                        // +0x00
        CWideString DstFileName;                        // +0x04
        float       Gain;                               // +0x08
        float       Offset;                             // +0x0C
        bool        CacheToHD;                          // +0x10
        CRegisteredMusicEntry(const CRegisteredMusicEntry& o)
            : SrcFileName(o.SrcFileName), DstFileName(o.DstFileName), Gain(o.Gain), Offset(o.Offset), CacheToHD(o.CacheToHD) {}
    };
};

namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const pair& o) : first(o.first), second(o.second) { }
};
}

typedef std::pair<long, CMusicManager::CRegisteredMusicEntry> CMusicEntryPair;

template <class K, class V>
struct CKeyPairCompareLess {                            // empty, passed by value
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
template <class RandomAccessIter, class Compare, class T, class Distance>
void __fastcall __make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp, T*, Distance*);            // 0x006BEE80

template <class RandomAccessIter, class T, class Compare, class Distance>
void __fastcall __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, T value,
                           Compare comp, Distance*);                                                                // 0x006BEDC0

template <class RandomAccessIter, class Compare>
void __fastcall sort_heap(RandomAccessIter first, RandomAccessIter last, Compare comp);                             // 0x006BF0E0

template <class RandomAccessIter, class Compare>
inline void make_heap(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    __make_heap(first, last, comp, (CMusicEntryPair*)0, (int*)0);
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

template void __fastcall __partial_sort<CMusicEntryPair*, CMusicEntryPair, CKeyPairCompareLess<long, CMusicManager::CRegisteredMusicEntry> >(
    CMusicEntryPair*, CMusicEntryPair*, CMusicEntryPair*, CMusicEntryPair*, CKeyPairCompareLess<long, CMusicManager::CRegisteredMusicEntry>);
}