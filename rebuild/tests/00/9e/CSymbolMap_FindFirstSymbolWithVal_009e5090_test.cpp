#include <cstdio>
struct CCharStringData;
struct CCharString { CCharStringData* m_pStringData; };
struct CSymbolInfo { CCharString GroupName; CCharString EntryName; long Val; };
struct SSymbolPair { CCharString first; CSymbolInfo second; };
struct CSymbolVectorMap { SSymbolPair* m_pFirst; SSymbolPair* m_pLast; SSymbolPair* m_pEnd; };
struct CSymbolMap {
    void* _padding_; CSymbolVectorMap SLONGMap;
    const CSymbolInfo* FindFirstSymbolWithVal(long value) const
    {
        for (const SSymbolPair* it = SLONGMap.m_pFirst; it != SLONGMap.m_pLast; ++it) {
            if (it->second.Val == value)
                return &it->second;
        }
        return 0;
    }
};
int main()
{
    SSymbolPair e[3];
    e[0].second.Val = 3; e[1].second.Val = 9; e[2].second.Val = 9;
    CSymbolMap m; m.SLONGMap.m_pFirst = e; m.SLONGMap.m_pLast = e + 3; m.SLONGMap.m_pEnd = e + 3;
    if (m.FindFirstSymbolWithVal(9) != &e[1].second || m.FindFirstSymbolWithVal(3) != &e[0].second || m.FindFirstSymbolWithVal(4) != 0) { std::printf("009e5090_TEST FAIL\n"); return 1; }
    m.SLONGMap.m_pLast = e; if (m.FindFirstSymbolWithVal(3) != 0) { std::printf("009e5090_TEST FAIL\n"); return 1; }
    std::printf("009e5090_TEST PASS\n");
    return 0;
}