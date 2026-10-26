// std::__copy_backward_ptrs(CCharString* first, CCharString* last, CCharString* result, const __false_type&) -- STLport backward copy over a 4-byte element with a user assignment operator (CCharString): n = last - first elements assigned back to front so the copy ends at result, returning its new start
// Leaf: ??$__copy_backward_ptrs@PAVCCharString@@PAV1@@std@@YIPAVCCharString@@PAV1@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049B180, 49 bytes, __fastcall (first ecx, last edx), `ret 8`; the element's
//   operator= 0x0099EFB0 called per element (thiscall on --result with --last pushed); returns result - n in eax.
// Identity (proven): std::__copy_backward_ptrs<CCharString*, CCharString*>; evidence: its only caller is the landed vector<CCharString>::_M_range_insert 0x0052B6A0 (`__copy_backward_ptrs(position, old_finish - n, old_finish, __false_type())`), and its per-element call is CCharString::operator= 0x0099EFB0.
// Label: the manifest's "Std_Copy_Backward_CCharString" is a descriptive / propagated name (family H041: six 49-byte backward copies that differ
//   only in the element's assignment callee and the stack-argument count).
// Callees: CCharString::operator= 0x0099EFB0 (FableWin.pdb ??4CCharString@@QAEABV0@ABV0@@Z).
// Shape: STLport 4.6 _algobase.h `__copy_backward_ptrs(first, last, result, const __false_type&)` -- the non-trivial-assignment overload vector::_M_fill_insert / insert / _M_range_insert call with _TrivialAss() -- with `__copy_backward(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0)` inlined; __copy_backward is __forceinline (as plain `inline` VC7.1 keeps it out of line as a
//   cdecl call, 25v49), the loop counts n down in ebx and reads the source through the constant distance last - result.
class CCharString {                                     // one pointer
public:
    void* Data;
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
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

template CCharString* __fastcall __copy_backward_ptrs<CCharString*, CCharString*>(CCharString*, CCharString*, CCharString*, const __false_type&);
}