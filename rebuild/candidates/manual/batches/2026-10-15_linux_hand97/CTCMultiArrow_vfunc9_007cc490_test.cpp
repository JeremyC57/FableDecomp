// Behaviour test for CTCMultiArrow::vfunc_9 @ 0x007CC490: timer-driven ammo effects, removal once spent and out of shots.
#include <cstdio>
struct CThing { int removed; long type; void RemoveTC(long t) { ++removed; type = t; } };
struct CTCMultiArrow {
    CThing* ParentThing;
    long NumberOfShots, CreateAmmoEffectCount, CreateAmmoEffectDelay, CreateAmmoEffectTimer;
    bool CutSceneCancelled;
    int m_created;
    void CreateAmmoEffect() { ++m_created; }
    void vfunc_9()
    {
        if (CreateAmmoEffectCount > 0) {
            if (!CutSceneCancelled) {
                if (--CreateAmmoEffectTimer == 0) { CreateAmmoEffectTimer = CreateAmmoEffectDelay; CreateAmmoEffect(); }
            }
        } else if (NumberOfShots <= 0) {
            ParentThing->RemoveTC(0x88);
        }
    }
};
int main()
{
    CThing t; t.removed = 0; t.type = 0;
    CTCMultiArrow m; m.ParentThing = &t; m.NumberOfShots = 3; m.CreateAmmoEffectCount = 2; m.CreateAmmoEffectDelay = 3;
    m.CreateAmmoEffectTimer = 2; m.CutSceneCancelled = false; m.m_created = 0;
    m.vfunc_9();
    if (m.CreateAmmoEffectTimer != 1 || m.m_created != 0) { std::printf("007cc490_TEST FAIL\n"); return 1; }
    m.vfunc_9();
    if (m.CreateAmmoEffectTimer != 3 || m.m_created != 1) { std::printf("007cc490_TEST FAIL\n"); return 1; }
    m.CutSceneCancelled = true; m.vfunc_9();
    if (m.CreateAmmoEffectTimer != 3) { std::printf("007cc490_TEST FAIL\n"); return 1; }
    m.CreateAmmoEffectCount = 0; m.vfunc_9();
    if (t.removed != 0) { std::printf("007cc490_TEST FAIL\n"); return 1; }
    m.NumberOfShots = 0; m.vfunc_9();
    if (t.removed != 1 || t.type != 0x88) { std::printf("007cc490_TEST FAIL\n"); return 1; }
    std::printf("007cc490_TEST PASS\n");
    return 0;
}
