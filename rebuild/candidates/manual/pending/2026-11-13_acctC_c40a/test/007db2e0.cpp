// Behaviour test for 0x007DB2E0 CTCTavernGameCardBlackjack::NewRound: after the base round reset and a shuffle, the player's hands are
// cleared to ONE empty hand, the pack index restarts at 0 and four cards are dealt from the top of the pack -- the first two to the
// dealer, the next two to the player's first hand -- each also appended to the moving-cards list; then the moving-card cursor is -1, the
// dealing target is the dealer's hand, the current player hand is hand 0, both hand cursors and the newly dealt card are cleared, the
// split / finished / dealer-started / doubled flags are reset and the game enters state 19 (deal the dealer's cards).
#include <cstdio>
#include <vector>
typedef int Card;
struct Game {
    std::vector<Card*> cards, pack, dealer; std::vector<std::vector<Card*> > hands;
    std::vector<Card*>* currentHand; std::vector<Card*>* dealing; Card* newly;
    long handIndex, dealIndex, packIndex, moving, state; bool split, finished, dealerStarted, doubled;
    int baseRounds, shuffles;
    void NewRound() {
        ++baseRounds; ++shuffles;
        hands.clear(); hands.push_back(std::vector<Card*>());
        packIndex = 0;
        for (int i = 0; i < 2; ++i) { cards.push_back(pack[packIndex]); dealer.push_back(pack[packIndex++]); }
        for (int i = 0; i < 2; ++i) { cards.push_back(pack[packIndex]); hands[0].push_back(pack[packIndex++]); }
        moving = -1; dealing = &dealer; currentHand = &hands[0]; handIndex = 0; dealIndex = 0; newly = 0;
        split = false; finished = false; dealerStarted = false; doubled = false; state = 19;
    }
};
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
int main() {
    static Card deck[6]; Game g; g.baseRounds = 0; g.shuffles = 0; g.state = 0;
    for (int i = 0; i < 6; ++i) g.pack.push_back(&deck[i]);
    g.hands.resize(3); g.hands[1].push_back(&deck[5]);                    // stale split hands from the last round
    g.packIndex = 5; g.moving = 3; g.handIndex = 2; g.dealIndex = 1; g.newly = &deck[4]; g.split = g.finished = g.dealerStarted = g.doubled = true;
    g.NewRound();
    check(g.baseRounds == 1 && g.shuffles == 1, "base NewRound and ShuffleCards run once");
    check(g.hands.size() == 1 && g.hands[0].size() == 2, "one player hand with two cards");
    check(g.dealer.size() == 2 && g.dealer[0] == &deck[0] && g.dealer[1] == &deck[1], "dealer gets pack cards 0 and 1");
    check(g.hands[0][0] == &deck[2] && g.hands[0][1] == &deck[3], "player gets pack cards 2 and 3");
    check(g.cards.size() == 4 && g.cards[0] == &deck[0] && g.cards[1] == &deck[1] && g.cards[2] == &deck[2] && g.cards[3] == &deck[3], "all four join the moving cards in deal order");
    check(g.packIndex == 4, "pack index is past the four dealt cards");
    check(g.moving == -1 && g.dealing == &g.dealer && g.currentHand == &g.hands[0], "cursors point at the dealer's hand and player hand 0");
    check(g.handIndex == 0 && g.dealIndex == 0 && g.newly == 0, "hand cursors and newly dealt card cleared");
    check(!g.split && !g.finished && !g.dealerStarted && !g.doubled && g.state == 19, "flags reset, state 19");
    if (!fails) std::printf("007db2e0_TEST PASS\n");
    return fails;
}
