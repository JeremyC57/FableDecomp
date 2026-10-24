// _global::Copy_SFlaggedTreePair1C_00ad6ff0(SFlaggedTreePair1C* first, SFlaggedTreePair1C* last, SFlaggedTreePair1C* result, SRandomAccessIteratorTag, long*) -- SGI __copy (forward) over 28-byte {byte, tree, tree} records: counted forward copy, returns the advanced result
// Leaf: Copy_SFlaggedTreePair1C_00ad6ff0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD6FF0, length 91 bytes (+ int3 padding), `ret 0Ch`.
// Callee: the 12-byte member's operator= @ 0x00AD3720 (twice; catalog "LTextTreeWalkThrough_CopyAssign").
// Shape: SGI-STL `__copy(first, last, result, random_access_iterator_tag, Distance*)` -- `for (n = last - first; n > 0;
// --n) { *result = *first; ++first; ++result; }` (first stepped before result, as retail), the advanced result returned. /Gr TU (__fastcall: first in ecx, last in
// edx, result + the two tags on the stack). The element (same as the 0x00AD6360 __copy_backward neighbour) assigns
// memberwise: the flag byte through al, then the two out-of-line tree assignments at +4 and +0x10; both pointers step
// by 0x1C after the calls.
// Label correction: the catalog's `_Copy_backward<CStaticMapGenerationVisibiilityInfo::CSeaBodyDesc*>` is propagated:
// this body copies forward. Member names are descriptive.
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

SFlaggedTreePair1C* __fastcall Copy_SFlaggedTreePair1C_00ad6ff0(SFlaggedTreePair1C* first, SFlaggedTreePair1C* last, SFlaggedTreePair1C* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}