// Behaviour test for CCombatAction_StrikeResponseMazeDoForcePush::FinishAction (0x008C2810): a strike from within 4
// units on a boss past phase 0 bumps the hit counter, or (past the def's tutorial hit limit, or in phase 2) makes Maze
// answer with a force push.
#include <cstdio>
#include <cmath>

class CThing { public: float X, Y; bool Dead; };
struct CBoss { long GenericCounter; long Phase; };
struct CAction {
    CThing* PCreature; CThing* PAttacker; CBoss* Boss; long TutorialNumGetHits; int Base; int ForcePushes;
    void FinishAction()
    {
        ++Base;
        long hits = Boss->GenericCounter + 1;
        if (PAttacker && !PAttacker->Dead) {
            float dx = PCreature->X - PAttacker->X, dy = PCreature->Y - PAttacker->Y;
            if (Boss->Phase > 0 && std::sqrt(dx * dx + dy * dy) < 4.0f) {
                if (hits > TutorialNumGetHits || Boss->Phase == 2)
                    ++ForcePushes;
                else
                    Boss->GenericCounter = hits;
            }
        }
    }
};

int main()
{
    CThing maze = { 0, 0, false }, hero = { 1, 1, false };
    CBoss boss = { 0, 1 };
    CAction a = { &maze, &hero, &boss, 2, 0, 0 };
    a.FinishAction(); a.FinishAction();
    bool ok = boss.GenericCounter == 2 && a.ForcePushes == 0;
    a.FinishAction();
    ok = ok && boss.GenericCounter == 2 && a.ForcePushes == 1;
    hero.X = 10; boss.GenericCounter = 0; a.FinishAction();
    ok = ok && boss.GenericCounter == 0 && a.ForcePushes == 1;
    hero.X = 1; boss.Phase = 2; a.FinishAction();
    ok = ok && a.ForcePushes == 2;
    boss.Phase = 0; a.FinishAction();
    ok = ok && a.ForcePushes == 2 && a.Base == 6;
    if (!ok) { std::printf("008c2810_TEST FAIL\n"); return 1; }
    std::printf("008c2810_TEST PASS\n");
    return 0;
}