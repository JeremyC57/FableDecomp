// CParentDefClassBase::GetPSubDefInfo(const CCharString&) -- CRC the name, look it up in the sub-def map, return the mutable info record or null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AD510, length 98 bytes (padded to 112). Vtable 0x01230CBC slot 15.
// Leaf: GetPSubDefInfo
// Callees: Calc @ 0x004014A0 (CRC-32 kernel, `__fastcall(seed, data, length)`; the empty literal 0x0129AAF4 for a
// string without a data block), CVectorMap<unsigned long, CSubDefInfo>::LowerBound(const unsigned long*) @ 0x009B20D0 (the non-const overload).
// Layout: retail CParentDefClassBase is 0x39 bytes (PDB 0x3d): SubDefInfoMap at +0x29 is a 12-byte STLport vector of
// 12-byte {key, CSubDefInfo} pairs plus Compare +0x0c / Dirty +0x0d. The engine header carries the PDB 16-byte vector,
// so the class is modelled locally (packed, polymorphic).
// Shape: the inline CRC call (GetLength() then GetPString(), each with its own null guard); the map's inline Find takes
// the key BY VALUE and passes the address of a COPY (`unsigned long k = key`), so the key stays in edi for the
// `key < it->first` test while the copy is homed in the dead name slot; the end pointer is re-read for the caller's
// `it != end()` test; the found arm returns &it->second (+4).
// ---- shared model (CParentDefClassBase sub-def map) ----
struct CCharStringData {
    const char*   m_pData;                  // +0 (CBasicString<char>::PData)
    unsigned long m_length;                 // +4 (StringLength)
};
struct CCharString {
    CCharStringData* m_pData;
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }   // "" = the shared empty literal 0x0129AAF4
};
unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC-32 kernel

struct CSubDefInfo {                        // CDefClassBase::CSubDefInfo (8 bytes)
    long DefIndex;                          // +0
    long OriginalParentDefIndex;            // +4
};
struct CSubDefPair {                        // std::pair<unsigned long, CSubDefInfo> (12 bytes)
    unsigned long first;
    CSubDefInfo   second;
};
struct CKeyPairCompareLess { };            // empty comparator, passed by value
struct __true_type { __true_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };
struct CSubDefInfoMap {                     // CVectorMap<unsigned long, CSubDefInfo, CKeyPairCompareLess> (retail 16 bytes)
    CSubDefPair* m_first;                   // +0x00
    CSubDefPair* m_last;                    // +0x04
    CSubDefPair* m_end;                     // +0x08
    CKeyPairCompareLess Compare;            // +0x0c
    bool         Dirty;                     // +0x0d
    CSubDefPair* LowerBound(const unsigned long* key);   // 0x009B20D0
    CSubDefPair* end() { return m_last; }
    CSubDefPair* Find(unsigned long key)
    {
        unsigned long k = key;
        CSubDefPair* it = LowerBound(&k);
        return (it == m_last || key < it->first) ? m_last : it;
    }
};
#pragma pack(push, 1)
struct CParentDefClassBase {
    virtual void v00();
    unsigned char  _defClassBase[0x21];     // +0x04 CDefClassBase part
    long           InstantiationName;       // +0x25
    CSubDefInfoMap SubDefInfoMap;           // +0x29
    CSubDefInfo* GetPSubDefInfo(const CCharString& name);
};
#pragma pack(pop)

CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString& name)
{
    unsigned long key = Calc_004014a0(0, name.GetPString(), name.GetLength());
    CSubDefPair* it = SubDefInfoMap.Find(key);
    if (it != SubDefInfoMap.end())
        return &it->second;
    return 0;
}
