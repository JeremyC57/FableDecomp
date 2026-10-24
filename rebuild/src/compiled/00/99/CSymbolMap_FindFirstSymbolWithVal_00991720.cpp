// CSymbolMap::FindFirstSymbolWithVal() -- return the value word (+0x0c) of the first 0x24-byte record in the vector at +0x30/+0x34 whose value is set (non-zero), or 0 when every record is empty
// Leaf: FindFirstSymbolWithVal
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00991720, length 37 bytes.
// Callees: none. Shape: no argument (`ret`), begin/end into eax/ecx, the top test, the six-byte
// `lea ebx,[ebx]` loop-alignment filler, `mov edx,[eax+0Ch] ; test edx,edx ; jne` and a SECOND load of
// the same word in the found tail (VC7.1 does not forward the tested register across the loop exit).
// The record vector at +0x30 is not CSymbolMap's PDB layout, so the owner is a descriptive local layout.
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
    long FindFirstSymbolWithVal() const;
};

long CSymbolMap::FindFirstSymbolWithVal() const
{
    for (const SSymbolValueRecord* it = m_pFirst; it != m_pLast; ++it) {
        if (it->m_value != 0)
            return it->m_value;
    }
    return 0;
}