// _global::CopyBackward_SPairWithList10_00a821c0(SPairWithList10* first, SPairWithList10* last, SPairWithList10* result, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 16-byte {long, long, 8-byte list} records: counted backward copy, returns the moved-back result
// Leaf: CopyBackward_SPairWithList10_00a821c0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A821C0, length 70 bytes (+ int3 padding), `ret 0Ch`.
// Callees: the 8-byte member's operator= @ 0x00517720 (catalog "std::list::operator=").
// Shape: SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)` -- `for (n = last - first;
// n > 0; --n) *--result = *--last;`, the moved-back result returned (landed 0x00517430 family). /Gr TU (__fastcall: first
// in ecx, last in edx, result + the two tags on the stack, `ret 0Ch`). The element's implicit operator= is inlined
// memberwise: the two words, then the out-of-line assignment of the 8-byte member at +8; the count is `sar eax,4` (power-of-two size).
// Label correction: the catalog's `_Copy_backward<CSharerParserOutputToDebugContextMapper::CEntry*>` is propagated; member names are descriptive.
struct SList8_00517720 {                        // 8 bytes
    void* m_pHead;
    long  m_size;
    SList8_00517720& operator=(const SList8_00517720& other);   // 0x00517720
};
struct SPairWithList10 {                        // 16 bytes
    long m_first;                                // +0x00
    long m_second;                               // +0x04
    SList8_00517720 m_list;                      // +0x08
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SPairWithList10* __fastcall CopyBackward_SPairWithList10_00a821c0(SPairWithList10* first, SPairWithList10* last, SPairWithList10* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}