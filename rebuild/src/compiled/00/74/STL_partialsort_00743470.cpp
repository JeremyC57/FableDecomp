// _STL::__partial_sort(pair<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo>* first, pair<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo>* middle, pair<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo>* last, pair<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo>*, CKeyPairCompareLess comp) -- heap-select the smallest keys into [first, middle) and sort them
// Leaf: __partial_sort
// Retail: Fable.exe (TLC, VC7.1) @ 0x00743470, 107 bytes, `ret 0Ch` (link-time __fastcall: first in ecx, middle in edx,
//   then last, the value-type tag pointer and the empty comparator on the stack).
// Callees: __make_heap(first, middle, comp, 0, 0) @ 0x0073DF30, __adjust_heap @ 0x0073D7C0 (landed; the inlined
//   __pop_heap), sort_heap @ 0x007423D0 (landed, `ret 4`).
// Caller: __introsort_loop 0x00743EC0 (its depth-limit fallback `partial_sort(first, last, last, comp)`).
// Label: the manifest's "PriorityQueue_Heap_Insert" is descriptive; the body is STLport `__partial_sort`.
// Element: the 8-byte pair of the CTCCreatureModeManager mode registry (CVectorMap<unsigned long, CRegisteredModeInfo>).
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, T(*i), comp, DISTANCE_TYPE(first)); sort_heap(first, middle, comp);` -- the
//   comparator dword stays in ebp, `last` is re-read from its slot; in the inlined __pop_heap the value (already in
//   edx/ecx from the compare) is built in the argument slot after `*i = *first`, the length is (middle - first) >> 3.
class CThing;
class CTCCreatureModeManager {
public:
    class CModeBase;
    struct CRegisteredModeInfo {                         // Ego_r.pdb 4 bytes
        CModeBase* (__cdecl* AllocFunc)(CThing*);
    };
};

namespace _STL {
template <class A, class B> struct pair {
    A first;                                             // +0x00
    B second;                                            // +0x04
    pair(const pair& other) : first(other.first), second(other.second) {}
};
}

template <class K, class V> class CKeyPairCompareLess {  // empty, passed by value
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

typedef _STL::pair<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo> TModePair;
typedef CKeyPairCompareLess<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo> TModeLess;

namespace _STL {
void __fastcall __make_heap(TModePair* first, TModePair* last, TModeLess comp, TModePair*, int*);              // 0x0073DF30
void __fastcall __adjust_heap(TModePair* first, int holeIndex, int len, TModePair val, TModeLess comp);       // 0x0073D7C0
void __fastcall sort_heap(TModePair* first, TModePair* last, TModeLess comp);                                 // 0x007423D0

inline void make_heap(TModePair* first, TModePair* last, TModeLess comp)
{
    __make_heap(first, last, comp, (TModePair*)0, (int*)0);
}

template <class RandomAccessIter, class T, class Compare, class Distance>
inline void __pop_heap(RandomAccessIter first, RandomAccessIter last, RandomAccessIter result, T val, Compare comp, Distance*)
{
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last - first), val, comp);
}

void __fastcall __partial_sort(TModePair* first, TModePair* middle, TModePair* last, TModePair*, TModeLess comp)
{
    make_heap(first, middle, comp);
    for (TModePair* i = middle; i < last; ++i)
        if (comp(*i, *first))
            __pop_heap(first, middle, i, TModePair(*i), comp, (int*)0);
    sort_heap(first, middle, comp);
}
}