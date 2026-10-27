// std::__copy(first, last, result, const random_access_iterator_tag&, int*) -- (CWideString elements) STLport random-access forward copy: `for (n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }`, returns the advanced result
// Leaf: ??$__copy@PAVCWideString@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0040E290, 53 bytes, /Gr __fastcall (first ecx, last edx; result, the tag reference and the
//   int* distance tag on the stack), `ret 0Ch`.
// Identity (probable): __copy<CWideString*, CWideString*, int>; the per-element call is CWideString::operator= 0x0099B7D0 (bound ??4CWideString@@QAEAAV0@ABV0@@Z) over 4-byte elements; its caller 0x0059AE20 is a vector erase-range copy-back.
// Callee: CWideString::operator= 0x0099B7D0.
// Shape (family K073, representative 0x0040E290): the landed __copy<CQuestCardItem*> 0x00623150 text; the element's own
//   operator= is out of line, so each pass is one call with the source pushed and the destination in ecx.
class CWideString {                                      // 4 bytes: shared string block
public:
    void* PStringData;
    CWideString& operator=(const CWideString& other);    // 0x0099B7D0
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

template CWideString* __fastcall __copy<CWideString*, CWideString*, int>(CWideString*, CWideString*, CWideString*, const random_access_iterator_tag&, int*);
}   // namespace std