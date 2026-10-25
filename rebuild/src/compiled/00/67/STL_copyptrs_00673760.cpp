// _STL::__copy_ptrs(const SRecord44* first, const SRecord44* last, SRecord44* result, const __false_type&) -- STLport assignment copy of a range of 44-byte records (the memcpy-unsafe pointer path): counted loop of block assignments, returns the end of the destination
// Leaf: ??$__copy_ptrs@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00673760, length 60 bytes, `ret 8`.
// Callers: the record vector's operator= 0x00674520 (manifest "Vector_Assign_44byte"; 0x006745E9 when the source fits in
//   the current size, 0x0067460F for the overlapping head when it fits in the capacity).
// Callees: none (the element's implicit operator= is the 11-dword `rep movsd`).
// Label: catalog "std::copy_range<44byte_element>" is descriptive; STLport spelling __copy_ptrs (+ inline __copy).
// Element: a 44-byte record whose type is not identified (descriptive model; a plain aggregate of 11 dwords is
//   enough for the implicit block assignment).
// Shape: STLport 4.x `__copy_ptrs(first, last, result, const __false_type&) { return __copy(first, last, result,
//   random_access_iterator_tag(), (ptrdiff_t*)0); }` with __copy inline (`for (_Distance __n = __last - __first; __n > 0;
//   --__n) { *__result = *__first; ++__first; ++__result; }`): the count is the signed divide by 44 (0x2E8BA2E9
//   reciprocal), the source strength-reduced to `(first - result) + result`; __copy is `__forceinline` (plain `inline`
//   leaves it an out-of-line `__cdecl` call). /Gr algorithm TU (`__fastcall`: first in
//   ecx, last in edx, result and the tag reference on the stack).
struct SRecord44 {
    unsigned long Words[11];                               // 0x2C bytes, copied as a block
};

namespace _STL {
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template <class _InputIter, class _OutputIter>
_OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
}

template SRecord44* __fastcall _STL::__copy_ptrs<const SRecord44*, SRecord44*>(const SRecord44*, const SRecord44*, SRecord44*, const _STL::__false_type&);