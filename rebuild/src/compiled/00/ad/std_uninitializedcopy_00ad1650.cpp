// std::__uninitialized_copy<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>*, same>(first, last, result, const __false_type&) -- copy-construct counted pointers into raw storage, return the end
// Leaf: ??$__uninitialized_copy@PAV?$CCountedPointer@VCOptimisedPrimitive@QuadricOptimiserInternals@@@@PAV1@@std@@YIPAV?$CCountedPointer@VCOptimisedPrimitive@QuadricOptimiserInternals@@@@PAV1@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD1650, 50 bytes, __fastcall (first ecx, last edx, result and the tag on the stack), `ret 8`; the counted-pointer copy ctor inline.
// agentNF57 (q10490): named as the STLport instance the landed binders bind (descriptive-by-library: retail is STLport 4.6, both PDBs
//   are Dinkumware builds): vector<CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> >::_M_insert_overflow 0x00AD68D0 calls
//   0x00AD1650 (head/tail copy), 0x00AD1690 (fill_len != 1) and 0x00AD37E0 (_M_clear) under exactly these spellings, and
//   std::__destroy_aux<COptimisedSubMesh*> 0x00AD7DC0 calls 0x00AD37E0 for each sub-mesh's Primitives. The element is Ego_r.pdb's
//   CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> (COptimisedSubMesh::Primitives); the template bodies are the ones
//   the landed 0x00AD68D0 defines, on the shared engine/CCountedPointer.h. Was placeholder pointee CUnprovenPointee_00ad68d0 and a local CCountedPointer / CCPPointerInfo model.
extern "C" void* __cdecl malloc(unsigned int size);
inline void* operator new(unsigned int, void* p) { return p; }

#include "engine/CCountedPointer.h"

namespace QuadricOptimiserInternals { class COptimisedPrimitive; }   // Ego_r.pdb: COptimisedSubMesh::Primitives element

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

}  // namespace std

typedef ::CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive> Element;
template Element* __fastcall std::__uninitialized_copy<Element*, Element*>(Element*, Element*, Element*, const std::__false_type&);
