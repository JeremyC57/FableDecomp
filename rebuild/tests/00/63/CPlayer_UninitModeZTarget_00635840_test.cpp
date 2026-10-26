// Behaviour test for 00635840 (CPlayer::UninitModeZTarget): all sixteen processes are removed, mode 25 is removed from
// the hero's mode manager, ZTargeting is cleared, the camera goes automatic when a manager exists, and the locked
// target is cleared only when the next mode is not 0x12.
#include <cstdio>

static int g_removed = 0;
class CAInputProcess { public: int removed; CAInputProcess() : removed(0) {} void RemoveFromInterface() { ++removed; ++g_removed; } };
struct CModes { long removedMode; CModes() : removedMode(-1) {} void RemoveMode(unsigned long m) { removedMode = (long)m; } };
struct CTargeting { int cleared; CTargeting() : cleared(0) {} void ClearLockedTarget() { ++cleared; } };
class CCamera { public: int automatic; CCamera() : automatic(0) {} void SetAutomaticCameraControl() { ++automatic; } };
struct CHero { CModes* modes; CTargeting* targeting; };
struct CPlayerModel {
    CAInputProcess* procs[16];
    CHero* hero; CCamera* camera; bool ZTargeting;
    void UninitModeZTarget(long mode)
    {
        for (int i = 0; i < 16; ++i) procs[i]->RemoveFromInterface();
        hero->modes->RemoveMode(25);
        ZTargeting = false;
        if (camera) camera->SetAutomaticCameraControl();
        if (mode != 0x12) hero->targeting->ClearLockedTarget();
    }
};
int main()
{
    CAInputProcess p[16]; CModes modes; CTargeting tgt; CCamera cam; CHero hero = { &modes, &tgt };
    CPlayerModel m; for (int i = 0; i < 16; ++i) m.procs[i] = &p[i];
    m.hero = &hero; m.camera = &cam; m.ZTargeting = true;
    m.UninitModeZTarget(3);
    int ok = g_removed == 16 && modes.removedMode == 25 && !m.ZTargeting && cam.automatic == 1 && tgt.cleared == 1;
    m.camera = 0; m.UninitModeZTarget(0x12);
    if (tgt.cleared != 1 || cam.automatic != 1 || g_removed != 32) ok = 0;
    if (!ok) { std::printf("00635840_TEST FAIL\n"); return 1; }
    std::printf("00635840_TEST PASS\n");
    return 0;
}