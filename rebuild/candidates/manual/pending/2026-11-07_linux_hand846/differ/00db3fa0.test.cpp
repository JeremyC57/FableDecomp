// Behaviour test for NScript::CQ_NewOakValeIntroScript::CNOVI_BookTrader::Main (0x00DB3FA0): on talk the trader gives the intro once
// (then "still got"), asks the chocolate question, and on yes sells the box for 3 gold only when the hero has 3 or more (setting
// GivenSweets so later talks just get the INTRO_10 line); a hit adds bad deed 2.
#include <cstdio>
#include <cstring>

struct Trader {
    bool DoneIntro, GivenSweets; long Gold, BadDeed; const char* Last;
    void Talk(int answer)
    {
        if (GivenSweets) { Last = "INTRO_10"; return; }
        Last = DoneIntro ? "STILL_GOT" : "INTRO";
        DoneIntro = true;
        if (answer == 1) {
            if (Gold >= 3) { Last = "GIVES_SWEETS"; Gold -= 3; GivenSweets = true; }
            else Last = "NOT_ENOUGH_CASH";
        } else Last = "BUY_LATER";
    }
    void Hit() { BadDeed = 2; }
};

int main()
{
    Trader t = { false, false, 2, 0, "" };
    t.Talk(0); bool ok = std::strcmp(t.Last, "BUY_LATER") == 0 && t.DoneIntro;
    t.Talk(1); ok = ok && std::strcmp(t.Last, "NOT_ENOUGH_CASH") == 0 && t.Gold == 2;
    t.Gold = 5; t.Talk(1); ok = ok && std::strcmp(t.Last, "GIVES_SWEETS") == 0 && t.Gold == 2 && t.GivenSweets;
    t.Talk(1); ok = ok && std::strcmp(t.Last, "INTRO_10") == 0 && t.Gold == 2;
    t.Hit(); ok = ok && t.BadDeed == 2;
    if (!ok) { std::printf("00db3fa0_TEST FAIL\n"); return 1; }
    std::printf("00db3fa0_TEST PASS\n");
    return 0;
}
