// std::__introsort_loop(pair<unsigned long, CSoundPair>* first, pair<unsigned long, CSoundPair>* last, pair<unsigned long, CSoundPair>*, int depth_limit, CKeyPairCompareLess<unsigned long, CSoundPair> comp) -- STLport introsort: partition around a median-of-three until 16 elements remain, falling back to partial_sort when the depth budget runs out
// Leaf: __introsort_loop
// Retail: Fable.exe (TLC, VC7.1) @ 0x00470E9E, 148 bytes, `ret 0Ch` (__fastcall: first in ecx, last in edx; the value-type
//   pointer, depth_limit and the 1-byte comparator on the stack).
// Identity: the catalog's "Quicksort_Recursive" is behavioural. Called by sort() @ 0x00470CF1 (depth 2 * lg(n)) from
//   CVectorMap<unsigned long, CSoundPair, CKeyPairCompareLess<..> >::Sort @ 0x00470CD8 (comparator byte +0xc, clears the
//   Dirty byte +0xd), which CSoundMap::TransferBinaryIn @ 0x0047042A calls: the element is CSoundMap::SoundMap's
//   pair<unsigned long, CSoundPair> (12 bytes, unsigned key compare).
// Callees: __median @ 0x00470F32, __unguarded_partition @ 0x00470F5F, __partial_sort @ 0x00470FA5 (partial_sort inlined),
//   itself (recursion on the right part).
// Model: STLport 4.x `__introsort_loop` (`__stl_threshold` 16); the pivot `_Tp(__median(..))` is built straight into the
//   by-value argument area with pair's member-wise copy constructor; the comparator push is shared by the partial_sort
//   and partition branches (retail pushes it before the `depth_limit == 0` test).
// Shape: `push 0Ch ; pop ecx` = size TU with a frame pointer (`optimize("y", off)` + `("s", on)`); __median stays out of line.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CSoundPair {
    long FirstSound;                                // +0x00
    long LastSound;                                 // +0x04
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;                                       // +0x00
    T2 second;                                      // +0x04
    __forceinline pair(const pair<T1, T2>& other) : first(other.first), second(other.second) { }
};
}

template <class Key, class Value> struct CKeyPairCompareLess {
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

typedef pair<unsigned long, CSoundPair> _SoundEntry;
template void __fastcall __introsort_loop(_SoundEntry* first, _SoundEntry* last, _SoundEntry*, int depth_limit,
                                          CKeyPairCompareLess<unsigned long, CSoundPair> comp);
}