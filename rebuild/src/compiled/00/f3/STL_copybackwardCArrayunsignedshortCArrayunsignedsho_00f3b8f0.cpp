// std::__copy_backward(CArray<unsigned short>* first, last, result, random_access_iterator_tag, int*) -- counted backward assignment of CArray<unsigned short> elements, returning the moved-back result
// Leaf: ??$__copy_backward@PAV?$CArray@G@@PAV1@H@std@@YIPAV?$CArray@G@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F3B8F0, 66 bytes, `__fastcall` (first ecx, last edx; result, the tag reference and
//   the distance pointer on the stack, `ret 0Ch`):
//   `push esi ; mov esi,edx ; sub edx,ecx ; (divide by 12) ; test eax,eax ; jle` then the loop `sub esi,0Ch ; sub edi,0Ch ;
//   push esi ; mov ecx,edi ; call 0x00F3B640 ; dec ebx ; jne`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 453 "0xF3B8A0/0xF3B8F0 CElement12 = CArray<unsigned short>"; hand453 README label
//   table): the element's operator= is the landed std::vector<unsigned short>::operator= 0x00F3B640 (the CArray<unsigned short>
//   base; its row lists these two loops among its callers), so the element is CArray<unsigned short> (12 bytes), as in the
//   CTriangleStripifier2 TU whose Ego_r.pdb signatures pass CArray<CArray<unsigned short> > (`V?$CArray@V?$CArray@G@@@@`);
//   callers 0x00F3F6CC / 0x00F3F9F8 in that TU. Body: STLport 4.6 _algobase.h as written; the landed CElement12_00f3b640 placeholder
//   rows had the same loop with a 12-byte opaque element.
// Namespace: std as in this TU's landed rows (0x00F3B640, 0x00F3B760, 0x00F3B7D0).
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {
public:
    _Tp* _M_start;                                   // +0x00
    _Tp* _M_finish;                                  // +0x04
    _Tp* _M_end_of_storage;                          // +0x08 (the _STLP_alloc_proxy's pointer)
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);   // 0x00F3B640 for unsigned short
};
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };
}
template <class T> class CArray : public std::vector<T> { };   // 12 bytes; the implicit operator= is the vector's
namespace std {
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                              const random_access_iterator_tag&, _Distance*)   // _algobase.h
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
template CArray<unsigned short>* __fastcall __copy_backward(CArray<unsigned short>*, CArray<unsigned short>*, CArray<unsigned short>*,
                                                            const random_access_iterator_tag&, int*);
}
