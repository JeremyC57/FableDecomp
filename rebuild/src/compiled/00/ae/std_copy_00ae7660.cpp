// std::__copy(_RandomAccessIter first, _RandomAccessIter last, _OutputIter result, const random_access_iterator_tag&, _Distance*) -- STLport random-access copy: assign last - first elements forward, return the end of the output (C3DGroup2)
// Leaf: ??$__copy@PBVC3DGroup2@@PAV1@H@std@@YIPAVC3DGroup2@@PBV1@0PAV1@ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ae7660, 61 bytes, __fastcall (first in ecx, last in edx), `ret 0Ch`.
// Identity (probable): the element's assignment is the leading dword then vector<C3DVertexBlend2>::operator= 0x00AE6690 (bound) on +4 = Ego_r.pdb C3DGroup2::VertexBlends (CArray<C3DVertexBlend2>), 16-byte stride; callers vector<C3DGroup2>::operator= 0x00AE7FE0 (twice, const sources; family L011's __uninitialized_copy<const C3DGroup2*> 0x00AE7530 serves the same vector) and the allocate-and-copy 0x00AE7880.
// Callee: the second member's operator= 0x00AE6690 (thiscall).
// Shape (family L049): n = (last - first) >> 4; per element the leading dword copied through eax, then one thiscall
//   assignment of the member at +4; returns the advanced result.
class C3DVertexBlend2 {                                    // Ego_r.pdb, 8 bytes
public:
    unsigned long VertexIndex;                            // +0x00
    float BlendValue;                                     // +0x04
};
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {                                             // STLport 4.6, 12 bytes
public:
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _Tp* _M_end_of_storage;                               // +0x08
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);   // 0x00AE6690 for C3DVertexBlend2
};
}
template <class T> class CArray : public std::vector<T> { };   // retail CArray<T> = the 12-byte STLport vector
class C3DGroup2 {                                          // Ego_r.pdb, 16 bytes in retail
public:
    long BoneIndex;                                       // +0x00
    CArray<C3DVertexBlend2> VertexBlends;                 // +0x04
};
namespace std {
struct random_access_iterator_tag { };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
}
template C3DGroup2* __fastcall std::__copy(const C3DGroup2*, const C3DGroup2*, C3DGroup2*, const std::random_access_iterator_tag&, int*);