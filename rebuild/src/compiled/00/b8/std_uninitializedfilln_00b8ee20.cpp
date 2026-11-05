// std::__uninitialized_fill_n(pair<unsigned long, CCountedPointer<CVertexBufferWin32> >* first, unsigned int n, const pair<...>& x, const __false_type&) -- copy-construct x into n raw slots from first, return the end
// Leaf: ??$__uninitialized_fill_n@PAU?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@std@@IU12@@std@@YIPAU?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@0@PAU10@IABU10@ABU__false_type@0@@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00b8ee20, 52 bytes, /Gr `__fastcall` (first ecx, n edx, x and the tag on the stack), `ret 8`.
// Identity (proven): called only by the landed vector<pair<unsigned long, CCountedPointer<CVertexBufferWin32> > >::_M_insert_overflow 0x00B8F840 (proven there: CEngineOcclusionManager's FlareVertexBuffers) which binds it ??$__uninitialized_fill_n@PAU?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@std@@IU12@@std@@....
// Body: STLport 4.6 `for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;` -- placement new's null test, the key dword,
//   then CCountedPointer's copy constructor (Data, Info, `if (Info) ++Info->RefCount`).
// Shape (family L074): the fill_n body the landed _M_insert_overflow 0x00B8F840 keeps as a same-TU noinline helper.
class CVertexBufferWin32;
#include "engine/CCountedPointer.h"
inline void* operator new(unsigned int, void* __p) { return __p; }
namespace std {
struct __false_type { };
template <class _T1, class _T2> struct pair {
    _T1 first;                                            // +0x00
    _T2 second;                                           // +0x04
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // STLport 4.6's user copy constructor
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new ((void*)__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template pair<unsigned long, CCountedPointer<CVertexBufferWin32> >* __fastcall __uninitialized_fill_n(pair<unsigned long, CCountedPointer<CVertexBufferWin32> >*, unsigned int, const pair<unsigned long, CCountedPointer<CVertexBufferWin32> >&, const __false_type&);
}   // namespace std