#include <cstdio>
// Behaviour model of CTCTavernGameCardBlackjack::vfunc_53 (0x007DA120): without a split, the dealer's second card and the player's
// first two cards are turned (with the flip sound) and the state becomes 0x16; after a split the state becomes 0x17.
struct Card { bool up; };
static long Run(bool split, Card* dealer, Card* player, int& sounds)
{
    if (!split) {
        sounds++;
        dealer[1].up = !dealer[1].up; player[0].up = !player[0].up; player[1].up = !player[1].up;
        return 0x16;
    }
    return 0x17;
}
int main()
{
    Card dealer[2] = { { true }, { false } }, player[2] = { { false }, { false } };
    int sounds = 0;
    long s1 = Run(false, dealer, player, sounds), s2 = Run(true, dealer, player, sounds);
    if (s1 != 0x16 || s2 != 0x17 || sounds != 1 || !dealer[1].up || !player[0].up || !player[1].up || !dealer[0].up) { std::printf("007da120_TEST FAIL\n"); return 1; }
    std::printf("007da120_TEST PASS\n");
    return 0;
}