// std::__copy(first, last, result, const random_access_iterator_tag&, int*) -- (CCharString elements) STLport random-access forward copy: `for (n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }`, returns the advanced result
// Leaf: ??$__copy@PAVCCharString@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x008AAC10, 53 bytes, /Gr __fastcall (first ecx, last edx; result, the tag reference and the
//   int* distance tag on the stack), `ret 0Ch`.
// Identity (probable): __copy<CCharString*, CCharString*, int>; the per-element call is CCharString::operator= 0x0099EFB0 (bound ??4CCharString@@QAEAAV0@ABV0@@Z) over 4-byte elements; no direct callers.
// Callee: CCharString::operator= 0x0099EFB0.
// Shape (family K073, representative 0x0040E290): the landed __copy<CQuestCardItem*> 0x00623150 text; the element's own
//   operator= is out of line, so each pass is one call with the source pushed and the destination in ecx.
class CCharString {                                      // 4 bytes: shared string block
public:
    void* PStringData;
    const CCharString& operator=(const CCharString& other);    // 0x0099EFB0
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

template CCharString* __fastcall __copy<CCharString*, CCharString*, int>(CCharString*, CCharString*, CCharString*, const random_access_iterator_tag&, int*);
}   // namespace std