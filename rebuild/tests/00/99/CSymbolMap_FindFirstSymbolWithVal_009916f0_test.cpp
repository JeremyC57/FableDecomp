#include <cstdio>
struct SSymbolValueRecord { long m_flags; long m_symbol; long m_reserved08; long m_value; unsigned char _pad10[0x14]; };
struct CSymbolMap {
    SSymbolValueRecord* m_pFirst; SSymbolValueRecord* m_pLast;
    long FindFirstSymbolWithVal(long symbol) const
    {
        for (const SSymbolValueRecord* it = m_pFirst; it != m_pLast; ++it) {
            if (it->m_symbol == symbol)
                return it->m_value;
        }
        return 0;
    }
};
int main()
{
    SSymbolValueRecord r[3];
    for (int i = 0; i < 3; ++i) { r[i].m_symbol = 100 + i; r[i].m_value = 10 * i + 5; }
    r[2].m_symbol = 101;
    CSymbolMap m; m.m_pFirst = r; m.m_pLast = r + 3;
    if (m.FindFirstSymbolWithVal(101) != 15 || m.FindFirstSymbolWithVal(100) != 5 || m.FindFirstSymbolWithVal(102) != 0) { std::printf("009916f0_TEST FAIL\n"); return 1; }
    m.m_pLast = r; if (m.FindFirstSymbolWithVal(100) != 0) { std::printf("009916f0_TEST FAIL\n"); return 1; }
    std::printf("009916f0_TEST PASS\n");
    return 0;
}