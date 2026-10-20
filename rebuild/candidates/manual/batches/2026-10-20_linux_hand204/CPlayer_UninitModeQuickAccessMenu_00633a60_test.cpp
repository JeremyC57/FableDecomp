// Behaviour test for CPlayer::UninitModeQuickAccessMenu (0x00633A60): the fifteen mode processes are detached in member order.
#include <cstdio>
typedef long EPlayerMode; struct CProcessedInput;
static int g_order[32]; static int g_count = 0;
struct CAInputProcess {
    int id; void* Interface;
    void RemoveFromInterface() { if (Interface) { g_order[g_count++] = id; Interface = 0; } }   // 0x00A0D2D0 shape
};
struct CPlayer {
    CAInputProcess *InputProcessControlCreature_ptr, *InputProcessControlCreatureActivateZTarget_ptr,
        *InputProcessControlCreatureRightStick_ptr, *InputProcessRightStickLookAround_ptr,
        *InputProcessCycleSpecialCameraModes_ptr, *InputProcessCreatureMovement_ptr, *InputProcessUseEnvironment_ptr,
        *InputProcessBlock_ptr, *InputProcessDebugControls_ptr, *InputProcessCombat_ptr, *InputProcessUseRangedWeapon_ptr,
        *InputProcessSpecialAbilities_ptr, *InputProcessJumpingAndRolling_ptr, *InputProcessSetRangedWeaponMode_ptr,
        *InputProcessQuickAccessMenu_ptr, *InputProcessMain_ptr;
};
struct CPlayer_Methods : CPlayer { void UninitModeQuickAccessMenu(EPlayerMode, CProcessedInput*); };
void CPlayer_Methods::UninitModeQuickAccessMenu(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreature_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreatureActivateZTarget_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreatureRightStick_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessRightStickLookAround_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessCycleSpecialCameraModes_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessCreatureMovement_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessUseEnvironment_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessBlock_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessDebugControls_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessCombat_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessUseRangedWeapon_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessSpecialAbilities_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessJumpingAndRolling_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessSetRangedWeaponMode_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessQuickAccessMenu_ptr)->RemoveFromInterface();
}
int main()
{
    int iface = 0; CAInputProcess p[16];
    for (int i = 0; i < 16; ++i) { p[i].id = i; p[i].Interface = &iface; }
    CPlayer_Methods pl; CAInputProcess** slots = &pl.InputProcessControlCreature_ptr;
    for (int i = 0; i < 16; ++i) slots[i] = &p[i];
    p[6].Interface = 0;   /* not attached: skipped */
    pl.UninitModeQuickAccessMenu(3, 0);
    if (g_count != 14 || p[15].Interface != &iface) { std::printf("00633a60_TEST FAIL\n"); return 1; }
    int k = 0;
    for (int i = 0; i < 15; ++i) { if (i == 6) continue; if (g_order[k++] != i || p[i].Interface != 0) { std::printf("00633a60_TEST FAIL\n"); return 1; } }
    std::printf("00633a60_TEST PASS\n");
    return 0;
}
