// std::__linear_insert(CActiveScriptPair* first, CActiveScriptPair* last, CActiveScriptPair val, CActiveScriptLess comp) -- insert val at last into the sorted (UID, active entity script) run [first, last): to the front (after shifting the run up) when it precedes *first, else by the unguarded backward walk
// Leaf: ??$__linear_insert@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBC330, 129 bytes (`ret 14h`).
// TU: the scripting library's STLport 4.x instantiations (0x00CB6xxx-0x00CBDxxx); the game is built /Gr, so the
//   algorithm templates are __fastcall (first two pointer arguments in ecx/edx, callee pops the rest).
// Identity: STLport __linear_insert over (UID, active entity script) = std::pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > (0x10 bytes); caller __insertion_sort
//   0x00CBCF5D. The manifest label "Item_InsertInPriorityOrder" is propagated and wrong.
// Callees: __copy_backward 0x00CBADA0, __unguarded_linear_insert 0x00CBA840, operator delete 0x00BFE9BC.
//   Pair operator= 0x00CBA520 (`*__first = __val`).
// Shape: as 0x00CBB920; val's D-form release closes the function.
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
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                              const random_access_iterator_tag&, _Distance*);   // 0x00CBADA0
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00CBA840

template <class _BidirectionalIter1, class _BidirectionalIter2>
__forceinline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}
template void __fastcall __linear_insert<CActiveScriptPair*, CActiveScriptPair, CActiveScriptLess>(CActiveScriptPair*, CActiveScriptPair*, CActiveScriptPair, CActiveScriptLess);
}