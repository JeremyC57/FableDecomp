// CParentDefClassBase::RemoveSubDef(const CCharString&) -- erase the sub-def map range keyed by CRC(name) (LowerBound/UpperBound + __copy over the tail), then sort the map if dirty
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B0170, length 138 bytes (padded to 144).
// Leaf: RemoveSubDef
// Callees: Calc @ 0x004014A0, CVectorMap::LowerBound @ 0x009B20D0, CVectorMap::UpperBound @ 0x009B2190,
// STLport __copy(first, last, result, random_access_iterator_tag, ptrdiff_t*) @ 0x009B1620 (manifest label
// "_Copy_impl<CGenerationPos*>" is a byte-twin), the map's sort @ 0x009B84C0.
// Shape: the inline Remove takes the key by value (`__forceinline`), names the UpperBound result before the
// __copy pushes, caches the old end in ebx; Dirty sort as in AddSubDef.
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
struct CSubDefPairVector {
    CSubDefPair* m_first;
    CSubDefPair* m_last;
    CSubDefPair* m_end;
};
CSubDefPair* __fastcall __copy(CSubDefPair* first, CSubDefPair* last, CSubDefPair* result,
                               const random_access_iterator_tag&, int*);   // 0x009B1620
void* FindRef_free___copy_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) CSubDefPair* __fastcall __copy(CSubDefPair* first, CSubDefPair* last, CSubDefPair* result, const random_access_iterator_tag& p3, int* p4)
{
    void* p = FindRef_free___copy_1();
    if (!p)
        return 0;
    *(void**)first = p;
    *(void**)last = p;
    *(void**)result = p;
    return 0;
}
#pragma auto_inline(on)
void __fastcall SortSubDefPairs(CSubDefPair* first, CSubDefPair* last, CKeyPairCompareLess comp);   // 0x009B84C0
void* FindRef_free_SortSubDefPairs_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) void __fastcall SortSubDefPairs(CSubDefPair* first, CSubDefPair* last, CKeyPairCompareLess comp)
{
    void* p = FindRef_free_SortSubDefPairs_0();
    if (!p)
        return;
    *(void**)first = p;
    *(void**)last = p;
    
}
#pragma auto_inline(on)
struct CSubDefInfoMap {
    CSubDefPairVector   Data;               // +0x00
    CKeyPairCompareLess Compare;            // +0x0c
    bool                Dirty;              // +0x0d
    CSubDefPair* LowerBound(const unsigned long* key);   // 0x009B20D0
    CSubDefPair* UpperBound(const unsigned long* key);   // 0x009B2190
    __forceinline void Remove(unsigned long key)
    {
        CSubDefPair* first = LowerBound(&key);
        CSubDefPair* last = Data.m_last;
        if (first != last) {
            CSubDefPair* upper = UpperBound(&key);
            Data.m_last = __copy(upper, last, first, random_access_iterator_tag(), (int*)0);
        }
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
    void RemoveSubDef(const CCharString& name);
};
#pragma pack(pop)

void CParentDefClassBase::RemoveSubDef(const CCharString& name)
{
    SubDefInfoMap.Remove(Calc_004014a0(0, name.GetPString(), name.GetLength()));
    SubDefInfoMap.Sort();
}