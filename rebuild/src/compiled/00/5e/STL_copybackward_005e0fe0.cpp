// std::copy_backward(CCharString* first, CCharString* last, CCharString* result) -- STLport copy_backward of a CCharString range (the inlined random-access __copy_backward), returns the moved-back result
// Leaf: ?copy_backward@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E0FE0, 49 bytes (+ padding to 64), `ret 4` (__fastcall /Gr algorithm TU: first in ecx,
//   last in edx, result on the stack -- three arguments, so the out-of-line layer is copy_backward itself with the
//   tagged `__copy_backward(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0)` inlined; it needs
//   `__forceinline` here -- a plain `inline` loop body stays out of line as a __cdecl call). Leaf `?copy_backward@@`:
//   the bare name also matches the `__copy_backward` symbol.
// Callees: 0x0099EFB0 CCharString::operator=.
// Instantiation: sort<CCharString*> @ 0x005E3A50 (called by __linear_insert 0x005E1A40 as copy_backward(first, last, last + 1)).
// Shape: `for (n = last - first; n > 0; --n) *--result = *--last;` -- the count in ebx, `last` strength-reduced to
//   `result + (last - result)` (edi holds the constant distance).
// Label: catalogued as `STL_CopyBackward_CCharString`.
struct CCharString {
    void* PStringData;
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

__forceinline CCharString* __copy_backward(CCharString* first, CCharString* last, CCharString* result,
                                    const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

CCharString* __fastcall copy_backward(CCharString* first, CCharString* last, CCharString* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}