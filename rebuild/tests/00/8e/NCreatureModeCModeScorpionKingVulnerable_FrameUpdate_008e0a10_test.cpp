// Behaviour test for NCreatureMode::CModeScorpionKingVulnerable::FrameUpdate (0x008E0A10): tail emitters every third frame while the
// mesh has a dummy for them, the summon action on a phase change (mode dropped, or the body hit location reopened when the action is
// refused), else Recover once the tail-curl animation is gone or already finished.
#include <stdio.h>
#include <string>
static int g_baseUpdates, g_effects, g_dummies, g_created, g_attached, g_cleared, g_phase, g_removedMode, g_recovers;
static bool g_ready, g_actionAccepted, g_tailEffectDead, g_hasCurlAnim, g_belly = true, g_body = false;
static std::string g_lastAttach;
struct Mode {
    long FrameCount; bool EffectsDone; long TailEffectIndex; bool TailCurlFinished;
    Mode() : FrameCount(0), EffectsDone(false), TailEffectIndex(7), TailCurlFinished(false) { }
    void Recover() { ++g_recovers; }
    void FrameUpdate();
};
static bool GetDummyObject(const std::string& name, long index) { return name == "EMITTER_TAIL" && index < g_dummies; }
void Mode::FrameUpdate()
{
    ++g_baseUpdates;
    ++FrameCount;
    if (!EffectsDone && FrameCount % 3 == 0) {
        long index = g_effects;
        if (GetDummyObject("EMITTER_TAIL", index)) {
            ++g_created;
            if (!g_tailEffectDead) {
                ++g_effects;
                ++g_attached;
                g_lastAttach = "EMITTER_TAIL";
            }
        } else {
            EffectsDone = true;
        }
    }
    if (g_ready) {
        g_cleared += g_effects; g_effects = 0;
        if (g_actionAccepted) {
            ++g_phase;
            g_removedMode = 0x37;
        } else {
            g_belly = false;
            g_body = true;
        }
    } else if (!g_hasCurlAnim || TailCurlFinished) {
        Recover();
    }
}
static int fails = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); ++fails; } } while (0)
int main(void)
{
    Mode m; g_dummies = 2; g_hasCurlAnim = true;
    m.FrameUpdate(); m.FrameUpdate(); CHECK(g_created == 0 && g_recovers == 0 && g_baseUpdates == 2);
    m.FrameUpdate(); CHECK(g_created == 1 && g_effects == 1 && g_attached == 1 && g_lastAttach == "EMITTER_TAIL");
    m.FrameUpdate(); m.FrameUpdate(); m.FrameUpdate(); CHECK(g_created == 2 && g_effects == 2 && !m.EffectsDone);
    m.FrameUpdate(); m.FrameUpdate(); m.FrameUpdate(); CHECK(g_created == 2 && m.EffectsDone);      // no third dummy
    m.FrameUpdate(); m.FrameUpdate(); m.FrameUpdate(); CHECK(g_created == 2);                       // and never asked again
    Mode d; g_tailEffectDead = true; g_effects = 0; d.FrameCount = 2; d.FrameUpdate(); CHECK(g_created == 3 && g_effects == 0);
    g_tailEffectDead = false;
    m.TailCurlFinished = true; m.FrameUpdate(); CHECK(g_recovers == 1);
    m.TailCurlFinished = false; g_hasCurlAnim = false; m.FrameUpdate(); CHECK(g_recovers == 2);
    g_hasCurlAnim = true; m.FrameUpdate(); CHECK(g_recovers == 2);
    g_ready = true; g_actionAccepted = false; g_effects = 2; m.FrameUpdate();
    CHECK(g_cleared == 2 && g_effects == 0 && !g_belly && g_body && g_phase == 0 && g_removedMode == 0 && g_recovers == 2);
    g_actionAccepted = true; m.FrameUpdate(); CHECK(g_phase == 1 && g_removedMode == 0x37);
    if (fails) { printf("008e0a10_TEST FAIL\n"); return 1; }
    printf("008e0a10_TEST PASS\n");
    return 0;
}