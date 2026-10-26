// Behaviour test for retail 0x00633420: the candidate source below is embedded verbatim (from src/00633420.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// CPlayer::UninitModeControlCreature(EPlayerMode, CProcessedInput*) -- remove all control-mode input processes from the interface
// Retail: Fable.exe (TLC, VC7.1) @ 0x00633420, 208 bytes. The eighteen calls are the non-virtual
// CAInputProcess::RemoveFromInterface @ 0x00A0D2D0 (manifest), in the retail member order.
// Layout: engine/CPlayer.h (the scoped_ptr<CInputProcess*> members); CPlayer_Methods is the lane's method-carrier
// convention for generated data-only headers (see the landed CPlayer::InitModeControlFreeCamera 0x006335E0).
#include "engine/CPlayer.h"
typedef long EPlayerMode; class CProcessedInput;
class CAInputProcess { public: void RemoveFromInterface(); };          // 0x00A0D2D0
struct CPlayer_Methods : CPlayer { void UninitModeControlCreature(EPlayerMode, CProcessedInput*); };
void CPlayer_Methods::UninitModeControlCreature(EPlayerMode mode, CProcessedInput* input)
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
    reinterpret_cast<CAInputProcess*>(InputProcessQuickAccessItems_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessCombat_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessSpecialAbilities_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessJumpingAndRolling_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessSetRangedWeaponMode_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessSetRangedWeaponThirdPerson_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessQuickAccessMenu_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessWatchForControlAngleChange_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessToggleViewHeroMode_ptr)->RemoveFromInterface();
}
// ===== harness =====
#include <stdio.h>
#include <string.h>
static void* g_seen[32]; static int g_n; static CGamePlayerInterface* g_iface;
void CAInputProcess::RemoveFromInterface() { g_seen[g_n++] = this; }
static CPlayer_Methods g_player;
static unsigned char g_procs[64][8];
int main() {
    memset(&g_player, 0, sizeof g_player);
    *(void**)&g_player.InputProcessControlCreature_ptr = &g_procs[0];
    *(void**)&g_player.InputProcessControlCreatureActivateZTarget_ptr = &g_procs[1];
    *(void**)&g_player.InputProcessControlCreatureRightStick_ptr = &g_procs[2];
    *(void**)&g_player.InputProcessRightStickLookAround_ptr = &g_procs[3];
    *(void**)&g_player.InputProcessCycleSpecialCameraModes_ptr = &g_procs[4];
    *(void**)&g_player.InputProcessCreatureMovement_ptr = &g_procs[5];
    *(void**)&g_player.InputProcessUseEnvironment_ptr = &g_procs[6];
    *(void**)&g_player.InputProcessBlock_ptr = &g_procs[7];
    *(void**)&g_player.InputProcessDebugControls_ptr = &g_procs[8];
    *(void**)&g_player.InputProcessQuickAccessItems_ptr = &g_procs[9];
    *(void**)&g_player.InputProcessCombat_ptr = &g_procs[10];
    *(void**)&g_player.InputProcessSpecialAbilities_ptr = &g_procs[11];
    *(void**)&g_player.InputProcessJumpingAndRolling_ptr = &g_procs[12];
    *(void**)&g_player.InputProcessSetRangedWeaponMode_ptr = &g_procs[13];
    *(void**)&g_player.InputProcessSetRangedWeaponThirdPerson_ptr = &g_procs[14];
    *(void**)&g_player.InputProcessQuickAccessMenu_ptr = &g_procs[15];
    *(void**)&g_player.InputProcessWatchForControlAngleChange_ptr = &g_procs[16];
    *(void**)&g_player.InputProcessToggleViewHeroMode_ptr = &g_procs[17];
    g_player.PlayerInterface = (CGamePlayerInterface*)0x1234;
    g_player.UninitModeControlCreature((EPlayerMode)3, 0);
    int ok = g_n == 18;
    ok = ok && g_seen[0] == (void*)&g_procs[0];
    ok = ok && g_seen[1] == (void*)&g_procs[1];
    ok = ok && g_seen[2] == (void*)&g_procs[2];
    ok = ok && g_seen[3] == (void*)&g_procs[3];
    ok = ok && g_seen[4] == (void*)&g_procs[4];
    ok = ok && g_seen[5] == (void*)&g_procs[5];
    ok = ok && g_seen[6] == (void*)&g_procs[6];
    ok = ok && g_seen[7] == (void*)&g_procs[7];
    ok = ok && g_seen[8] == (void*)&g_procs[8];
    ok = ok && g_seen[9] == (void*)&g_procs[9];
    ok = ok && g_seen[10] == (void*)&g_procs[10];
    ok = ok && g_seen[11] == (void*)&g_procs[11];
    ok = ok && g_seen[12] == (void*)&g_procs[12];
    ok = ok && g_seen[13] == (void*)&g_procs[13];
    ok = ok && g_seen[14] == (void*)&g_procs[14];
    ok = ok && g_seen[15] == (void*)&g_procs[15];
    ok = ok && g_seen[16] == (void*)&g_procs[16];
    ok = ok && g_seen[17] == (void*)&g_procs[17];
    printf(ok ? "00633420_TEST PASS\n" : "00633420_TEST FAIL\n"); return ok ? 0 : 1;
}