// _global::CopyBackward_SSortedItemPtr_007108a0(SSortedItem** first, SSortedItem** last, SSortedItem** result) -- STLport copy_backward over a pointer array: counted backward copy, returns the moved-back result
// Leaf: CopyBackward_SSortedItemPtr_007108a0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007108A0, length 36 bytes, `ret 4`.
// Caller: the stable-sort merge 0x00710D00 (catalog "StableSortMerge_Pair_EConversationAttitude"; a __merge_backward over
// pointers ordered by the signed long at +0x54 of each pointee) copies the leftover run with copy_backward(first1, last1, result).
// Shape: STLport 4.x `copy_backward(first, last, result)` with the pointer dispatch and `__copy_backward(first, last,
// result, random_access_iterator_tag(), (ptrdiff_t*)0)` all inlined (only result on the stack: `ret 4`):
// `for (n = last - first; n > 0; --n) *--result = *--last;` -- the count `sar 2`, `last` strength-reduced to
// `(last - result) + result`, the moved-back result returned. Same bytes as 0x00672210 but for the one-argument entry.
// Label correction: the catalog's `_Copy_impl<std::pair<float,EConversationAttitude>*,...>` is propagated (that pair is 8
// bytes; these elements are 4-byte pointers); the pointee is descriptive.
struct SSortedItem {
    unsigned char _pad[0x54];
    long          m_sortKey;                     // +0x54 (the merge's key)
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

inline SSortedItem** __copy_backward(SSortedItem** first, SSortedItem** last, SSortedItem** result, const random_access_iterator_tag&, int*)
{
    for (int n = int(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}

SSortedItem** __fastcall CopyBackward_SSortedItemPtr_007108a0(SSortedItem** first, SSortedItem** last, SSortedItem** result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}