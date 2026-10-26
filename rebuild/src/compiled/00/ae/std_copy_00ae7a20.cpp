// std::__copy(first, last, result, const random_access_iterator_tag&, int*) -- (C3DGroup2*, C3DGroup2*, int) STLport random-access forward copy: `for (n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }`, returns the advanced result
// Leaf: ??$__copy@PAVC3DGroup2@@PAV1@H@std@@YIPAVC3DGroup2@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE7A20, 61 bytes (+ int3 padding to 64), `ret 0Ch`: /Gr algorithm TU (first ecx, last edx;
//   result, the const random_access_iterator_tag& and the int* distance tag on the stack), spelled here as explicit __fastcall.
// Identity (proven): element C3DGroup2 (16 bytes); evidence: its callers are 0x00AE7BB0 and 0x00AE7F30 (the vector operator= over 16-byte elements whose +4 member is assigned by vector<C3DVertexBlend2>::operator= 0x00AE6690); H070's 0x00AE76A0 is the same element's backward copy, the program loader's skinning groups -- C3DGroup2 is {long BoneIndex; CArray<C3DVertexBlend2> VertexBlends} (Ego_r.pdb, class)
// Label: the manifest's "_Copy_impl<CTCSkeletalMorph::CSkeletalMorph*,CTCSkeletalMorph::CSkeletalMorph*>" is propagated (family H081: six forward copies over a 4-byte field followed by a member with an
//   out-of-line operator=; retail has no identical-code folding, so each address is one element type). Two shapes: 8-byte elements
//   (`sar edx,3`) and 16-byte ones (`sar edx,4`).
// Callee: vector<C3DVertexBlend2>::operator= 0x00AE6690 on VertexBlends.
// Shape: the element's implicit operator= inline -- the first dword through eax, then the member's operator= on +4.
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // retail: 12 bytes
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector& operator=(const vector& __x);              // out of line (0x00AE6690)
};
}
template <class T> class CArray : public std::vector<T> { };     // retail CArray: an STLport vector (12 bytes)
class C3DVertexBlend2;                               // 8 bytes
class C3DGroup2 {                                    // 16 bytes in retail
public:
    long                     BoneIndex;              // +0x00
    CArray<C3DVertexBlend2>  VertexBlends;           // +0x04
};

namespace std {

struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                              _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template C3DGroup2* __fastcall __copy<C3DGroup2*, C3DGroup2*, int>(C3DGroup2*, C3DGroup2*, C3DGroup2*,
                                                                const random_access_iterator_tag&, int*);
}   // namespace std