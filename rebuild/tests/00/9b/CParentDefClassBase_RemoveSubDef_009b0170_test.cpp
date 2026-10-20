// Behaviour test for 009b0170: body copied from src/009b0170.cpp (CParentDefClassBase::RemoveSubDef), callees stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CCharStringData { const char* m_pData; unsigned long m_length; };
struct CCharString {
    CCharStringData* m_pData;
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }
};
static int g_crc = 0; static const void* g_crcData = 0; static unsigned long g_crcLen = 0; static unsigned long g_crcResult = 0;
static unsigned long __fastcall Calc_004014a0(unsigned long seed, const void* data, unsigned long length)
{ ++g_crc; CHECK(seed == 0); g_crcData = data; g_crcLen = length; return g_crcResult; }
struct CSubDefInfo { long DefIndex; long OriginalParentDefIndex; };
struct CSubDefPair { unsigned long first; CSubDefInfo second; };
struct CKeyPairCompareLess { };
struct __true_type { __true_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };
struct CSubDefPairVector { CSubDefPair* m_first; CSubDefPair* m_last; CSubDefPair* m_end; };
static int g_copy = 0, g_sort = 0;
static CSubDefPair* __fastcall __copy(CSubDefPair* first, CSubDefPair* last, CSubDefPair* result, const random_access_iterator_tag&, int*)
{ ++g_copy; while (first != last) *result++ = *first++; return result; }
static void __fastcall SortSubDefPairs(CSubDefPair* first, CSubDefPair* last, CKeyPairCompareLess comp)
{ ++g_sort; (void)comp; for (CSubDefPair* i = first; i != last; ++i) for (CSubDefPair* j = i + 1; j != last; ++j) if (j->first < i->first) { CSubDefPair t = *i; *i = *j; *j = t; } }
static int g_lower = 0, g_upper = 0;
struct CSubDefInfoMap {
    CSubDefPairVector   Data;
    CKeyPairCompareLess Compare;
    bool                Dirty;
    CSubDefPair* LowerBound(const unsigned long* key) { ++g_lower; CSubDefPair* p = Data.m_first; while (p != Data.m_last && p->first < *key) ++p; return p; }
    CSubDefPair* UpperBound(const unsigned long* key) { ++g_upper; CSubDefPair* p = Data.m_first; while (p != Data.m_last && p->first <= *key) ++p; return p; }
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
    unsigned char  _defClassBase[0x21];
    long           InstantiationName;
    CSubDefInfoMap SubDefInfoMap;
    void RemoveSubDef(const CCharString& name);
};
#pragma pack(pop)
void CParentDefClassBase::RemoveSubDef(const CCharString& name)
{
    SubDefInfoMap.Remove(Calc_004014a0(0, name.GetPString(), name.GetLength()));
    SubDefInfoMap.Sort();
}
int main()
{
    CSubDefPair pairs[5]; for (int i = 0; i < 5; ++i) { pairs[i].first = (i == 1 || i == 2) ? 20 : (unsigned long)(i * 10); pairs[i].second.DefIndex = i; }
    // keys: 0, 20, 20, 30, 40
    CParentDefClassBase def; memset(&def, 0, sizeof def);
    def.SubDefInfoMap.Data.m_first = pairs; def.SubDefInfoMap.Data.m_last = pairs + 5; def.SubDefInfoMap.Data.m_end = pairs + 5;
    CCharStringData sd; sd.m_pData = "Sub"; sd.m_length = 3; CCharString name; name.m_pData = &sd;
    g_crcResult = 20; def.SubDefInfoMap.Dirty = false;
    def.RemoveSubDef(name);                                  // removes both key-20 entries, not dirty -> no sort
    CHECK(g_crc == 1 && g_crcData == sd.m_pData && g_crcLen == 3 && g_lower == 1 && g_upper == 1 && g_copy == 1 && g_sort == 0);
    CHECK(def.SubDefInfoMap.Data.m_last == pairs + 3 && pairs[0].second.DefIndex == 0 && pairs[1].second.DefIndex == 3 && pairs[2].second.DefIndex == 4);
    g_crcResult = 99; def.SubDefInfoMap.Dirty = true;
    def.RemoveSubDef(name);                                  // key absent (lower bound == end): no upper bound / copy; dirty -> sorted and cleared
    CHECK(g_lower == 2 && g_upper == 1 && g_copy == 1 && g_sort == 1 && def.SubDefInfoMap.Dirty == false && def.SubDefInfoMap.Data.m_last == pairs + 3);
    CCharString empty; empty.m_pData = 0;                    // null string data: empty literal, length 0
    g_crcResult = 0; def.RemoveSubDef(empty);
    CHECK(g_crc == 3 && g_crcLen == 0 && ((const char*)g_crcData)[0] == 0 && g_copy == 2 && def.SubDefInfoMap.Data.m_last == pairs + 2 && pairs[0].second.DefIndex == 3);
    if (fails == 0) printf("009b0170_TEST PASS\n"); else printf("009b0170_TEST FAIL (%d)\n", fails);
    return fails;
}