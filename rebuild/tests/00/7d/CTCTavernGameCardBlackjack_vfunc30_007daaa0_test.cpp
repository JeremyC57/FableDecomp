#include <cstdio>
#include <string.h>
// Behaviour test for CTCTavernGameCardBlackjack::vfunc_30 (0x007DAAA0).
struct CTavernCard { long v; };
struct CCardVector {
    CTavernCard** m_first; CTavernCard** m_last; CTavernCard** m_endOfStorage;
    static CTavernCard** CopyDown(CTavernCard** src, CTavernCard** srcEnd, CTavernCard** dest)
    {
        if (src == srcEnd)
            return dest;
        unsigned long bytes = (char*)srcEnd - (char*)src;
        return (CTavernCard**)((char*)memmove(dest, src, bytes) + bytes);
    }
    void Erase(CTavernCard** eraseFirst, CTavernCard** eraseLast) { m_last = CopyDown(eraseLast, m_last, eraseFirst); }
    void Clear() { Erase(m_first, m_last); }
};
static int g_base = 0;
struct CTCTavernGameCardBase { unsigned char _pad[0x318]; void LeaveGame() { ++g_base; } };
struct CTCTavernGameCardBlackjack : CTCTavernGameCardBase { CCardVector m_cards; void vfunc_30(); };
void CTCTavernGameCardBlackjack::vfunc_30()
{
    CTCTavernGameCardBase::LeaveGame();
    m_cards.Clear();
}
int main()
{
    static CTCTavernGameCardBlackjack g;
    CTavernCard c[3]; CTavernCard* store[4] = { &c[0], &c[1], &c[2], 0 };
    g.m_cards.m_first = store; g.m_cards.m_last = store + 3; g.m_cards.m_endOfStorage = store + 4;
    g.vfunc_30();
    if (g_base != 1 || g.m_cards.m_last != store || g.m_cards.m_first != store || g.m_cards.m_endOfStorage != store + 4)
    { std::printf("007daaa0_TEST FAIL\n"); return 1; }
    // erase a middle range through the same helper: tail moves down
    g.m_cards.m_last = store + 3;
    g.m_cards.Erase(store, store + 1);
    if (g.m_cards.m_last != store + 2 || store[0] != &c[1] || store[1] != &c[2]) { std::printf("007daaa0_TEST FAIL\n"); return 1; }
    std::printf("007daaa0_TEST PASS\n");
    return 0;
}