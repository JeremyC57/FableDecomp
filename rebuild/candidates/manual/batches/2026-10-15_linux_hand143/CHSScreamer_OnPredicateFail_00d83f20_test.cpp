// Behaviour test for 0x00D83F20 (CHS_Screamer::OnPredicateFail): mother-script bookkeeping and the super-screamer clean-up.
#include <cstdio>
struct ScriptThing { int id; };
struct Def { float ScreamerBarIncrease; int SuperScreamerTimerTime; };
struct Mother { int ScreamersAlive; float BarToFill; bool GotSuperScreamer; int TimerHandle; };
struct Game {
    int timerSet, timerTime, removed, ended; const ScriptThing* removedThing; const ScriptThing* endedThing; bool ra, rb;
    Game() : timerSet(-1), timerTime(0), removed(0), ended(0), removedThing(0), endedThing(0), ra(true), rb(false) {}
    void SetTimer(int t, int time) { timerSet = t; timerTime = time; }
    void RemoveThing(const ScriptThing& t, bool a, bool b) { ++removed; removedThing = &t; ra = a; rb = b; }
    void EntityEndScreamerSuperAttackThing(const ScriptThing& t) { ++ended; endedThing = &t; }
};
static Game* g_pGameScriptInterface; static Def* g_pScriptDef;
struct Screamer {
    Game* game; ScriptThing Me; Mother* parent; bool SuperScreamer; ScriptThing SuperScreamerEffect;
    void OnPredicateFail()
    {
        --parent->ScreamersAlive;
        parent->BarToFill += g_pScriptDef->ScreamerBarIncrease;
        if (SuperScreamer) {
            parent->GotSuperScreamer = false;
            g_pGameScriptInterface->SetTimer(parent->TimerHandle, g_pScriptDef->SuperScreamerTimerTime);
            game->RemoveThing(SuperScreamerEffect, false, true);
            game->EntityEndScreamerSuperAttackThing(Me);
        }
    }
};
int main()
{
    Def def = { 2.5f, 300 }; Game global, own; g_pGameScriptInterface = &global; g_pScriptDef = &def;
    Mother m = { 5, 1.0f, true, 17 };
    Screamer plain; plain.game = &own; plain.parent = &m; plain.SuperScreamer = false;
    plain.OnPredicateFail();
    int ok = m.ScreamersAlive == 4 && m.BarToFill == 3.5f && m.GotSuperScreamer && global.timerSet == -1 && own.removed == 0;
    Screamer super; super.game = &own; super.parent = &m; super.SuperScreamer = true;
    super.OnPredicateFail();
    ok = ok && m.ScreamersAlive == 3 && m.BarToFill == 6.0f && !m.GotSuperScreamer && global.timerSet == 17 && global.timerTime == 300;
    ok = ok && own.removed == 1 && own.removedThing == &super.SuperScreamerEffect && !own.ra && own.rb && own.ended == 1 && own.endedThing == &super.Me;
    if (!ok) { std::printf("00d83f20_TEST FAIL\n"); return 1; }
    std::printf("00d83f20_TEST PASS\n");
    return 0;
}
