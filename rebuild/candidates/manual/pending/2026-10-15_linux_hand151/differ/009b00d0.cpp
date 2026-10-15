// CParentDefClassBase::AddSubDef(const CCharString&, long, long) -- append {CRC(name), {defIndex, parentIndex}} to the sub-def map (inline STLport push_back), then sort the map if dirty
// Grade: DIFFER(163v155) -- the placement copy of the pair reads `second` back from the frame (`mov edx,[esp+14h] ; mov [ecx+4],edx` x2) where retail stores edi/ebx straight from the parameter registers; everything else matches
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B00D0, length 155 bytes (padded to 160).
// Leaf: AddSubDef
// Callees: Calc @ 0x004014A0 (CRC-32), vector<pair<ulong,CSubDefInfo>>::_M_insert_overflow @ 0x009B60D0,
// the map's sort @ 0x009B84C0 (`__fastcall(first, last, CKeyPairCompareLess)`).
// Layout: retail CParentDefClassBase is 0x39 bytes; SubDefInfoMap at +0x29 = 12-byte vector + Compare +0x0c + Dirty +0x0d.
// Shape: GetPString() written as `s = ""; if (p) s = p->data;` (gives retail's `mov edx,lit ; je ; mov edx,[eax]`);
// Add + push_back inline (`__forceinline`), Dirty stored then re-read by the inline Sort().
// ---- shared model (CParentDefClassBase sub-def map) ----
struct CCharStringData {
    const char*   m_pData;                  // +0 (CBasicString<char>::PData)
    unsigned long m_length;                 // +4 (StringLength)
};
struct CCharString {
    CCharStringData* m_pData;
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { const char* s = ""; if (m_pData) s = m_pData->m_pData; return s; }   // "" = the shared empty literal 0x0129AAF4
};
unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length);   // 0x004014A0: CRC-32 kernel

struct CSubDefInfo {                        // CDefClassBase::CSubDefInfo (8 bytes)
    long DefIndex;                          // +0
    long OriginalParentDefIndex;            // +4
    CSubDefInfo(long def, long parent) : DefIndex(def), OriginalParentDefIndex(parent) {}
};
struct CSubDefPair {                        // std::pair<unsigned long, CSubDefInfo> (12 bytes)
    unsigned long first;
    CSubDefInfo   second;
    CSubDefPair(unsigned long k, const CSubDefInfo& v) : first(k), second(v) {}
};
struct CKeyPairCompareLess { };            // empty comparator, passed by value
struct __true_type { __true_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };
inline void* operator new(unsigned int, void* where) { return where; }
struct CSubDefPairVector {
    CSubDefPair* m_first;
    CSubDefPair* m_last;
    CSubDefPair* m_end;
    void _M_insert_overflow(CSubDefPair* pos, const CSubDefPair& x, const __true_type&, unsigned long n, bool atend);   // 0x009B60D0
    void push_back(const CSubDefPair& x)
    {
        if (m_last != m_end) {
            new (m_last) CSubDefPair(x);
            ++m_last;
        }
        else
            _M_insert_overflow(m_last, x, __true_type(), 1, true);
    }
};
void __fastcall SortSubDefPairs(CSubDefPair* first, CSubDefPair* last, CKeyPairCompareLess comp);   // 0x009B84C0
struct CSubDefInfoMap {
    CSubDefPairVector   Data;               // +0x00
    CKeyPairCompareLess Compare;            // +0x0c
    bool                Dirty;              // +0x0d
    __forceinline void Add(unsigned long key, long def, long parent)
    {
        CSubDefPair p(key, CSubDefInfo(def, parent));
        Data.push_back(p);
        Dirty = true;
    }
    void Sort()
    {
        if (Dirty) {
            SortSubDefPairs(Data.m_first, Data.m_last, Compare);
            Dirty = false;
        }
    }
};
#pragma pack(push, 1)
struct CParentDefClassBase {
    virtual void v00();
    unsigned char  _defClassBase[0x21];     // +0x04 CDefClassBase part
    long           InstantiationName;       // +0x25
    CSubDefInfoMap SubDefInfoMap;           // +0x29
    void AddSubDef(const CCharString& name, long defIndex, long originalParentDefIndex);
};
#pragma pack(pop)

void CParentDefClassBase::AddSubDef(const CCharString& name, long defIndex, long originalParentDefIndex)
{
    SubDefInfoMap.Add(Calc_004014a0(0, name.GetPString(), name.GetLength()), defIndex, originalParentDefIndex);
    SubDefInfoMap.Sort();
}
