// std::__unguarded_partition(CActiveScriptPair* first, CActiveScriptPair* last, CActiveScriptPair pivot, CActiveScriptLess comp) -- STLport Hoare partition of the (UID, active entity script) pairs around the pivot key, swapping out-of-place pairs in place; returns the split point
// Leaf: ??$__unguarded_partition@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBB710, 315 bytes, __fastcall (first ecx, last edx; the 16-byte pivot by value
//   and the empty 4-byte comparator on the stack, `ret 14h`).
// Identity (two sources): (1) reloc_bindings.tsv binds 0x00CBB710 as
//   ??$__unguarded_partition@PAU?$pair@_KV?$CCountedPointer@VCActiveEntityScriptBase@@@@@std@@U12@V?$CKeyPairCompareLess@...
//   from its only caller, the landed std::__introsort_loop 0x00CBD700 (six call sites, the pivot built in the argument
//   slot); (2) the body is STLport 4.6 _algo.c `__unguarded_partition` verbatim over that 16-byte pair: the unsigned
//   __int64 key compared high word first (ja/jb, then the low word jae), the CCountedPointer's count block bumped on the
//   iter_swap temporary's copy and released after. Same TU and model as the landed siblings __unguarded_partition
//   0x00CBB860 and std::swap 0x00CBA7E0. The manifest label "SortedMerge_WithRefcount" is descriptive and wrong.
// Callees: operator delete 0x00BFE9BC (the counted-pointer releases); the iter_swap / swap / pair operator= chain is
//   inlined here (unlike 0x00CBB860, which calls swap 0x00CBA8E0).
// Shape: STLport text; `iter_swap` and `swap` inline, the pair's implicit operator= inlined twice (the key's two words,
//   then CCountedPointer::operator= with data/info read first); the pivot's destructor at the single return.
void __cdecl operator delete(void* block);                 // 0x00BFE9BC
#include "engine/CCountedPointer.h"
class CActiveEntityScriptBase;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
typedef std::pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > CActiveScriptPair;   // 16 bytes
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef CKeyPairCompareLess<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > CActiveScriptLess;
namespace std {
template <class _Tp>
inline void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    swap(*__i1, *__i2);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
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
template CActiveScriptPair* __fastcall __unguarded_partition<CActiveScriptPair*, CActiveScriptPair, CActiveScriptLess>(CActiveScriptPair*, CActiveScriptPair*, CActiveScriptPair, CActiveScriptLess);
}