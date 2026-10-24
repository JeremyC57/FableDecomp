// _global::CopyBackwardPtrs_Element32_00a95a70(SElement32*, SElement32*, SElement32*, const __false_type&) -- STLport __copy_backward_ptrs: counted backward copy of 0x20-byte POD elements (the SGI __copy_backward inlined)
// Leaf: CopyBackwardPtrs_Element32_00a95a70
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A95A70, length 46 bytes.
// Shape: `ret 8` -- result and ONE reference argument on the stack (the caller 0x00A970E1 pushes `lea eax,[esp+34h]`
// then the result). This is STLport's `__copy_backward_ptrs(first, last, result, const __false_type&)` with the SGI
// `__copy_backward(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0)` inlined: the inlined loop loads
// result first (`mov eax,[esp+4]`), counts in esi/ebx and forms the source as (last - result) + result (`lea
// esi,[edx+eax]`) for the 8-dword `rep movsd` of the implicit POD operator=. The out-of-line 4/5-argument
// __copy_backward form keeps two pointer registers instead (tried: plain, /Oa, /Ow, tag by reference, /Os).
// Element: 0x20-byte POD (sar 5). The manifest's `pair<unsigned long,C3DDummyObject>` label is propagated (that pair
// is 0x38 bytes, see 0x00A314E0); the caller belongs to the animation key-frame vector assign (0x00A97040), so the
// element stays a descriptive 32-byte record.
struct SElement32 {
    unsigned long d[8];
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

inline SElement32* __copy_backward(SElement32* first, SElement32* last, SElement32* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

SElement32* __fastcall CopyBackwardPtrs_Element32_00a95a70(SElement32* first, SElement32* last, SElement32* result, const __false_type&)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}