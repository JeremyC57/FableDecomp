#include <cstdio>
// Behaviour test for CTCTavernGameCardPairs::vfunc_57 (0x007D5F90): a copy of the model and the logic.
struct CTCTavernGameCardPairs {
    unsigned char _pad[0x300];
    long m_numTurnedCards;
    long m_turnedCard;
    long vfunc_57();
};

long CTCTavernGameCardPairs::vfunc_57()
{
    long card = 0;
    switch (m_numTurnedCards) {
    case 1:
        card = m_turnedCard;
        break;
    }
    return card;
}

int main()
{
    static CTCTavernGameCardPairs g;
    g.m_turnedCard = 42;
    g.m_numTurnedCards = 1;
    if (g.vfunc_57() != 42) { std::printf("007d5f90_TEST FAIL\n"); return 1; }
    g.m_numTurnedCards = 0;
    if (g.vfunc_57() != 0) { std::printf("007d5f90_TEST FAIL\n"); return 1; }
    g.m_numTurnedCards = 2;
    if (g.vfunc_57() != 0) { std::printf("007d5f90_TEST FAIL\n"); return 1; }
    std::printf("007d5f90_TEST PASS\n");
    return 0;
}
