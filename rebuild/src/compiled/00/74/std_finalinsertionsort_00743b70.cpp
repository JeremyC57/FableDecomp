// std::__final_insertion_sort(pair<unsigned long,CTCCreatureModeManager::CRegisteredModeInfo>*, pair<...>*, CKeyPairCompareLess comp) -- insertion-sort the first 16 mode-registry pairs, then unguarded-insert the rest
// Leaf: ??$__final_insertion_sort@PAU?$pair@KVCRegisteredModeInfo@CTCCreatureModeManager@@@std@@V?$CKeyPairCompareLess@KVCRegisteredModeInfo@CTCCreatureModeManager@@@@@std@@YIXPAU?$pair@KVCRegisteredModeInfo@CTCCreatureModeManager@@@0@0V?$CKeyPairCompareLess@KVCRegisteredModeInfo@CTCCreatureModeManager@@@@@Z
// Retail: Fable.exe @ 0x00743B70, 71 bytes, `ret 4` (link-time __fastcall: first in ecx, last in edx, the empty
//   comparator dword on the stack).
// Identity (probable): the STLport 4.6 `__final_insertion_sort` body (threshold __stl_threshold = 16 elements of 8 bytes:
//   `(last - first) > 16` folds to `((last - first) & ~7) > 0x80`), over the same 8-byte element its two callees work on:
//   __insertion_sort 0x00743430 (landed as InsertionSort_SElement8, a descriptive stand-in) and
//   __unguarded_insertion_sort_aux(first, last, T*, comp) 0x0073D720 (landed: pair<unsigned long,
//   CTCCreatureModeManager::CRegisteredModeInfo> with CKeyPairCompareLess) -- the inlined __unguarded_insertion_sort
//   passes the null value-type tag (`push 0`).
// Element: the CTCCreatureModeManager mode registry (CVectorMap<unsigned long, CRegisteredModeInfo>), as landed
//   __partial_sort 0x00743470 / __unguarded_insertion_sort_aux 0x0073D720.
class CThing;
class CTCCreatureModeManager {
public:
    class CModeBase;
    class CRegisteredModeInfo { public:                         // Ego_r.pdb 4 bytes
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
void __fastcall __insertion_sort(TModePair* first, TModePair* last, TModeLess comp);                          // 0x00743430
void __fastcall __unguarded_insertion_sort_aux(TModePair* first, TModePair* last, TModePair*, TModeLess comp); // 0x0073D720

inline void __unguarded_insertion_sort(TModePair* first, TModePair* last, TModeLess comp)
{
    __unguarded_insertion_sort_aux(first, last, (TModePair*)0, comp);
}

template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__last - __first > 16) {
        __insertion_sort(__first, __first + 16, __comp);
        __unguarded_insertion_sort(__first + 16, __last, __comp);
    }
    else
        __insertion_sort(__first, __last, __comp);
}
template void __fastcall __final_insertion_sort(TModePair*, TModePair*, TModeLess);
}