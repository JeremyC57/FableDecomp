// Behaviour test for 0x006881D0 (CInputProcessControlCreature::Update): the spell / expression-shift modes drop when their buttons are released.
#include <cstdio>
struct Player { bool SpellMode, ExpressionShiftMode; int GetPlayerNumber() const { return 0; } };
struct GPI {
    bool spellForced, shiftForced; bool held56, held57;
    bool IsSpellModeForced() const { return spellForced; }
    bool IsExpressionShiftModeForced() const { return shiftForced; }
    bool IsGameActionEventInQueue(int action, int kind, int) { return kind == 2 && ((action == 0x56 && held56) || (action == 0x57 && held57)); }
};
struct Process {
    Player* MyPlayer; GPI* gpi;
    void Update()
    {
        if (MyPlayer->SpellMode && !gpi->IsSpellModeForced()) { bool held = gpi->IsGameActionEventInQueue(0x56, 2, MyPlayer->GetPlayerNumber()); if (!held) MyPlayer->SpellMode = held; }
        if (MyPlayer->ExpressionShiftMode && !gpi->IsExpressionShiftModeForced()) { if (!gpi->IsGameActionEventInQueue(0x57, 2, MyPlayer->GetPlayerNumber())) MyPlayer->ExpressionShiftMode = false; }
    }
};
int main()
{
    Player pl = { true, true }; GPI g = { false, false, true, true }; Process p; p.MyPlayer = &pl; p.gpi = &g;
    p.Update(); int ok = pl.SpellMode && pl.ExpressionShiftMode;
    g.held56 = false; p.Update(); ok = ok && !pl.SpellMode && pl.ExpressionShiftMode;
    g.held57 = false; g.shiftForced = true; p.Update(); ok = ok && pl.ExpressionShiftMode;
    g.shiftForced = false; p.Update(); ok = ok && !pl.ExpressionShiftMode;
    if (!ok) { std::printf("006881d0_TEST FAIL\n"); return 1; }
    std::printf("006881d0_TEST PASS\n");
    return 0;
}
