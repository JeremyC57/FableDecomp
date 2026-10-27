// std::__copy_backward<CTexture*, CTexture*, int>(CTexture* first, CTexture* last, CTexture* result, const random_access_iterator_tag&, int*) -- STLport random-access backward copy: n = last - first elements assigned back to front, returns the new start of the copy
// Leaf: ??$__copy_backward@PAVCTexture@@PAV1@H@std@@YIPAVCTexture@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe @ 0x0049AE70, 55 bytes, __fastcall (first ecx, last edx; result, the tag and the int* distance tag on the
// stack), `ret 0Ch`; per element `push --last ; mov ecx,--result ; call 0x009FA1C0`, the count in ebx.
// Identity (probable): the out-of-line STLport 4.6 __copy_backward over 8-byte (`sar eax,3`) elements whose assignment is
// CTexture::operator= 0x009FA1C0 (FableWin.pdb ??4CTexture@@QAEABV0@ABV0@@Z); it sits between the landed CTexture helpers
// __copy 0x0049AE30 and __copy_backward_ptrs 0x0049B0F0 (which inlines its own __copy_backward). No direct caller (gap-scan COMDAT).
// Callee: CTexture::operator= @ 0x009FA1C0.
#define CTexture DonorCTexture
#include "engine/CTexture.h"
#undef CTexture
class CTexture : public DonorCTexture {                 // engine/CTexture.h, 8 bytes
public:
    const CTexture& operator=(const CTexture& other);   // 0x009FA1C0
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

template CTexture* __fastcall __copy_backward<CTexture*, CTexture*, int>(CTexture*, CTexture*, CTexture*, const random_access_iterator_tag&, int*);
}