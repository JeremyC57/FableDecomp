// std::__unguarded_insertion_sort_aux(CActiveScriptPair* first, CActiveScriptPair* last, CActiveScriptPair*, CActiveScriptLess comp) -- insertion-sort the tail of an introsorted (UID, active entity script) range without bound checks
// Leaf: ??$__unguarded_insertion_sort_aux@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBB270, 70 bytes (`ret 8`).
// TU: the scripting library's STLport 4.x instantiations (0x00CB6xxx-0x00CBDxxx); the game is built /Gr, so the
//   algorithm templates are __fastcall (first two pointer arguments in ecx/edx, callee pops the rest).
// Identity: STLport __unguarded_insertion_sort_aux over (UID, active entity script) = std::pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > (0x10 bytes);
//   callers 0x00CBB857, 0x00CBD3FC, 0x00CBDF49 (final insertion sorts). The manifest label
//   "_Copy_impl<CNavigationPosition*,...>" is propagated and wrong.
// Callees: __unguarded_linear_insert 0x00CBA840.
// Shape: the by-value 16-byte temporary is copy-built in the argument slot (inline counted-pointer copy, `inc [ecx]`).
//   CCountedPointer is modelled with the release form the D rows need (decrement, re-read, test, then Data = Info = 0):
//   VC7.1 emits the same bytes for this row as with the short `if (Info && --Info->RefCount == 0)` form.
void __cdecl operator delete(void* block);                 // 0x00BFE9BC
#include "engine/CCountedPointer.h"
class CActiveEntityScriptBase;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess {    // empty, passed by value (4-byte slot)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef std::pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > CActiveScriptPair;   // 16 bytes
typedef CKeyPairCompareLess<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > CActiveScriptLess;
namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00CBA840

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
template void __fastcall __unguarded_insertion_sort_aux<CActiveScriptPair*, CActiveScriptPair, CActiveScriptLess>(CActiveScriptPair*, CActiveScriptPair*, CActiveScriptPair*, CActiveScriptLess);
}