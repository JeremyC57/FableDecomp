// std::__uninitialized_copy(const C3DGroup2* first, const C3DGroup2* last, C3DGroup2* result, const __false_type&) -- STLport placement copy-construct each record (the scalar, then the member's out-of-line copy constructor) and return the advanced result
// Leaf: ??$__uninitialized_copy@PBVC3DGroup2@@PAV1@@std@@YIPAVC3DGroup2@@PBV1@0PAV1@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ae7530, 54 bytes, __fastcall (first in ecx, last in edx, result + tag on the stack), `ret 8`.
// Identity (proven): the element copy is BoneIndex + CArray<C3DVertexBlend2> (bound copy constructor 0x00AE5D80) = Ego_r.pdb C3DGroup2 {long BoneIndex; CArray<C3DVertexBlend2> VertexBlends} (16 bytes retail); callers vector<C3DGroup2>::operator= 0x00AE7FE0 and the allocate-and-copy 0x00AE7880 (const sources).
// Callees: CArray<C3DVertexBlend2>::CArray(const CArray&) 0x00AE5D80 (bound).
// Shape (family L011): STLport 4.6 `__uninitialized_copy(first, last, result, const __false_type&)` verbatim; the record's
//   implicit copy constructor under placement new's null test; stride 16.
inline void* operator new(unsigned int, void* p) { return p; }
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {             // STLport pair: implicit copy constructor
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
}
class C3DVertexBlend2;
template <class T> class CArray {                           // retail CArray<T> = 12-byte STLport vector
public:
    T* m_first;
    T* m_last;
    T* m_end;
    CArray(const CArray& other);                          // 0x00AE5D80 for C3DVertexBlend2
};
class C3DGroup2 {                                          // Ego_r.pdb; retail 16 bytes
public:
    long BoneIndex;                                       // +0x00
    CArray<C3DVertexBlend2> VertexBlends;                 // +0x04
};
template C3DGroup2* __fastcall std::__uninitialized_copy<const C3DGroup2*, C3DGroup2*>(const C3DGroup2*, const C3DGroup2*, C3DGroup2*, const std::__false_type&);