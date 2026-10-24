// _global::CopyBackward_SVectorPair18_0051c710(SVectorPair18* first, SVectorPair18* last, SVectorPair18* result, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 24-byte records of two 12-byte vectors: counted backward copy, returns the moved-back result
// Leaf: CopyBackward_SVectorPair18_0051c710
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051C710, length 79 bytes (+ int3 padding), `ret 0Ch`.
// Callees: operator= @ 0x00515320 (catalog "std::vector<C2DLineF>::operator=") on +0, operator= @ 0x0051BCD0 on +0xC.
// Shape: SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)` -- `for (n = last - first;
// n > 0; --n) *--result = *--last;`, the moved-back result returned (landed 0x00517430 family). /Gr TU (__fastcall: first
// in ecx, last in edx, result + the two tags on the stack, `ret 0Ch`). The element's implicit operator= is inlined
// memberwise: two out-of-line vector assignments, `push esi ; mov ecx,edi` for the first, `lea` of both +0xC members for the second.
// Label correction: the catalog's `_Copy_backward<CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc*>` is propagated; member names are descriptive.
struct SVector_00515320 {                       // 12 bytes
    void* m_first;
    void* m_last;
    void* m_end;
    SVector_00515320& operator=(const SVector_00515320& other);   // 0x00515320
};
struct SVector_0051bcd0 {                       // 12 bytes
    void* m_first;
    void* m_last;
    void* m_end;
    SVector_0051bcd0& operator=(const SVector_0051bcd0& other);   // 0x0051BCD0
};
struct SVectorPair18 {                          // 24 bytes
    SVector_00515320 m_lines;                    // +0x00
    SVector_0051bcd0 m_points;                   // +0x0c
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SVectorPair18* __fastcall CopyBackward_SVectorPair18_0051c710(SVectorPair18* first, SVectorPair18* last, SVectorPair18* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}