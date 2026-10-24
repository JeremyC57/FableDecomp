// _global::CopyBackwardPtrs_long_00672210(const long* first, const long* last, long* result, const __false_type&) -- STLport __copy_backward_ptrs<long*>: counted backward copy of 4-byte words, returns the moved-back result
// Leaf: CopyBackwardPtrs_long_00672210
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00672210, length 36 bytes (+ int3 padding), `ret 8`.
// Caller: the sorted-insert step 0x006721C0 (catalog "std::InsertSorted_Pair"; it compares the words signed, `cmp ; jge`)
// calls it as copy_backward(first, last, last + 1) with `lea` of a zeroed tag byte.
// Shape: STLport 4.x `__copy_backward_ptrs(first, last, result, const __false_type&)` with `__copy_backward(first, last,
// result, random_access_iterator_tag(), (ptrdiff_t*)0)` inlined: `for (n = last - first; n > 0; --n) *--result = *--last;`
// -- the count `sar 2`, `last` strength-reduced to `(last - result) + result` (`mov ecx,edx ; sub ecx,eax ; mov edx,
// [ecx+eax-4]`), the moved-back result returned in eax. /Gr algorithm TU (__fastcall: first in ecx, last in edx, result +
// the tag reference on the stack).
// Label correction: the catalog's `_Copy_impl<std::pair<float,EConversationAttitude>*,...>` is propagated (that pair is 8
// bytes; these elements are 4).
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

inline long* __copy_backward(const long* first, const long* last, long* result, const random_access_iterator_tag&, int*)
{
    for (int n = int(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}

long* __fastcall CopyBackwardPtrs_long_00672210(const long* first, const long* last, long* result, const __false_type&)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}