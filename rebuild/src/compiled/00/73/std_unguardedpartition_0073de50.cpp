// std::__unguarded_partition(std::pair<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo >* first, std::pair<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo >* last, std::pair<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo > pivot, CKeyPairCompareLess<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo > comp) -- STLport Hoare partition: advance first past elements below pivot, retreat last past elements above it, swap and continue until they cross; return first
// Leaf: ??$__unguarded_partition@PAU?$pair@KVCRegisteredModeInfo@CTCCreatureModeManager@@@std@@U12@V?$CKeyPairCompareLess@KVCRegisteredModeInfo@CTCCreatureModeManager@@@@@std@@YIPAU?$pair@KVCRegisteredModeInfo@CTCCreatureModeManager@@@0@PAU10@0U10@V?$CKeyPairCompareLess@KVCRegisteredModeInfo@CTCCreatureModeManager@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073de50, 85 bytes, /Gr __fastcall (first in ecx, last in edx; pivot, comp on the stack), `ret 0Ch`.
// Identity (proven): bound (reloc_bindings) as the non-template ?__unguarded_partition@std@@YIPAU?$pair@KVCRegisteredModeInfo@CTCCreatureModeManager@@@1@PAU21@0U21@V?$CKeyPairCompareLess@...@@@Z; its only caller is the landed __introsort_loop 0x00743EC0 of that pair (family L053's heap helpers serve the same sort).
// Callees: none (the comparator is inline: an unsigned compare of the first dword).
// Shape (family L054): STLport 4.6 _algo.c __unguarded_partition verbatim with the inline iter_swap (two-dword swap
//   through ebx/esi/edi); the pivot's key stays in ecx.
class CModeBase;
class CThing;
class CTCCreatureModeManager {
public:
    class CRegisteredModeInfo {                            // Ego_r.pdb, 4 bytes
    public:
        CModeBase* (__cdecl* AllocFunc)(CThing&);         // +0x00
    };
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;                                           // +0x04
};
}
template <class K, class V> class CKeyPairCompareLess {    // empty, passed by value
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
namespace std {
template <class _Tp> inline void swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}
template <class _ForwardIter1, class _ForwardIter2> inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    swap(*__i1, *__i2);
}
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}
}
template std::pair<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo >* __fastcall std::__unguarded_partition(std::pair<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo >*, std::pair<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo >*, std::pair<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo >, CKeyPairCompareLess<unsigned long, CTCCreatureModeManager::CRegisteredModeInfo >);