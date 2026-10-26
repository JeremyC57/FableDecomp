// CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) const -- the global def index of a def name: through the name-CRC map when it is populated, otherwise by asking the definition symbol map; 0 for an empty name or an unknown one
// Leaf: GetDefGlobalIndexFromName
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AD410, length 141 bytes.
// Callees: Calc @ 0x004014A0 (`__fastcall(seed, chars, length)`, CRC-32 over the table at 0x0129A168),
// CVectorMap<unsigned long, long>::LowerBound(const unsigned long&) @ 0x009B21F0 (manifest names the CDefString
// instantiation; identical bodies), CSymbolMap lookup (name, long* out) @ 0x009E5170 (manifest "SymbolMap_Lookup").
// Layout (Ego_r.pdb with retail 12-byte vectors): PDefinitionSymbolMap (CCountedPointer<CSymbolMap>: Object +0x60,
// Info +0x64), NameCRCToIndexMap (CVectorMap<unsigned long, long>: first +0x68, last +0x6c) -- modelled locally.
// Shape: the empty-name test is the two-step accessor (null block, then zero length) with duplicated `return 0` tails;
// the fallback's out-value `index` is declared at function scope, so it gets its own push-ecx frame word instead of
// sharing the dead `name` slot with the CRC key's conversion temporary. The map test is the inline empty(), and the
// lookup is the inline find(const unsigned long&): LowerBound, then `e = m_pLast` and the miss test that returns e.
// Calc and LowerBound are LTCG-compiled in retail, so the caller knows neither writes memory: the pre-CRC m_pLast load
// is reused for e (a `mov esi, ebp` copy after the call) and the compare keeps the key in edi rather than re-reading the
// temporary. Same-TU noinline stand-ins with the retail algorithms reproduce that knowledge. The final hit test
// re-reads end(). The symbol map is reached through CCountedPointer::operator->, which loads the object before the
// arguments are pushed.
class CCharStringData { public:
    const char*   m_pChars;                         // +0x00
    unsigned long m_length;                         // +0x04
};
extern const char g_emptyLiteral_0129aaf4[];        // 0x0129AAF4: ""
class CCharString { public:
    CCharStringData* m_pStringData;
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
    const char* GetChars() const { return m_pStringData == 0 ? g_emptyLiteral_0129aaf4 : m_pStringData->m_pChars; }
};
extern const unsigned long g_crcTable_0129a168[256];   // 0x0129A168: CRC-32 table
// Stand-in for Calc @ 0x004014A0 (the retail body; same TU so the caller sees that it writes no memory).
#pragma auto_inline(off)
__declspec(noinline) unsigned long __fastcall Calc_004014a0(unsigned long seed, const char* data, unsigned long length)
{
    unsigned long crc = seed;
    if (length != 0)
        for (unsigned long i = 0; i < length; ++i)
            crc = (crc >> 8) ^ g_crcTable_0129a168[(unsigned char)data[i] ^ (crc & 0xff)];
    return crc;
}
#pragma auto_inline(on)

struct SNameIndex {                                 // pair<unsigned long, long>
    unsigned long first;
    long          second;
};
struct CNameCRCToIndexMap {                         // CVectorMap<unsigned long, long>: sorted vector + compare + dirty
    SNameIndex* m_pFirst;
    SNameIndex* m_pLast;
    SNameIndex* m_pEnd;
    SNameIndex* LowerBound(const unsigned long& key) const;   // 0x009B21F0
    bool empty() const { return m_pFirst == m_pLast; }
    SNameIndex* end() const { return m_pLast; }
    SNameIndex* find(const unsigned long& key) const
    {
        SNameIndex* i = LowerBound(key);
        SNameIndex* e = m_pLast;
        if (i == e || key < i->first)
            return e;
        return i;
    }
};

// Stand-in for LowerBound @ 0x009B21F0 (the retail binary search; same TU so the caller sees that it writes no memory).
#pragma auto_inline(off)
__declspec(noinline) SNameIndex* CNameCRCToIndexMap::LowerBound(const unsigned long& key) const
{
    SNameIndex* first = m_pFirst;
    long len = m_pLast - first;
    while (len > 0) {
        long half = len / 2;
        if (first[half].first < key) { first += half + 1; len -= half + 1; }
        else len = half;
    }
    return first;
}
#pragma auto_inline(on)
class CSymbolMap { public:
    bool Lookup(const CCharString& name, long* value) const;  // 0x009E5170
};

struct CSymbolMapPointer {                          // CCountedPointer<CSymbolMap>
    CSymbolMap* Object;                             // +0x00
    void*       Info;                               // +0x04
    CSymbolMap* operator->() const { return Object; }
};

class CDefinitionManager { public:
    unsigned char      _pad00[0x60];
    CSymbolMapPointer  PDefinitionSymbolMap;        // +0x60 CCountedPointer<CSymbolMap>
    CNameCRCToIndexMap NameCRCToIndexMap;           // +0x68
    long GetDefGlobalIndexFromName(const CCharString& name) const;
};

long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) const
{
    long index;
    if (name.GetLength() == 0)
        return 0;
    if (!NameCRCToIndexMap.empty()) {
        long key = Calc_004014a0(0, name.GetChars(), name.GetLength());
        SNameIndex* it = NameCRCToIndexMap.find(key);
        if (it != NameCRCToIndexMap.end())
            return it->second;
    }
    else {
        if (PDefinitionSymbolMap->Lookup(name, &index))
            return index;
    }
    return 0;
}