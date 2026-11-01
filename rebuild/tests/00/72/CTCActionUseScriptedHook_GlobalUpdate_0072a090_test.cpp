// Behaviour test for CTCActionUseScriptedHook::GlobalUpdate (0x0072A090): a copy of the decision logic over a minimal model,
// checking the confirmation consume, the locked/unlocked teleport branches and the camera-track gate (point type 7 only).
#include <cstdio>
#include <cstring>

struct Hook {
    bool forceConfirmation, waitingForConfirmation, performAction, teleport, entranceLocked;
    bool hasEntrance, hasCameraTrack, trackHasPoint;
    long trackPointType;
};
struct World {
    bool messagePresent; long messageExtra; int removed;
    int teleports; int sounds; char lastSound[32]; int cameraModes;
    bool hasSound; bool heroPresent; bool heroDead;
};

static void PlaySound(World& w, const char* name) { ++w.sounds; std::strcpy(w.lastSound, name); }

static void GlobalUpdate(Hook& h, World& w)
{
    if (h.forceConfirmation && h.waitingForConfirmation) {
        if (w.messagePresent) {
            if (w.messageExtra == 1)
                h.performAction = true;
            h.waitingForConfirmation = false;
            ++w.removed;
        }
        return;
    }
    if (!h.performAction)
        return;
    if (h.teleport && h.hasEntrance) {
        if (h.entranceLocked) {
            if (w.hasSound)
                PlaySound(w, "DOOR_OPEN_FAIL");
        } else if (w.heroPresent && !w.heroDead) {
            ++w.teleports;
            PlaySound(w, "BIG_DOOR_UNLATCH");
        }
    }
    if (h.hasCameraTrack && h.trackHasPoint && h.trackPointType == 7)
        ++w.cameraModes;
    h.performAction = false;
}

int main()
{
    int fails = 0;
    World w0; std::memset(&w0, 0, sizeof w0); w0.messagePresent = true; w0.messageExtra = 1;
    Hook h0; std::memset(&h0, 0, sizeof h0); h0.forceConfirmation = true; h0.waitingForConfirmation = true;
    GlobalUpdate(h0, w0);
    if (!h0.performAction || h0.waitingForConfirmation || w0.removed != 1) ++fails;

    World w1; std::memset(&w1, 0, sizeof w1); w1.messagePresent = true; w1.messageExtra = 0;
    Hook h1 = h0; h1.performAction = false; h1.waitingForConfirmation = true;
    GlobalUpdate(h1, w1);
    if (h1.performAction || w1.removed != 1) ++fails;

    World w2; std::memset(&w2, 0, sizeof w2); w2.hasSound = true; w2.heroPresent = true;
    Hook h2; std::memset(&h2, 0, sizeof h2); h2.performAction = true; h2.teleport = true; h2.hasEntrance = true; h2.entranceLocked = true;
    GlobalUpdate(h2, w2);
    if (w2.sounds != 1 || std::strcmp(w2.lastSound, "DOOR_OPEN_FAIL") != 0 || w2.teleports != 0 || h2.performAction) ++fails;

    World w3; std::memset(&w3, 0, sizeof w3); w3.hasSound = true; w3.heroPresent = true;
    Hook h3 = h2; h3.performAction = true; h3.entranceLocked = false; h3.hasCameraTrack = true; h3.trackHasPoint = true; h3.trackPointType = 7;
    GlobalUpdate(h3, w3);
    if (w3.teleports != 1 || std::strcmp(w3.lastSound, "BIG_DOOR_UNLATCH") != 0 || w3.cameraModes != 1) ++fails;

    World w4; std::memset(&w4, 0, sizeof w4); w4.heroPresent = true; w4.heroDead = true;
    Hook h4 = h3; h4.performAction = true; h4.trackPointType = 3;
    GlobalUpdate(h4, w4);
    if (w4.teleports != 0 || w4.sounds != 0 || w4.cameraModes != 0 || h4.performAction) ++fails;

    Hook h5; std::memset(&h5, 0, sizeof h5); World w5; std::memset(&w5, 0, sizeof w5);
    h5.hasCameraTrack = true; h5.trackHasPoint = true; h5.trackPointType = 7;
    GlobalUpdate(h5, w5);
    if (w5.cameraModes != 0) ++fails;

    if (fails == 0) std::printf("0072a090_TEST PASS\n");
    else std::printf("0072a090_TEST FAIL %d\n", fails);
    return fails;
}