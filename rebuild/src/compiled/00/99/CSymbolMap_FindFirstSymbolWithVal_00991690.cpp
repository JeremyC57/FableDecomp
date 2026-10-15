// CSymbolMap::FindFirstSymbolWithVal(long value) -- linear search of the 0x24-byte records in the vector at +0x30/+0x34 for the first whose value word (+0x0c) equals the argument; return that record's symbol word (+0x04), or 0 when none matches
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00991690, length 39 bytes.
// Callees: none. Shape: begin/end loaded into eax/ecx, an entry `cmp ; je` (the for-loop's top test), the
// argument loaded once into edx before the aligned loop head (`mov edi,edi` filler), `add eax,24h` stride,
// `xor eax,eax ; ret 4` for the not-found tail and the found tail placed after it. The record vector
// (first/last at +0x30/+0x34) is not CSymbolMap's PDB layout (that one keeps a CVectorMap at +4), so the
// owner is modelled with a descriptive local layout; the label is propagated over byte-identical twins.
struct SSymbolValueRecord {                        // 0x24 bytes
    long          m_flags;                         // +0x00
    long          m_symbol;                        // +0x04
    long          m_reserved08;                    // +0x08
    long          m_value;                         // +0x0c
    unsigned char _pad10[0x24 - 0x10];
};

struct CSymbolMap {
    unsigned char       _pad00[0x30];
    SSymbolValueRecord* m_pFirst;                  // +0x30
    SSymbolValueRecord* m_pLast;                   // +0x34
    long FindFirstSymbolWithVal(long value) const;
};

long CSymbolMap::FindFirstSymbolWithVal(long value) const
{
    for (const SSymbolValueRecord* it = m_pFirst; it != m_pLast; ++it) {
        if (it->m_value == value)
            return it->m_symbol;
    }
    return 0;
}