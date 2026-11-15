// Behaviour test for CTCTavernGameCardBlackjack::CardsMainGame (0x007DB6A0), the round-finished scan and the 0x17 decision: the
// scan skips hands that are bust or still face down and pays a won hand its bet, 1.5x (floored) for a two-card 21 when the player
// has not split; in state 0x17 a bust hand goes to BlackjackBusted (0x26), fewer than two cards deals, a splittable hand asks
// (0x1c / 0x1d), a 21 or a doubled hand moves on to the next hand, else the double / hit choice (0x1d when the money allows a
// double on two cards, else 0x1e). A model.
#include <cstdio>
#include <cmath>
long Payout(long bet, long value, long cards, bool split)
{
    if (value == 21 && cards == 2 && !split) return (long)std::floor(bet * 1.5f);
    return bet;
}
long Decide(long value, long cards, bool canSplit, bool doubled, long money, long baseBet)
{
    if (value > 21) return 0x26;
    if (cards < 2) return -1;
    if (canSplit) return 0x1c;
    if (value != 21 && !doubled) return money >= baseBet && cards == 2 ? 0x1d : 0x1e;
    return 0x23;
}
int main()
{
    bool ok = Payout(5, 21, 2, false) == 7 && Payout(5, 21, 2, true) == 5 && Payout(5, 20, 3, false) == 5 && Payout(10, 21, 2, false) == 15;
    ok = ok && Decide(22, 3, false, false, 10, 5) == 0x26 && Decide(10, 1, false, false, 10, 5) == -1;
    ok = ok && Decide(16, 2, true, false, 10, 5) == 0x1c && Decide(16, 2, false, false, 10, 5) == 0x1d;
    ok = ok && Decide(16, 3, false, false, 10, 5) == 0x1e && Decide(16, 2, false, false, 4, 5) == 0x1e;
    ok = ok && Decide(21, 3, false, false, 10, 5) == 0x23 && Decide(15, 3, false, true, 10, 5) == 0x23;
    std::printf(ok ? "007db6a0_TEST PASS\n" : "007db6a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
