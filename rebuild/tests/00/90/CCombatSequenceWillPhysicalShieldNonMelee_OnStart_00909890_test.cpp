// behaviour test for 0x00909890 CCombatSequence_WillPhysicalShieldNonMelee::OnStart(): the creature's will-power indicator, when present, is reset once with -1 (the def's recharge time); a creature without one is left alone
#include <cstdio>
struct Indicator { int calls; float last; Indicator() : calls(0), last(0.0f) { } void Reset(float t) { calls++; last = t; } };
struct Creature { Indicator* WillPower; };
struct Sequence {
    Creature* Owner;
    void OnStart()                                       // the function under test
    {
        Indicator* indicator = Owner->WillPower;
        if (indicator)
            indicator->Reset(-1.0f);
    }
};
int main()
{
    int fail = 0;
    Indicator ind; Creature with; with.WillPower = &ind; Creature without; without.WillPower = 0;
    Sequence s; s.Owner = &with; s.OnStart();
    if (ind.calls != 1 || ind.last != -1.0f) fail |= 1;
    Sequence t; t.Owner = &without; t.OnStart();
    if (ind.calls != 1) fail |= 2;
    if (fail) { std::printf("00909890_TEST FAIL %d\n", fail); return 1; }
    std::printf("00909890_TEST PASS\n");
    return 0;
}