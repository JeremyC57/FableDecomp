// Behaviour test for CQ_MinionCampScript::CBriarRose::Main (0x00DA0EF0): the intro cut scene picks the lower camp when the hero is
// nearer the lower rose marker; on the way to the altar Briar runs, walks while the hero lags (5 m) and stops to call him when he is
// 9 m away and further from the altar than she is (once per stop); while chanting the chant timer running out seals the portal, a
// hero hit pauses the chant, every NumHitsBetweenForcePushes-th hit triggers a force push, and an idle Briar restarts the chant.
#include <cstdio>

enum { RUN = 1, WALK = 0 };
struct Briar {
    long MoveState; bool SaidFollow; long Moves, Follows, LastMoveType;
    long BriarState, HitCount, NumHits, ForcePushes, PauseThreads, Chants; bool Playing, PortalSealed;
    static const char* Intro(float distUpper, float distLower) { return distUpper > distLower ? "LOWER" : "UPPER"; }
    void MoveStep(float meToAltar, float heroToAltar, float heroToMe)
    {
        bool heroFurther = meToAltar < heroToAltar;
        if (!(heroToMe < 9.0f) && heroFurther) {
            if (!SaidFollow) { ++Follows; SaidFollow = true; MoveState = 2; }
        } else {
            if (!(heroToMe < 5.0f) && heroFurther) {
                if (MoveState != 1) { ++Moves; LastMoveType = WALK; MoveState = 1; }
            } else {
                if (MoveState != 0) { ++Moves; LastMoveType = RUN; MoveState = 0; }
            }
            SaidFollow = false;
        }
    }
    void ChantStep(long timeLeft, bool paused, bool heroHit, bool hit)
    {
        if (timeLeft <= 0 && !paused) { PortalSealed = true; BriarState = 4; Playing = false; }
        else if (heroHit) { BriarState = 1; Playing = false; ++PauseThreads; }
        else if (hit) {
            if (++HitCount >= NumHits) { ++ForcePushes; HitCount = 0; }
            BriarState = 1; Playing = false; ++PauseThreads;
        }
        else if (!Playing) { ++Chants; Playing = true; }
    }
};

int main()
{
    bool ok = true;
    ok = ok && Briar::Intro(30.0f, 10.0f)[0] == 'L' && Briar::Intro(10.0f, 30.0f)[0] == 'U';
    Briar b = { 0, false, 0, 0, RUN, 3, 0, 3, 0, 0, 0, true, false };
    b.MoveStep(10.0f, 12.0f, 3.0f);  ok = ok && b.Moves == 0 && b.MoveState == 0;            // hero close: keep running
    b.MoveStep(10.0f, 16.0f, 6.0f);  ok = ok && b.Moves == 1 && b.LastMoveType == WALK;     // lagging: walk
    b.MoveStep(10.0f, 20.0f, 10.0f); ok = ok && b.Follows == 1 && b.MoveState == 2;         // far behind: stop and call
    b.MoveStep(10.0f, 21.0f, 11.0f); ok = ok && b.Follows == 1;                             // only once per stop
    b.MoveStep(10.0f, 12.0f, 2.0f);  ok = ok && b.Moves == 2 && b.LastMoveType == RUN && !b.SaidFollow;
    b.ChantStep(10, false, false, true); ok = ok && b.HitCount == 1 && b.BriarState == 1 && b.PauseThreads == 1;
    b.BriarState = 3;
    b.ChantStep(10, false, false, true); b.ChantStep(10, false, false, true);
    ok = ok && b.ForcePushes == 1 && b.HitCount == 0;
    b.ChantStep(10, false, false, false); ok = ok && b.Chants == 1 && b.Playing;
    b.ChantStep(0, true, false, false);   ok = ok && !b.PortalSealed;                       // paused timer does not seal
    b.ChantStep(0, false, true, false);   ok = ok && b.PortalSealed && b.BriarState == 4;
    if (!ok) { std::printf("00da0ef0_TEST FAIL\n"); return 1; }
    std::printf("00da0ef0_TEST PASS\n");
    return 0;
}