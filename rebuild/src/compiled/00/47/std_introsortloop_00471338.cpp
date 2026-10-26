// std::__introsort_loop(pair<unsigned long, CDefString>* first, pair<unsigned long, CDefString>* last, pair<unsigned long, CDefString>*, int depth_limit, CKeyPairCompareLess<unsigned long, CDefString> comp) -- STLport introsort of the sound-map criteria entries by key: partition around a median-of-three until 16 elements remain, partial_sort when the depth budget runs out
// Leaf: __introsort_loop
// Retail: Fable.exe (TLC, VC7.1) @ 0x00471338, 141 bytes, `ret 0Ch` (__fastcall: first in ecx, last in edx; the value-type
//   pointer, depth_limit and the 1-byte comparator on the stack).
// Identity: the catalog's "Quicksort_Recursive_8byte" is behavioural. Called by sort() @ 0x004711B3 from
//   CVectorMap<unsigned long, CDefString, CKeyPairCompareLess<..> >::Sort @ 0x0047119A (comparator byte +0xc, clears the
//   Dirty byte +0xd), which CSoundMap::TransferBinaryIn @ 0x0047042A calls for its second map: the element is
//   CSoundMap::CriteriaMap's pair<unsigned long, CDefString> (8 bytes, unsigned key compare). The 12-byte twin is the
//   SoundMap instantiation @ 0x00470E9E (same source with CSoundPair).
// Callees: __median @ 0x004713C5, __unguarded_partition @ 0x004713F2, __partial_sort @ 0x0047142B (partial_sort inlined),
//   itself (recursion on the right part).
// Model: STLport 4.x `__introsort_loop` (`__stl_threshold` 16: for 8-byte elements the size test is the byte distance
//   masked with ~7 against 0x80); the pivot is built in the by-value argument area with pair's member-wise copy constructor.
// Shape: size TU with a frame pointer (`optimize("y", off)` + `("s", on)`); __median stays out of line.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CDefString { public:
    long TablePos;                                  // +0x00 def-string table index
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;                                       // +0x00
    T2 second;                                      // +0x04
    __forceinline pair(const pair<T1, T2>& other) : first(other.first), second(other.second) { }
};
}

template <class Key, class Value> class CKeyPairCompareLess { public:
    bool operator()(const std::pair<Key, Value>& a, const std::pair<Key, Value>& b) const { return a.first < b.first; }
};

namespace std {
template <class T, class Compare>
const T& __fastcall __median(const T& a, const T& b, const T& c, Compare comp);                    // 0x00470F32
template <class RandomAccessIter, class T, class Compare>
RandomAccessIter __fastcall __unguarded_partition(RandomAccessIter first, RandomAccessIter last, T pivot, Compare comp);   // 0x00470F5F
template <class RandomAccessIter, class T, class Compare>
void __fastcall __partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, T*, Compare comp); // 0x00470FA5

template <class T> __forceinline T* __value_type(const T*) { return (T*)0; }   // _STLP_VALUE_TYPE for pointers

template <class RandomAccessIter, class Compare>
__forceinline void partial_sort(RandomAccessIter first, RandomAccessIter middle, RandomAccessIter last, Compare comp)
{
    __partial_sort(first, middle, last, __value_type(first), comp);
}

template <class RandomAccessIter, class T, class Size, class Compare>
void __fastcall __introsort_loop(RandomAccessIter first, RandomAccessIter last, T*, Size depth_limit, Compare comp)
{
    while (last - first > 16) {
        if (depth_limit == 0) {
            partial_sort(first, last, last, comp);
            return;
        }
        --depth_limit;
        RandomAccessIter cut = __unguarded_partition(first, last,
                                                     T(__median(*first, *(first + (last - first) / 2), *(last - 1), comp)),
                                                     comp);
        __introsort_loop(cut, last, (T*)0, depth_limit, comp);
        last = cut;
    }
}

typedef pair<unsigned long, CDefString> _CriteriaEntry;
template void __fastcall __introsort_loop(_CriteriaEntry* first, _CriteriaEntry* last, _CriteriaEntry*, int depth_limit,
                                          CKeyPairCompareLess<unsigned long, CDefString> comp);
}