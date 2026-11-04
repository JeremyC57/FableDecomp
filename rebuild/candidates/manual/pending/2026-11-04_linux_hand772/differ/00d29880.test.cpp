// Behaviour test for 0x00D29880 NScript::CQ_EndGameScript::CHookCoastTeleporter::Main: a bar that starts below the decrement
// amount gets Mother's two instructions; the bar drains by the decrement amount each interval (clamped at 0) until it reaches the
// upper limit (the bar is filled externally); the gateway cut-scene, teleport and log-book entry happen only when the bar was not
// full at the start; the thread then idles until terminated.
#include <cstdio>
static float gBar = 0.0f;
static const float kUpper = 100.0f, kDec = 5.0f; static const long kInterval = 3;
static int gFrames = 0, gTerminateAt = 1000, gMessages = 0, gCutscenes = 0, gTeleports = 0, gLog = 0, gTimer = 0, gUpdates = 0;
static bool gShowBar = false, gRemoved = false;
static bool Term() { return gFrames >= gTerminateAt; }
static void Frame() { ++gFrames; if (gTimer > 0) --gTimer; gBar += 7.0f; }   // the hero fills the bar faster than it drains
static void Main()
{
    bool full = gBar >= kUpper;
    if (gBar < kDec) { if (Term()) return; gMessages += 2; gShowBar = true; }
    gTimer = 1;
    while (gBar < kUpper) {
        Frame();
        if (Term()) return;
        if (gTimer == 0) {
            if (Term()) return;
            if (gBar < kDec) { if (Term()) return; gBar = 0.0f; }
            else { if (Term()) return; gBar -= kDec; }
            gTimer = kInterval;
        }
        ++gUpdates;
    }
    if (Term()) return;
    gRemoved = true;
    if (!full) {
        if (Term()) return;
        ++gCutscenes; ++gTeleports; gLog = 240;
    }
    for (;;) { Frame(); if (Term()) return; }
}
int main()
{
    gTerminateAt = 200;
    Main();
    bool ok = gMessages == 2 && gShowBar && gRemoved && gCutscenes == 1 && gTeleports == 1 && gLog == 240 && gUpdates > 0 && gFrames == 200;
    // A bar already full at the start: no messages, no cut-scene, straight to idling.
    gBar = 150.0f; gFrames = 0; gMessages = 0; gShowBar = false; gRemoved = false; gCutscenes = 0; gTeleports = 0; gLog = 0; gUpdates = 0;
    Main();
    ok = ok && gMessages == 0 && !gShowBar && gRemoved && gCutscenes == 0 && gLog == 0 && gUpdates == 0;
    // Terminating inside the drain loop skips the clean-up.
    gBar = 0.0f; gFrames = 0; gTerminateAt = 3; gRemoved = false; gCutscenes = 0;
    Main();
    ok = ok && !gRemoved && gCutscenes == 0;
    std::printf(ok ? "00d29880_TEST PASS\n" : "00d29880_TEST FAIL\n");
    return ok ? 0 : 1;
}
