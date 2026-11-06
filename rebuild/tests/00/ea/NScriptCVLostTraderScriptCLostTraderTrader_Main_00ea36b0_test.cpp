// Behaviour test for CV_LostTraderScript::CLostTraderTrader::Main (0x00EA36B0): the trader's state brain. State 1 waits while
// the hero is beyond the outer distance; state 2 yells for help between the middle and outer rings (back to 1 when the hero
// leaves), faces the hero, yells at him inside the middle ring and moves to 3 once the hero is inside the inner ring;
// state 3 introduces himself once the hero is within 3 units and moves to 4; state 4 follows the hero until his brother is
// within the reunite distance, then 5 (idle). A hit by the hero makes trader and hero allies in every state.
#include <cstdio>

struct Trader {
    long State; bool Allies, ReachedTrader; int Yells, YellsAtHero, Intros;
    float Outer, Middle, Inner;
    // one pass of the switch for a hero at distance d (the inner loops collapsed to their exit conditions)
    void Step(float d, float brother, float reunite, bool hit)
    {
        if (hit) Allies = true;
        switch (State) {
        case 1: if (!(d > Outer)) State = 2; break;
        case 2:
            if (d > Middle && !(d > Outer)) ++Yells;
            if (d > Outer) { State = 1; break; }
            if (d > Inner && !(d > Middle)) ++YellsAtHero;
            if (!(d > Inner)) State = 3;
            break;
        case 3: if (!(d > 3.0f)) { ++Intros; State = 4; } break;
        case 4: ReachedTrader = true; if (brother < reunite) State = 5; break;
        default: break;
        }
    }
};

int main()
{
    Trader t = { 1, false, false, 0, 0, 0, 40.0f, 20.0f, 10.0f };
    bool ok = true;
    t.Step(50.0f, 100.0f, 5.0f, false); ok = ok && t.State == 1;
    t.Step(30.0f, 100.0f, 5.0f, false); ok = ok && t.State == 2;
    t.Step(30.0f, 100.0f, 5.0f, false); ok = ok && t.State == 2 && t.Yells == 1;
    t.Step(45.0f, 100.0f, 5.0f, false); ok = ok && t.State == 1;
    t.Step(30.0f, 100.0f, 5.0f, true);  ok = ok && t.State == 2 && t.Allies;
    t.Step(15.0f, 100.0f, 5.0f, false); ok = ok && t.State == 2 && t.YellsAtHero == 1;
    t.Step(5.0f, 100.0f, 5.0f, false);  ok = ok && t.State == 3;
    t.Step(5.0f, 100.0f, 5.0f, false);  ok = ok && t.State == 3 && t.Intros == 0;
    t.Step(2.0f, 100.0f, 5.0f, false);  ok = ok && t.State == 4 && t.Intros == 1;
    t.Step(2.0f, 9.0f, 5.0f, false);    ok = ok && t.State == 4 && t.ReachedTrader;
    t.Step(2.0f, 4.0f, 5.0f, false);    ok = ok && t.State == 5;
    t.Step(2.0f, 4.0f, 5.0f, false);    ok = ok && t.State == 5;
    if (!ok) { std::printf("00ea36b0_TEST FAIL\n"); return 1; }
    std::printf("00ea36b0_TEST PASS\n");
    return 0;
}