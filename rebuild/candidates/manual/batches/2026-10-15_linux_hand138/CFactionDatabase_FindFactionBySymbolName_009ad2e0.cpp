// CFactionDatabase::FindFactionBySymbolName(const CCharString& name) -- CRC the faction's symbol name and look it up in the CRC-keyed faction map; returns the faction's counted pointer, or null when there is none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AD2E0, 98 bytes (+ int3 padding).
// Callees: Calc @ 0x004014A0 (`__fastcall(0, chars, length)`, the CRC; "" @ 0x0129AAF4 for a null string),
//   CVectorMap<unsigned long, CCountedPointer<CFaction>>::LowerBound(const unsigned long*) @ 0x009B2290.
// Layout: the faction map (CVectorMap: first +0x78, last +0x7c) of 12-byte pair<unsigned long, CCountedPointer<CFaction>>;
//   modelled locally (no generated header for the class).
// Shape: the CCharString accessors are null-case-first and the length (rightmost argument) is read first; the CRC key is
//   kept in edi and copied into a search-key temporary homed in the dead `name` argument slot (`mov [esp+14h],edi`, the
//   address goes to LowerBound, the compare uses the register: the GetInterface pattern); the inline Find is the
//   `if (it == last || key < it->key) it = last;` (unsigned: `jae`) and the caller re-reads `end` (+0x7c) for its own
//   `== end` test, returning `&it->second` (`add eax,4`).
struct CCharStringData {
    const char*   m_pChars;                        // +0x00
    unsigned long m_length;                        // +0x04
};
extern const char g_emptyLiteral_0129aaf4[];       // 0x0129AAF4: ""
struct CCharString {
    CCharStringData* m_pStringData;
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
    const char* GetChars() const { return m_pStringData == 0 ? g_emptyLiteral_0129aaf4 : m_pStringData->m_pChars; }
};
unsigned long __fastcall Calc_004014a0(unsigned long seed, const char* data, unsigned long length);   // 0x004014A0: CRC

struct CFaction;
struct SCountBlock;
struct CCountedPointer_CFaction {
    CFaction*    m_pObject;
    SCountBlock* m_pCount;
};

struct CFactionMap {                               // CVectorMap<unsigned long, CCountedPointer<CFaction>>
    struct SEntry { unsigned long key; CCountedPointer_CFaction second; };
    SEntry* first;                                 // +0x00
    SEntry* last;                                  // +0x04
    SEntry* endOfStorage;                          // +0x08
    SEntry* LowerBound(const unsigned long* key);  // 0x009B2290
    SEntry* end() const { return last; }
    SEntry* Find(unsigned long key)
    {
        unsigned long searchKey = key;
        SEntry* it = LowerBound(&searchKey);
        if (it == last || key < it->key)
            it = last;
        return it;
    }
};

struct CFactionDatabase {
    unsigned char _pad0[0x78];
    CFactionMap   m_factions;                      // +0x78
    CCountedPointer_CFaction* FindFactionBySymbolName(const CCharString& name);
};

CCountedPointer_CFaction* CFactionDatabase::FindFactionBySymbolName(const CCharString& name)
{
    CFactionMap::SEntry* it = m_factions.Find(Calc_004014a0(0, name.GetChars(), name.GetLength()));
    if (it == m_factions.end())
        return 0;
    return &it->second;
}
