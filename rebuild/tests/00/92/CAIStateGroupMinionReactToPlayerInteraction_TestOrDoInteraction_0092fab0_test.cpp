// Behaviour test for CAIStateGroup_MinionReactToPlayerInteraction::TestOrDoInteraction (0x0092FAB0): a plain model of the
// decision order -- a sober shopkeeper with an open shop and a trading hero answers reaction 3 at once (test mode); otherwise
// the reaction slots fall through in order (gossip 0 unless married, greeting 1 once its frame has passed, opinion greeting 2
// unless the trader interaction happened), and in test mode the first applicable slot returns true.
#include <cstdio>

struct State {
    bool waiting; bool shop_keeper; bool drunk; bool hero_trades; bool shop_open;
    int reaction; bool married; bool has_gossip; long frame; long next[3]; bool wife_group; bool did_trade; unsigned long greet_index;
};

static bool TestOrDo(State& s, bool test)
{
    if (test && !s.waiting && s.shop_keeper && !s.drunk && s.hero_trades && s.shop_open) {
        s.reaction = 3;
        return true;
    }
    switch (s.reaction) {
    case 0:
        if (!s.married && s.has_gossip && test)
            return true;
    case 1:
        if (s.frame > s.next[1]) {
            s.reaction = 1;
            if (s.wife_group && test)
                return true;
        }
    case 2:
        if (!s.did_trade && s.frame > s.next[2]) {
            s.reaction = 2;
            if (s.greet_index > 0 && test)
                return true;
        }
    case 3:
        break;
    }
    return false;
}

int main()
{
    State s = { false, true, false, true, true, 0, false, false, 10, { 0, 20, 5 }, false, false, 0 };
    if (!TestOrDo(s, true) || s.reaction != 3) { std::printf("0092fab0_TEST FAIL\n"); return 1; }
    s.drunk = true; s.reaction = 0; s.has_gossip = true;
    if (!TestOrDo(s, true) || s.reaction != 0) { std::printf("0092fab0_TEST FAIL\n"); return 1; }
    s.married = true; s.reaction = 0; s.greet_index = 2;
    if (!TestOrDo(s, true) || s.reaction != 2) { std::printf("0092fab0_TEST FAIL\n"); return 1; }
    s.did_trade = true; s.reaction = 0;
    if (TestOrDo(s, true)) { std::printf("0092fab0_TEST FAIL\n"); return 1; }
    std::printf("0092fab0_TEST PASS\n");
    return 0;
}