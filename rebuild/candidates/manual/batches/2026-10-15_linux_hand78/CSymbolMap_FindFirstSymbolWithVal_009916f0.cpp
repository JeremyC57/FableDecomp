// CSymbolMap::FindFirstSymbolWithVal(long symbol) -- the reverse lookup of the 0x24-byte record vector at +0x30/+0x34: find the first record whose symbol word (+0x04) equals the argument and return its value word (+0x0c), or 0 when none matches
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009916F0, length 39 bytes.
// Callees: none. Shape: identical to the 0x00991690 twin with the key/result words swapped (`cmp [eax+4],edx`
// in the aligned loop, `mov eax,[eax+0Ch]` in the found tail after the `xor eax,eax ; ret 4` exit). The
// record vector at +0x30 is not CSymbolMap's PDB layout, so the owner is a descriptive local layout.
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
    long FindFirstSymbolWithVal(long symbol) const;
};

long CSymbolMap::FindFirstSymbolWithVal(long symbol) const
{
    for (const SSymbolValueRecord* it = m_pFirst; it != m_pLast; ++it) {
        if (it->m_symbol == symbol)
            return it->m_value;
    }
    return 0;
}
