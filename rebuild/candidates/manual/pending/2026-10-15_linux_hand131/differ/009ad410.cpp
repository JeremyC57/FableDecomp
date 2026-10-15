// CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) const -- the global def index of a def name: through the name-CRC map when it is populated, otherwise by asking the definition symbol map; 0 for an empty name or an unknown one
// Grade: DIFFER(135v141) -- the lane homes the fallback's out-value `index` in the dead `name` argument slot where retail gives it a push-ecx frame word (so no `push ecx`/`pop ecx`), and colours this/last/map/key as ebp/ebx/edi/esi where retail has ebx/ebp/esi/edi
// Leaf: GetDefGlobalIndexFromName
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AD410, length 141 bytes.
// Callees: Calc @ 0x004014A0 (`__fastcall(0, chars, length)`, the name's CRC), CVectorMap<unsigned long, long>::LowerBound
// (const unsigned long&) @ 0x009B21F0 (manifest names the CDefString instantiation; identical bodies), CSymbolMap lookup
// (name, long* out) @ 0x009E5170 (manifest "SymbolMap_Lookup").
// Layout (Ego_r.pdb with retail 12-byte vectors): PDefinitionSymbolMap (CCountedPointer<CSymbolMap>) object at +0x60,
// NameCRCToIndexMap (CVectorMap<unsigned long, long>: first +0x68, last +0x6c) -- modelled locally.
// Shape: the empty-name test is the two-step accessor (null block, then zero length) with duplicated `return 0` tails;
// the map's end is captured in a local before the empty test and reused by the inlined find after the CRC and lower-bound
// calls (ebp), while the final hit test re-reads the member; the CRC key is a signed `long` so LowerBound's const-ref binds a
// conversion temporary (homed in the dead `name` slot) and the compare keeps the register copy; the misses share the final
// `return 0`; the fallback's out-value lives in the push-ecx frame word in retail.
struct CCharStringData {
    const char*   m_pChars;                         // +0x00
    unsigned long m_length;                         // +0x04
};
extern const char g_emptyLiteral_0129aaf4[];        // 0x0129AAF4: ""
struct CCharString {
    CCharStringData* m_pStringData;
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
    const char* GetChars() const { return m_pStringData == 0 ? g_emptyLiteral_0129aaf4 : m_pStringData->m_pChars; }
};
unsigned long __fastcall Calc_004014a0(unsigned long seed, const char* data, unsigned long length);   // 0x004014A0: CRC

struct SNameIndex {                                 // pair<unsigned long, long>
    unsigned long first;
    long          second;
};
struct CNameCRCToIndexMap {                         // CVectorMap<unsigned long, long>: sorted vector + compare + dirty
    SNameIndex* m_pFirst;
    SNameIndex* m_pLast;
    SNameIndex* m_pEnd;
    SNameIndex* LowerBound(const unsigned long& key);   // 0x009B21F0
};

struct CSymbolMap {
    bool Lookup(const CCharString& name, long* value);  // 0x009E5170
};

struct CDefinitionManager {
    unsigned char      _pad00[0x60];
    CSymbolMap*        PDefinitionSymbolMap;        // +0x60 CCountedPointer<CSymbolMap>::Object
    void*              PDefinitionSymbolMapInfo;    // +0x64
    CNameCRCToIndexMap NameCRCToIndexMap;           // +0x68
    long GetDefGlobalIndexFromName(const CCharString& name);
};

long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name)
{
    if (name.GetLength() == 0)
        return 0;
    CNameCRCToIndexMap* map = &NameCRCToIndexMap;
    SNameIndex* last = map->m_pLast;
    if (map->m_pFirst != last) {
        long key = Calc_004014a0(0, name.GetChars(), name.GetLength());
        SNameIndex* it = map->LowerBound(key);      // a conversion temporary is bound to the reference
        if (it == last || key < it->first)
            it = last;
        if (it != NameCRCToIndexMap.m_pLast)
            return it->second;
    }
    else {
        long index;
        if (PDefinitionSymbolMap->Lookup(name, &index))
            return index;
    }
    return 0;
}
