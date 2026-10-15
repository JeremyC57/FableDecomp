#include <cstdio>
// Behaviour test for CTCTavernGameCardSorting::vfunc_57 (0x007D4E10): a copy of the model and the logic.
struct CTCTavernGameCardSorting {
    unsigned char _pad[0x300];
    long m_numTurnedCards;
    long m_turnedCard;
    long vfunc_57();
};

long CTCTavernGameCardSorting::vfunc_57()
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
    static CTCTavernGameCardSorting g;
    g.m_turnedCard = 42;
    g.m_numTurnedCards = 1;
    if (g.vfunc_57() != 42) { std::printf("007d4e10_TEST FAIL\n"); return 1; }
    g.m_numTurnedCards = 0;
    if (g.vfunc_57() != 0) { std::printf("007d4e10_TEST FAIL\n"); return 1; }
    g.m_numTurnedCards = 2;
    if (g.vfunc_57() != 0) { std::printf("007d4e10_TEST FAIL\n"); return 1; }
    std::printf("007d4e10_TEST PASS\n");
    return 0;
}
