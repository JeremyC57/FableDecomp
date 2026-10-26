// std::__adjust_heap(pair<unsigned long, CDefString>* first, int holeIndex, int len, pair<unsigned long, CDefString> value, CKeyPairCompareLess<unsigned long, CDefString> comp) -- STLport heap sift-down of a sound-map criteria entry by key, then the inlined __push_heap back up
// Leaf: __adjust_heap
// Retail: Fable.exe (TLC, VC7.1) @ 0x004714AE, 130 bytes, `ret 10h` (__fastcall: first in ecx, holeIndex in edx; len, the
//   8-byte value and the 1-byte comparator on the stack).
// Identity: the catalog's "Heap_AdjustDown" is behavioural. Same instantiation as __introsort_loop @ 0x00471338 (the
//   CSoundMap::CriteriaMap sort, element pair<unsigned long, CDefString>, unsigned key compare); callers 0x00471473,
//   0x00471564, 0x004715A1 are its make_heap / pop_heap users. The 12-byte twin is 0x00471055 (SoundMap).
// Model: STLport 4.x `__adjust_heap` (secondChild = 2 * (secondChild + 1)) with `__push_heap(first, holeIndex, topIndex,
//   value, comp)` inlined; the by-value element stays in ebx/edi, element assignments are two dword moves.
// Shape: size TU with a frame pointer (`optimize("y", off)` + `("s", on)`).
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
    __forceinline bool operator()(const std::pair<Key, Value>& a, const std::pair<Key, Value>& b) const { return a.first < b.first; }
};

namespace std {
template <class RandomAccessIter, class Distance, class T, class Compare>
__forceinline void __push_heap(RandomAccessIter first, Distance holeIndex, Distance topIndex, T value, Compare comp)
{
    Distance parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), value)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

template <class RandomAccessIter, class Distance, class T, class Compare>
void __fastcall __adjust_heap(RandomAccessIter first, Distance holeIndex, Distance len, T value, Compare comp)
{
    Distance topIndex = holeIndex;
    Distance secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, value, comp);
}

typedef pair<unsigned long, CDefString> _CriteriaEntry;
template void __fastcall __adjust_heap(_CriteriaEntry* first, int holeIndex, int len, _CriteriaEntry value,
                                       CKeyPairCompareLess<unsigned long, CDefString> comp);
}