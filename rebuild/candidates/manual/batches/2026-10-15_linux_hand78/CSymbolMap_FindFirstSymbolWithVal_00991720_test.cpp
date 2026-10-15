#include <cstdio>
struct SSymbolValueRecord { long m_flags; long m_symbol; long m_reserved08; long m_value; unsigned char _pad10[0x14]; };
struct CSymbolMap {
    SSymbolValueRecord* m_pFirst; SSymbolValueRecord* m_pLast;
    long FindFirstSymbolWithVal() const
    {
        for (const SSymbolValueRecord* it = m_pFirst; it != m_pLast; ++it) {
            if (it->m_value != 0)
                return it->m_value;
        }
        return 0;
    }
};
int main()
{
    SSymbolValueRecord r[3];
    r[0].m_value = 0; r[1].m_value = 77; r[2].m_value = 88;
    CSymbolMap m; m.m_pFirst = r; m.m_pLast = r + 3;
    if (m.FindFirstSymbolWithVal() != 77) { std::printf("00991720_TEST FAIL\n"); return 1; }
    r[1].m_value = 0; r[2].m_value = 0;
    if (m.FindFirstSymbolWithVal() != 0) { std::printf("00991720_TEST FAIL\n"); return 1; }
    m.m_pLast = r; if (m.FindFirstSymbolWithVal() != 0) { std::printf("00991720_TEST FAIL\n"); return 1; }
    std::printf("00991720_TEST PASS\n");
    return 0;
}
