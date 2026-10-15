#include <cstdio>
// Behaviour test for CTCTavernGameCardBlackjack::HasGoneBust (0x007DA8B0).
struct CTavernCard { unsigned char _pad[8]; long m_value; };
struct CCardHand { CTavernCard** m_first; CTavernCard** m_last; CTavernCard** m_endOfStorage; };
struct CTCTavernGameCardBlackjack {
    unsigned char _pad[0x33c];
    CCardHand* m_pCurrentHand;
    long HasGoneBust();
};
long CTCTavernGameCardBlackjack::HasGoneBust()
{
    CCardHand* hand = m_pCurrentHand;
    long total = 0;
    long aces = 0;
    for (CTavernCard** it = hand->m_first; it != hand->m_last; ++it) {
        long value = (*it)->m_value;
        if (value == 11)
            ++aces;
        total += value;
    }
    while (total > 21 && aces > 0) {
        total -= 10;
        --aces;
    }
    return total > 21;
}
static long Score(CTCTavernGameCardBlackjack& g, CCardHand& h, const long* v, int n, CTavernCard* cards, CTavernCard** ptrs)
{
    for (int i = 0; i < n; ++i) { cards[i].m_value = v[i]; ptrs[i] = &cards[i]; }
    h.m_first = ptrs; h.m_last = ptrs + n; h.m_endOfStorage = ptrs + n;
    g.m_pCurrentHand = &h;
    return g.HasGoneBust();
}
int main()
{
    static CTCTavernGameCardBlackjack g;
    CCardHand h; CTavernCard cards[8]; CTavernCard* ptrs[8];
    const long a[] = { 10, 10, 2 };        // 22 -> bust
    const long b[] = { 11, 11, 10 };       // 32 -> 22 -> 12: not bust
    const long c[] = { 11, 10 };           // 21: not bust
    const long d[] = { 11, 10, 5, 7 };     // 33 -> 23: bust
    if (Score(g, h, a, 3, cards, ptrs) != 1 || Score(g, h, b, 3, cards, ptrs) != 0
        || Score(g, h, c, 2, cards, ptrs) != 0 || Score(g, h, d, 4, cards, ptrs) != 1
        || Score(g, h, a, 0, cards, ptrs) != 0) { std::printf("007da8b0_TEST FAIL\n"); return 1; }
    std::printf("007da8b0_TEST PASS\n");
    return 0;
}
