// _global::CopyBackward_SFlaggedTreePair1C_00ad6360(SFlaggedTreePair1C* first, SFlaggedTreePair1C* last, SFlaggedTreePair1C* result, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 28-byte {byte, 12-byte tree, 12-byte tree} records: counted backward copy, returns the moved-back result
// Leaf: CopyBackward_SFlaggedTreePair1C_00ad6360
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD6360, length 93 bytes, `ret 0Ch`.
// Callees: the 12-byte member's operator= @ 0x00AD3720 (twice; catalog "LTextTreeWalkThrough_CopyAssign").
// Shape: SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)` -- `for (n = last - first;
// n > 0; --n) *--result = *--last;`, the moved-back result returned (landed 0x00517430 family). /Gr TU (__fastcall: first
// in ecx, last in edx, result + the two tags on the stack, `ret 0Ch`). The element's implicit operator= is inlined
// memberwise: the leading byte through al (stored after the first `lea` pair), then the two out-of-line member assignments at +4 and +0x10.
// Label correction: the catalog's `_Copy_backward<CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc*>` is propagated; member names are descriptive.
struct STree_00ad3720 {                         // 12 bytes
    void* m_header;
    void* m_root;
    long  m_count;
    STree_00ad3720& operator=(const STree_00ad3720& other);   // 0x00AD3720
};
struct SFlaggedTreePair1C {                     // 28 bytes
    unsigned char m_flag;                        // +0x00
    STree_00ad3720 m_first;                      // +0x04
    STree_00ad3720 m_second;                     // +0x10
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SFlaggedTreePair1C* __fastcall CopyBackward_SFlaggedTreePair1C_00ad6360(SFlaggedTreePair1C* first, SFlaggedTreePair1C* last, SFlaggedTreePair1C* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}