// MemCmp_Unsigned16(const unsigned short* lhs, const unsigned short* rhs, unsigned long count) -- compare two runs of 16-bit characters, memcmp-style
// Retail: Fable.exe (TLC, VC7.1) @ 0x00403C60, 75 bytes. Callees: none.
//
// Element-wise comparison for wide strings: walks `count` 16-bit units and returns -1 / 1 at the first
// pair that differs (ordered by unsigned code unit) or 0 when the runs are equal. This is the comparison
// behind CWideString equality and ordering, so it deliberately compares code units, not bytes, and
// never stops at a NUL; the caller decides the length. A zero count compares equal.
//
// Matching: single indexed loop with the compare written as `!=` then `<`, which gives the
// `jae`/`sbb`-free two-branch body; `index` is the loop variable rather than advancing pointers.
int __cdecl MemCmp_Unsigned16(
    const unsigned short* lhs, const unsigned short* rhs, unsigned long count)
{
    unsigned long index;
    for (index = 0; index < count; ++index)
    {
        // First difference decides the order.
        if (lhs[index] != rhs[index])
            return lhs[index] < rhs[index] ? -1 : 1;
    }
    return 0;
}
