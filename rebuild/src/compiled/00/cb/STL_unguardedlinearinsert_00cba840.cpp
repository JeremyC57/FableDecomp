// std::__unguarded_linear_insert(CActiveScriptPair* last, CActiveScriptPair val, CActiveScriptLess comp) -- shift every earlier (UID, active entity script) pair with a larger UID up one slot and drop val into the gap
// Leaf: ??$__unguarded_linear_insert@
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBA840, 150 bytes (`ret 14h`: by-value 16-byte val + comparator slot).
// TU: the scripting library's STLport 4.x instantiations (0x00CB6xxx-0x00CBDxxx); the game is built /Gr, so the
//   algorithm templates are __fastcall (first two pointer arguments in ecx/edx, callee pops the rest).
// Identity: STLport __unguarded_linear_insert over (UID, active entity script) = std::pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > (0x10 bytes);
//   callers __unguarded_insertion_sort_aux 0x00CBB2A4 and __linear_insert 0x00CBC390. The manifest label
//   "SpeechFrame_InsertSorted" is propagated and wrong.
// Callees: pair operator= 0x00CBA520 (VC7.1 emits the implicit pair assignment out of line: UID copy plus the inline
//   CCountedPointer::operator=), operator delete 0x00BFE9BC (val's release; the count block's delete function via +4).
// Shape: `*__last = *__next` in the loop; the u64 compare is high dword first; val's destructor releases at the end.
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
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}
template void __fastcall __unguarded_linear_insert<CActiveScriptPair*, CActiveScriptPair, CActiveScriptLess>(CActiveScriptPair*, CActiveScriptPair, CActiveScriptLess);
}