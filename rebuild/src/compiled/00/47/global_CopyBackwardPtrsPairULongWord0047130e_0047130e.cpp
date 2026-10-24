// _global::CopyBackwardPtrs_PairULongWord_0047130e(SPairULongWord*, SPairULongWord*, SPairULongWord*, const __false_type&) -- STLport __copy_backward_ptrs: counted backward copy of 8-byte POD pairs (the SGI __copy_backward inlined)
// Leaf: CopyBackwardPtrs_PairULongWord_0047130e
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0047130E, length 42 bytes (size-optimised TU: no alignment padding).
// Shape: `ret 8` -- only result and ONE reference argument on the stack: the caller (0x004712D6, an insertion-sort step
// over the same 8-byte elements) pushes &tag (a zero-filled byte local) and result. This is STLport's
// `__copy_backward_ptrs(first, last, result, const __false_type&)` with the SGI
// `__copy_backward(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0)` inlined into it: inlining makes
// VC7.1 load result first (`mov eax,[esp+4]`), keep the count in esi and address the source as result + (last -
// result) (`mov ecx,edx ; sub ecx,eax` then `[ecx+eax]`). An out-of-line 4-argument loop instead keeps both pointers.
// In this /Os TU the inner helper must be __forceinline (a plain inline one is called out of line).
// Element: 8-byte POD pair; the caller orders it by an unsigned key (`cmp ; jae/jb`). The manifest's
// `pair<long,float>` label is propagated; the second word's type is not observable here.
#pragma optimize("s", on)

struct SPairULongWord {
    unsigned long first;               // +0x00 sort key
    unsigned long second;              // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

__forceinline SPairULongWord* __copy_backward(SPairULongWord* first, SPairULongWord* last, SPairULongWord* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

SPairULongWord* __fastcall CopyBackwardPtrs_PairULongWord_0047130e(SPairULongWord* first, SPairULongWord* last, SPairULongWord* result, const __false_type&)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}