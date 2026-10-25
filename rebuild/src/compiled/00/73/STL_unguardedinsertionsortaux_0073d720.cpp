// std::__unguarded_insertion_sort_aux(pair<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo>* first, pair<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo>* last, pair<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo>*, CKeyPairCompareLess comp) -- insertion-sort pass with no lower-bound test over the 8-byte mode-registry entries, each sifted down by its unsigned key
// Leaf: __unguarded_insertion_sort_aux
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073D720, 73 bytes (+7 int3), `ret 8` (link-time __fastcall: first in ecx, last in
//   edx, the value-type tag pointer and the empty comparator on the stack). No callees.
// Caller: the landed sort 0x007441D0 (its inlined __final_insertion_sort runs this over [first + 16, last)).
// Label: the manifest's `_Copy_impl<CNavigationPosition*,CNavigationPosition*>` is a propagation; the body is STLport's
//   unguarded insertion sort. Element: the 8-byte pair of CVectorMap<unsigned long, CTCCreatureModeManager::
//   CRegisteredModeInfo, CKeyPairCompareLess<..> > (the mode registry whose Count is 0x0073DA50 and UpperBound the landed
//   0x0073D250); the instance is pinned by the 8-byte stride and the UNSIGNED key compare, the second member is one dword.
// Shape: STLport `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp)` with the linear insert
//   inlined: the value's key in edi and its dword in ebx (the pair's memberwise user copy constructor), `next` in eax,
//   `last` in esi, the unsigned key compare `cmp edi,ebp ; jae/jb`.
class CThing;
class CTCCreatureModeManager {
public:
    class CModeBase;
    struct CRegisteredModeInfo {                         // Ego_r.pdb 4 bytes
        CModeBase* (__cdecl* AllocFunc)(CThing*);
    };
};

namespace std {
template <class A, class B> struct pair {
    A first;                                             // +0x00
    B second;                                            // +0x04
    pair(const pair& other) : first(other.first), second(other.second) {}
};
}

template <class K, class V> class CKeyPairCompareLess {  // empty, passed by value
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

typedef std::pair<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo> TModePair;
typedef CKeyPairCompareLess<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo> TModeLess;

namespace std {
template <class RandomAccessIter, class T, class Compare>
inline void __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp)
{
    RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

void __fastcall __unguarded_insertion_sort_aux(TModePair* first, TModePair* last, TModePair*, TModeLess comp)
{
    for (TModePair* i = first; i != last; ++i)
        __unguarded_linear_insert(i, TModePair(*i), comp);
}
}