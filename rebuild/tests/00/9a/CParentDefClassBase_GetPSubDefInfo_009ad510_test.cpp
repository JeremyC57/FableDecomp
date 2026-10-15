// behaviour test for 0x009AD510 CParentDefClassBase::GetPSubDefInfo: CRC of the name, lower bound, exact-key check, &second or null
#include <cstdio>
#include <cstring>
struct CCharStringData { const char* m_pData; unsigned int m_length; };
struct CCharString {
    CCharStringData* m_pData;
    unsigned int GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetPString() const { return m_pData == 0 ? "" : m_pData->m_pData; }
};
static unsigned int Calc(unsigned int seed, const char* s, unsigned int n) { unsigned int h = seed; for (unsigned int i = 0; i < n; ++i) h = h * 31 + (unsigned char)s[i]; return h; }
struct CSubDefInfo { int DefIndex; int OriginalParentDefIndex; };
struct CSubDefPair { unsigned int first; CSubDefInfo second; };
struct CSubDefInfoMap {
    CSubDefPair* m_first; CSubDefPair* m_last;
     CSubDefPair* LowerBound(const unsigned int* key)  {  CSubDefPair* p = m_first; while (p != m_last && p->first < *key) ++p; return p; }
     CSubDefPair* end()  { return m_last; }
     CSubDefPair* Find(unsigned int key)  { unsigned int k = key;  CSubDefPair* it = LowerBound(&k); return (it == m_last || key < it->first) ? m_last : it; }
};
struct CParentDefClassBase {
    CSubDefInfoMap SubDefInfoMap;
     CSubDefInfo* GetPSubDefInfo(const CCharString& name) 
    {
        unsigned int key = Calc(0, name.GetPString(), name.GetLength());
         CSubDefPair* it = SubDefInfoMap.Find(key);
        if (it != SubDefInfoMap.end()) return &it->second;
        return 0;
    }
};
int main()
{
    CCharStringData a = { "Alpha", 5 }, b = { "Beta", 4 }, c = { "Zeta", 4 };
    CSubDefPair recs[2];
    unsigned int ka = Calc(0, "Alpha", 5), kb = Calc(0, "Beta", 4);
    CSubDefPair pa = { ka < kb ? ka : kb, { 1, 2 } }, pb = { ka < kb ? kb : ka, { 3, 4 } };
    recs[0] = pa; recs[1] = pb;
    CParentDefClassBase o; o.SubDefInfoMap.m_first = recs; o.SubDefInfoMap.m_last = recs + 2;
    CCharString sa = { &a }, sb = { &b }, sc = { &c }, sn = { 0 };
     CSubDefInfo* ra = o.GetPSubDefInfo(sa);
     CSubDefInfo* rb = o.GetPSubDefInfo(sb);
    if (!ra || !rb || ra == rb || o.GetPSubDefInfo(sc) != 0 || o.GetPSubDefInfo(sn) != 0 ||
        (ka < kb ? ra->DefIndex != 1 : ra->DefIndex != 3)) { std::printf("009ad510_TEST FAIL\n"); return 1; }
    std::printf("009ad510_TEST PASS\n");
    return 0;
}