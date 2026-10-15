// Behaviour test for CTCTavernGameCardBlackjack::EndRound (0x007DAAE0): base EndRound, dealer cards and bets cleared through
// erase(begin, end) with the early-out copy-down, win state reset, game state 8.
#include <cstdio>
#include <cstring>
template <class T> struct Vec {
    T* m_first; T* m_last; T* m_end;
    static T* CopyDown(T* src, T* srcEnd, T* dest)
    {
        if (src == srcEnd) return dest;
        unsigned int count = (unsigned int)((const char*)srcEnd - (const char*)src);
        return (T*)((char*)std::memmove(dest, src, count) + count);
    }
    void erase(T* f, T* l) { m_last = CopyDown(l, m_last, f); }
    void clear() { erase(m_first, m_last); }
};
static int g_baseEnd; static long g_state;
struct Game {
    Vec<int*> DealersCards; Vec<long> Bets; long WinState;
    void BaseEndRound() { ++g_baseEnd; }
    void SetState(long s) { g_state = s; }
    void EndRound() { BaseEndRound(); DealersCards.clear(); Bets.clear(); WinState = 0; SetState(8); }
};
int main()
{
    int* cards[4]; long bets[3] = { 5, 6, 7 };
    Game g; g.DealersCards.m_first = cards; g.DealersCards.m_last = cards + 4; g.DealersCards.m_end = cards + 4;
    g.Bets.m_first = bets; g.Bets.m_last = bets + 3; g.Bets.m_end = bets + 3; g.WinState = 3;
    g.EndRound();
    if (g_baseEnd != 1 || g.DealersCards.m_last != cards || g.Bets.m_last != bets || g.WinState != 0 || g_state != 8) { std::printf("007daae0_TEST FAIL\n"); return 1; }
    std::printf("007daae0_TEST PASS\n");
    return 0;
}
