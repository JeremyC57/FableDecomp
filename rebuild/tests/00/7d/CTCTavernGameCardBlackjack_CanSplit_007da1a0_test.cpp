// Behaviour test for CTCTavernGameCardBlackjack::CanSplit @ 0x007DA1A0: money covers the bet, not split yet, two cards of equal rank.
#include <cstdio>
class CCard { public: long Index; };
struct CVector { CCard** first; CCard** last; unsigned long size() const { return (unsigned long)(last - first); } };
struct CTCTavernGameCardBlackjack {
    long BaseBet, CurrentMoney; CVector* CurrentPlayerCards; bool HasPlayerSplit;
    bool CanSplit()
    {
        if (CurrentMoney >= BaseBet && !HasPlayerSplit && CurrentPlayerCards->size() == 2
            && CurrentPlayerCards->first[0]->Index % 13 == CurrentPlayerCards->first[1]->Index % 13) return true;
        return false;
    }
};
int main()
{
    CCard a, b, c; a.Index = 3; b.Index = 16; c.Index = 4; CCard* hand[3] = { &a, &b, &c };
    CVector v; v.first = hand; v.last = hand + 2;
    CTCTavernGameCardBlackjack g; g.BaseBet = 10; g.CurrentMoney = 10; g.CurrentPlayerCards = &v; g.HasPlayerSplit = false;
    if (!g.CanSplit()) { std::printf("007da1a0_TEST FAIL\n"); return 1; }
    g.CurrentMoney = 9; if (g.CanSplit()) { std::printf("007da1a0_TEST FAIL\n"); return 1; }
    g.CurrentMoney = 50; g.HasPlayerSplit = true; if (g.CanSplit()) { std::printf("007da1a0_TEST FAIL\n"); return 1; }
    g.HasPlayerSplit = false; v.last = hand + 3; if (g.CanSplit()) { std::printf("007da1a0_TEST FAIL\n"); return 1; }
    hand[1] = &c; v.last = hand + 2; if (g.CanSplit()) { std::printf("007da1a0_TEST FAIL\n"); return 1; }
    std::printf("007da1a0_TEST PASS\n");
    return 0;
}