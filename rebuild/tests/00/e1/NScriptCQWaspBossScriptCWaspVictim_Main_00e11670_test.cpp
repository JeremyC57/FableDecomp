// Behaviour test for 0x00E11670 NScript::CQ_WaspBossScript::CWaspVictim::Main: the villager screams (rate-limited by the panic timer)
// while the attacker lives and the hero is near, then thanks the hero, earns morality only for the first two saved villagers,
// runs to the escape point and fades out.
#include <cstdio>
static int gFrames = 0, gTerminateAt = 1000, gTimer = 0, gScreams = 0, gSaved = 0, gMorality = 0, gMoves = 0, gFades = 0;
static bool gAttackerAlive = true, gScared = true, gRemoved = false, gQuestStart = false, gQueenAttacks = false;
static float gHealth = 30.0f, gHeroDist = 50.0f, gEscapeDist = 10.0f;
static int gSavedCount = 0, gBusy = 0;
static bool Term() { return gFrames >= gTerminateAt; }
static void Frame()
{
    ++gFrames;
    if (gTimer > 0) --gTimer;
    if (gFrames == 2) gQuestStart = true;
    if (gFrames == 5) gHeroDist = 5.0f;          // hero comes close: scream
    if (gFrames == 30) gAttackerAlive = false;   // hero kills the attacker
    if (gMoves > 0 && gEscapeDist > 0.0f) gEscapeDist -= 1.0f;
    if (gBusy > 0) --gBusy;
}
static void Main()
{
    Frame();
    if (Term()) return;
    while (!gQuestStart) { Frame(); if (Term()) return; }
    if (Term()) return;
    gHealth += 6.0f - gHealth;
    if (gQueenAttacks) { if (Term()) return; gRemoved = true; }
    while (gAttackerAlive) {
        Frame();
        if (Term()) return;
        if (gTimer == 0) {
            if (Term()) return;
            if (gHeroDist < 10.0f) {
                if (Term()) return;
                gTimer = 7;
                ++gScreams;
            }
        }
    }
    if (Term()) return;
    ++gSaved;
    gScared = false;
    if (gSavedCount < 2) {
        if (Term()) return;
        ++gMorality;
        ++gSavedCount;
    }
    while (gEscapeDist > 2.0f) {
        Frame();
        if (Term()) return;
        if (gBusy == 0) {
            if (Term()) return;
            ++gMoves;
            gBusy = 3;
        }
    }
    if (Term()) return;
    ++gFades;
}
int main()
{
    Main();
    // 25 frames of attack with the hero near (frames 5..29) and a 7-frame panic timer: screams at frames 5, 13, 21, 29.
    bool ok = gHealth == 6.0f && !gRemoved && gScreams == 4 && gSaved == 1 && !gScared && gMorality == 1 && gSavedCount == 1
           && gMoves >= 1 && gFades == 1;
    // A third saved villager gets no morality.
    gFrames = 0; gTimer = 0; gAttackerAlive = true; gQuestStart = false; gHeroDist = 50.0f; gEscapeDist = 10.0f; gMoves = 0;
    gSavedCount = 2; gMorality = 0; gFades = 0;
    Main();
    ok = ok && gMorality == 0 && gSavedCount == 2 && gFades == 1;
    // Terminating while waiting for the quest start leaves everything untouched.
    gFrames = 0; gTerminateAt = 1; gFades = 0; gQuestStart = false;
    Main();
    ok = ok && gFades == 0;
    std::printf(ok ? "00e11670_TEST PASS\n" : "00e11670_TEST FAIL\n");
    return ok ? 0 : 1;
}