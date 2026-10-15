#include <cstdio>
#include <new>
#include <string.h>
// Behaviour test for CTCTavernGameCardBlackjack::CTCTavernGameCardBlackjack (0x007DB280).
class CThing { public: int id; };
struct CTavernCard;
struct CCardVector {
    CTavernCard** m_first; CTavernCard** m_last; CTavernCard** m_endOfStorage;
    CCardVector() : m_first(0), m_last(0), m_endOfStorage(0) {}
};
static CThing* g_owner = 0;
struct CTCTavernGameCardBase {
    CTCTavernGameCardBase(CThing* owner) { g_owner = owner; }
    virtual void v00() {}
    virtual ~CTCTavernGameCardBase() {}
    unsigned char _pad_0x04[0x314];
};
struct CTCTavernGameCardBlackjack : CTCTavernGameCardBase {
    CCardVector m_playerCards; CCardVector m_dealerCards; CCardVector m_deck; CCardVector* m_pCurrentHand;
    CTCTavernGameCardBlackjack(CThing* owner);
    virtual void v00() {}
};
CTCTavernGameCardBlackjack::CTCTavernGameCardBlackjack(CThing* owner)
    : CTCTavernGameCardBase(owner)
{
}
int main()
{
    static unsigned char raw[sizeof(CTCTavernGameCardBlackjack)];
    memset(raw, 0xAB, sizeof(raw));
    CThing t; t.id = 1;
    CTCTavernGameCardBlackjack* g = new (raw) CTCTavernGameCardBlackjack(&t);
    if (g_owner != &t || g->m_playerCards.m_first || g->m_playerCards.m_last || g->m_playerCards.m_endOfStorage
        || g->m_dealerCards.m_first || g->m_dealerCards.m_last || g->m_dealerCards.m_endOfStorage
        || g->m_deck.m_first || g->m_deck.m_last || g->m_deck.m_endOfStorage)
    { std::printf("007db280_TEST FAIL\n"); return 1; }
    std::printf("007db280_TEST PASS\n");
    return 0;
}