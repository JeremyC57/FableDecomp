// std::__uninitialized_copy(P* first, P* last, P* result, const __false_type&) -- STLport: copy-construct each
//   pair<unsigned long, CCountedPointer<CVertexBufferWin32> > into raw storage (placement new, so a null destination is skipped; the counted
//   pointer bumps Info->RefCount), return the end of the copy
// Leaf: ??$__uninitialized_copy@PAU?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@std@@PAU12@@std@@YIPAU?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@0@PAU10@00ABU__false_type@0@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B8EDE0, 56 bytes, __fastcall (first ecx, last edx), `ret 8`.
// Identity: the row was catalogued "std::_Uninit_copy<... pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > ...>"
//   (Dinkumware spelling, wrong element). Its callers are the landed vector<pair<unsigned long, CCountedPointer<CVertexBufferWin32> > >::_M_insert_overflow
//   0x00B8F840 (head and tail copies) only; Ego_r has vector<pair<unsigned long, CCountedPointer<CVertexBufferWin32> > > (Ego_r allocator type). Family pass 2 (agent951 G055) agrees; retail has no
//   ICF, so each of the seven byte-identical bodies is its own instantiation.
// Model: the caller's own model (landed 0x00B8F840): CCountedPointer's copy constructor, STLport pair (a struct), `_Construct`
//   by placement new, `__uninitialized_copy` as a __fastcall template.
extern "C" void* __cdecl malloc(unsigned int size);
inline void* operator new(unsigned int, void* p) { return p; }
#include "engine/CCountedPointer.h"
class CVertexBufferWin32;
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
typedef pair<unsigned long, CCountedPointer<CVertexBufferWin32> > _Pair;
template _Pair* __fastcall __uninitialized_copy<_Pair*, _Pair*>(_Pair*, _Pair*, _Pair*, const __false_type&);
}
