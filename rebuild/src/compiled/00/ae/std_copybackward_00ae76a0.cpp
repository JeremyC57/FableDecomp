// std::__copy_backward(first, last, result, const random_access_iterator_tag&, int*) -- (C3DGroup2*, C3DGroup2*, int) STLport random-access backward copy: `for (n = last - first; n > 0; --n) *--result = *--last;`, returns the moved-back result
// Leaf: ??$__copy_backward@PAVC3DGroup2@@PAV1@H@std@@YIPAVC3DGroup2@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AE76A0, 64 bytes (+ int3 padding to 64), `ret 0Ch`: /Gr algorithm TU (first ecx, last edx;
//   result, the const random_access_iterator_tag& and the int* distance tag on the stack), spelled here as explicit __fastcall.
// Identity (proven): element C3DGroup2 (16 bytes); evidence: its callers are 0x00AE79D0 and 0x00AE82C0 -- the latter is the insert(pos, n, x) of the landed vector<C3DGroup2>::resize 0x00AE8540 (the program loader's skinning-group array); C3DGroup2 is {long BoneIndex; CArray<C3DVertexBlend2> VertexBlends} (Ego_r.pdb, class)
// Label: the manifest's "_Copy_backward<std::pair<float,CCharString>*,std::pair<float,class_CCharString>*>" is propagated (family H070: six backward copies over a 4-byte field followed by a member with an
//   out-of-line operator=; retail has no identical-code folding, so each address is one element type). Two shapes: 8-byte elements
//   (`sar eax,3`) and 16-byte ones (`sar eax,4`).
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

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template C3DGroup2* __fastcall __copy_backward<C3DGroup2*, C3DGroup2*, int>(C3DGroup2*, C3DGroup2*, C3DGroup2*,
                                                                         const random_access_iterator_tag&, int*);
}   // namespace std