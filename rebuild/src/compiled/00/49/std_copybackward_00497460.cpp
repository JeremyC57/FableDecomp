// std::copy_backward(CWideString* first, CWideString* last, CWideString* result) -- STLport backward copy over a 4-byte element with a user assignment operator (CWideString): n = last - first elements assigned back to front so the copy ends at result, returning its new start
// Leaf: ??$copy_backward@PAVCWideString@@PAV1@@std@@YIPAVCWideString@@PAV1@00@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00497460, 49 bytes, __fastcall (first ecx, last edx), `ret 4`; the element's
//   operator= 0x0099B7D0 called per element (thiscall on --result with --last pushed); returns result - n in eax.
// Identity (proven): std::copy_backward<CWideString*, CWideString*>; evidence: its callers are the landed __linear_insert<CWideString*, CWideString, NGameComponentHelpers::CCaseInsensitiveStringCompare> 0x00497500 and __linear_insert<CWideString*, CWideString, less<CWideString> > 0x009B58B0 (`copy_backward(first, last, last + 1)`: ecx, edx and one pushed argument for this `ret 4` body, so the front end copy_backward itself, with __copy_backward_aux / __copy_backward_ptrs / __copy_backward inlined); its per-element call is CWideString::operator= 0x0099B7D0.
// Label: the manifest's "VectorInsert_CopyWideStringsReverse" is a descriptive / propagated name (family H041: six 49-byte backward copies that differ
//   only in the element's assignment callee and the stack-argument count).
// Callees: CWideString::operator= 0x0099B7D0 (FableWin.pdb ??4CWideString@@QAEABV0@ABV0@@Z).
// Shape: STLport 4.6 _algobase.h `copy_backward(first, last, result)` -> `__copy_backward_aux(..., __true_type)` -> `__copy_backward_ptrs(..., __false_type)` -> the random-access `__copy_backward`, all inlined into the front end; __copy_backward is __forceinline (as plain `inline` VC7.1 keeps it out of line as a
//   cdecl call, 25v49), the loop counts n down in ebx and reads the source through the constant distance last - result.
class CWideString {                                     // one pointer
public:
    void* Data;
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__forceinline _BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                 _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _RandomAccessIter, class _BidirectionalIter>
inline _BidirectionalIter __fastcall __copy_backward_ptrs(_RandomAccessIter __first, _RandomAccessIter __last,
                                                          _BidirectionalIter __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
__forceinline _BidirectionalIter2 __copy_backward_aux(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                      _BidirectionalIter2 __result, const __true_type&)
{
    return __copy_backward_ptrs(__first, __last, __result, __false_type());
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __fastcall copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result)
{
    return __copy_backward_aux(__first, __last, __result, __true_type());
}

template CWideString* __fastcall copy_backward<CWideString*, CWideString*>(CWideString*, CWideString*, CWideString*);
}