// Behaviour test for CPlayer::UninitModeLightning (0x006366B0): the twelve lightning-mode input processes are
// removed in retail order, the hero's mode manager drops mode 25, Z-targeting is cleared and the camera manager (when
// the world provides one) returns to automatic control.
#include <cstdio>
#include <cstring>

static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CAInputProcess { public: const char* name; void RemoveFromInterface() { Log(name); } };
class CTCCreatureModeManager { public: void RemoveMode(unsigned long m) { char b[16]; std::sprintf(b, "rm%lu", m); Log(b); } };
class CThing { public: bool hasModes; CTCCreatureModeManager* modes;
    bool HasTC(long t) const { return t == 0x31 && hasModes; } CTCCreatureModeManager* GetTC(long) { return modes; } };
class CGameCameraManager { public: void SetAutomaticCameraControl() { Log("auto"); } };
struct CCameraManagerSource { CGameCameraManager* cam; bool GetCameraManager(CGameCameraManager** c) { *c = cam; return cam != 0; } };
class CWorld { public: CCameraManagerSource* CameraSource; };
class CPlayer { public:
    CWorld* World;
    CAInputProcess *InputProcessControlCreature_ptr, *InputProcessRightStickLookAround_ptr, *InputProcessStrafe_ptr,
        *InputProcessLightning_ptr, *InputProcessTargetLockRightStickTargetSelect_ptr, *InputProcessBlock_ptr,
        *InputProcessDebugControls_ptr, *InputProcessQuickAccessItems_ptr, *InputProcessSpecialAbilities_ptr,
        *InputProcessJumpingAndRolling_ptr, *InputProcessTargetLockCycleTargets_ptr, *InputProcessWatchForControlAngleChange_ptr;
    bool ZTargeting;
    CThing* hero;
    CThing* GetHeroThing() { return hero; }
    void UninitModeLightning(long mode, void* input);
};
void CPlayer::UninitModeLightning(long mode, void* input)
{
    (void)mode; (void)input;
    InputProcessControlCreature_ptr->RemoveFromInterface();
    InputProcessRightStickLookAround_ptr->RemoveFromInterface();
    InputProcessStrafe_ptr->RemoveFromInterface();
    InputProcessLightning_ptr->RemoveFromInterface();
    InputProcessTargetLockRightStickTargetSelect_ptr->RemoveFromInterface();
    InputProcessBlock_ptr->RemoveFromInterface();
    InputProcessDebugControls_ptr->RemoveFromInterface();
    InputProcessQuickAccessItems_ptr->RemoveFromInterface();
    InputProcessSpecialAbilities_ptr->RemoveFromInterface();
    InputProcessJumpingAndRolling_ptr->RemoveFromInterface();
    InputProcessTargetLockCycleTargets_ptr->RemoveFromInterface();
    InputProcessWatchForControlAngleChange_ptr->RemoveFromInterface();
    CThing* h = GetHeroThing();
    CTCCreatureModeManager* modes = 0;
    if (h->HasTC(0x31))
        modes = h->GetTC(0x31);
    modes->RemoveMode(25);
    ZTargeting = false;
    CGameCameraManager* camera = 0;
    if (World->CameraSource->GetCameraManager(&camera))
        camera->SetAutomaticCameraControl();
}
int main()
{
    int bad = 0;
    CAInputProcess p[12] = { {"cc"}, {"rs"}, {"st"}, {"li"}, {"tlr"}, {"bl"}, {"db"}, {"qa"}, {"sa"}, {"jr"}, {"tlc"}, {"wca"} };
    CTCCreatureModeManager mm; CThing hero = { true, &mm };
    CGameCameraManager cm; CCameraManagerSource src = { &cm }; CWorld w = { &src };
    CPlayer pl; pl.World = &w; pl.hero = &hero; pl.ZTargeting = true;
    pl.InputProcessControlCreature_ptr = &p[0]; pl.InputProcessRightStickLookAround_ptr = &p[1]; pl.InputProcessStrafe_ptr = &p[2];
    pl.InputProcessLightning_ptr = &p[3]; pl.InputProcessTargetLockRightStickTargetSelect_ptr = &p[4]; pl.InputProcessBlock_ptr = &p[5];
    pl.InputProcessDebugControls_ptr = &p[6]; pl.InputProcessQuickAccessItems_ptr = &p[7]; pl.InputProcessSpecialAbilities_ptr = &p[8];
    pl.InputProcessJumpingAndRolling_ptr = &p[9]; pl.InputProcessTargetLockCycleTargets_ptr = &p[10]; pl.InputProcessWatchForControlAngleChange_ptr = &p[11];
    g_log[0] = 0;
    pl.UninitModeLightning(0, 0);
    if (std::strcmp(g_log, "cc;rs;st;li;tlr;bl;db;qa;sa;jr;tlc;wca;rm25;auto;") != 0 || pl.ZTargeting) bad = 1;
    src.cam = 0; g_log[0] = 0; pl.ZTargeting = true;
    pl.UninitModeLightning(0, 0);
    if (std::strstr(g_log, "auto") || pl.ZTargeting) bad = 1;
    if (bad) { std::printf("006366b0_TEST FAIL\n"); return 1; }
    std::printf("006366b0_TEST PASS\n");
    return 0;
}