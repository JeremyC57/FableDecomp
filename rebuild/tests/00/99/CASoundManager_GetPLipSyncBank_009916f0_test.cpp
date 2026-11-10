#include <cstdio>
struct SSymbolValueRecord { long m_flags; long m_symbol; long m_reserved08; long m_value; unsigned char _pad10[0x14]; };
class CASoundManager { public:
    SSymbolValueRecord* m_pFirst; SSymbolValueRecord* m_pLast;
    long GetPLipSyncBank(long symbol) const
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
    CASoundManager m; m.m_pFirst = r; m.m_pLast = r + 3;
    if (m.GetPLipSyncBank(101) != 15 || m.GetPLipSyncBank(100) != 5 || m.GetPLipSyncBank(102) != 0) { std::printf("009916f0_TEST FAIL\n"); return 1; }
    m.m_pLast = r; if (m.GetPLipSyncBank(100) != 0) { std::printf("009916f0_TEST FAIL\n"); return 1; }
    std::printf("009916f0_TEST PASS\n");
    return 0;
}