// CSymbolMap::FindFirstSymbolWithVal(const CCharString& name) -- case-insensitive name search of the 0x24-byte records in the vector at +0x30/+0x34: return the symbol word (+0x04) of the first record whose CCharString (+0x14) equals the argument under _stricmp, or 0 when none matches
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009917E0, length 87 bytes.
// Callees: _stricmp @ 0x00BFEBA8 (`__cdecl`, `add esp,8`); the CCharString empty-text global @ 0x0129AAF4.
// Shape: `this` in edi because the end word (+0x34) is re-read after every call; the key reference in ebx and
// its text re-derived each iteration (the call may change it): both texts go through the null-case-first
// accessor `m_pData == 0 ? empty : m_pData->m_pChars`; VC evaluates the right argument (the key) first.
// The record vector at +0x30 is not CSymbolMap's PDB layout, so the owner is a descriptive local layout.
extern "C" int __cdecl _stricmp(const char* a, const char* b);   // 0x00BFEBA8
extern const char g_EmptyCharString[];             // 0x0129AAF4

struct CCharStringData {
    const char*   m_pChars;                        // +0x00
    unsigned long m_length;                        // +0x04
};

struct CCharString {
    CCharStringData* m_pData;
    const char* GetChars() const { return m_pData == 0 ? g_EmptyCharString : m_pData->m_pChars; }
};

struct SSymbolNameRecord {                         // 0x24 bytes
    long          m_flags;                         // +0x00
    long          m_symbol;                        // +0x04
    unsigned char _pad08[0x14 - 0x08];
    CCharString   m_name;                          // +0x14
    unsigned char _pad18[0x24 - 0x18];
};

struct CSymbolMap {
    unsigned char      _pad00[0x30];
    SSymbolNameRecord* m_pFirst;                   // +0x30
    SSymbolNameRecord* m_pLast;                    // +0x34
    long FindFirstSymbolWithVal(const CCharString& name) const;
};

long CSymbolMap::FindFirstSymbolWithVal(const CCharString& name) const
{
    for (const SSymbolNameRecord* it = m_pFirst; it != m_pLast; ++it) {
        if (_stricmp(it->m_name.GetChars(), name.GetChars()) == 0)
            return it->m_symbol;
    }
    return 0;
}
