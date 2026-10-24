// Behaviour test for 0x009E5170 CSymbolMap::FindSymbol(const CCharString&, long&).
#include <cstdio>
#include <cstring>

struct CCharString {
    const char* m_pText;
    bool FastCompare(const CCharString& other) const { return std::strcmp(m_pText, other.m_pText) < 0; }
};
struct CSymbolInfo { CCharString GroupName; CCharString EntryName; long Val; };
struct SSymbolPair { CCharString first; CSymbolInfo second; };

struct CSymbolVectorMap {
    SSymbolPair* m_pFirst;
    SSymbolPair* m_pLast;
    SSymbolPair* m_pEnd;
    SSymbolPair* LowerBound(const CCharString& key) const
    {
        SSymbolPair* it = m_pFirst;
        while (it != m_pLast && it->first.FastCompare(key)) ++it;
        return it;
    }
    SSymbolPair* end() const { return m_pLast; }
    SSymbolPair* Find(const CCharString& key) const
    {
        SSymbolPair* it = LowerBound(key);
        SSymbolPair* last = end();
        return (it == last || key.FastCompare(it->first)) ? last : it;
    }
};

struct CSymbolMap {
    void* _padding_;
    CSymbolVectorMap SLONGMap;
    bool FindSymbol(const CCharString& name, long& value) const
    {
        SSymbolPair* it = SLONGMap.Find(name);
        if (it != SLONGMap.end()) {
            value = it->second.Val;
            return true;
        }
        return false;
    }
};

int main()
{
    SSymbolPair pairs[3];
    const char* names[3] = { "ALPHA", "BETA", "GAMMA" };
    for (int i = 0; i < 3; ++i) {
        pairs[i].first.m_pText = names[i];
        pairs[i].second.GroupName.m_pText = "G";
        pairs[i].second.EntryName.m_pText = names[i];
        pairs[i].second.Val = 100 + i;
    }
    CSymbolMap map;
    map._padding_ = 0;
    map.SLONGMap.m_pFirst = pairs; map.SLONGMap.m_pLast = pairs + 3; map.SLONGMap.m_pEnd = pairs + 3;
    CCharString beta = { "BETA" }, delta = { "DELTA" }, zeta = { "ZETA" }, alpha = { "ALPHA" };
    long v = -1;
    bool ok = map.FindSymbol(beta, v) && v == 101;
    v = -1;
    ok = ok && !map.FindSymbol(delta, v) && v == -1 && !map.FindSymbol(zeta, v) && v == -1;
    ok = ok && map.FindSymbol(alpha, v) && v == 100;
    if (!ok) { std::printf("009e5170_TEST FAIL\n"); return 1; }
    std::printf("009e5170_TEST PASS\n");
    return 0;
}