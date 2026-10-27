// std::__copy(first, last, result, const random_access_iterator_tag&, int*) -- (CTexture elements) STLport random-access forward copy: `for (n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }`, returns the advanced result
// Leaf: ??$__copy@PAVCTexture@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049AE30, 53 bytes, /Gr __fastcall (first ecx, last edx; result, the tag reference and the
//   int* distance tag on the stack), `ret 0Ch`.
// Identity (probable): __copy<CTexture*, CTexture*, int>; the per-element call is the landed CTexture::operator= 0x009FA1C0 over 8-byte elements (Ego_r.pdb CTexture is 8 bytes); no direct callers.
// Callee: CTexture::operator= 0x009FA1C0.
// Shape (family K073, representative 0x0040E290): the landed __copy<CQuestCardItem*> 0x00623150 text; the element's own
//   operator= is out of line, so each pass is one call with the source pushed and the destination in ecx.
struct IDirect3DTexture9;
class CTexture {                                         // Ego_r.pdb, 8 bytes
public:
    IDirect3DTexture9* PD3DTexture;                      // +0x00
    unsigned long Bits;                                  // +0x04 ByteLength:28 / AllocationSource:4
    CTexture& operator=(const CTexture& other);          // 0x009FA1C0
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

template CTexture* __fastcall __copy<CTexture*, CTexture*, int>(CTexture*, CTexture*, CTexture*, const random_access_iterator_tag&, int*);
}   // namespace std