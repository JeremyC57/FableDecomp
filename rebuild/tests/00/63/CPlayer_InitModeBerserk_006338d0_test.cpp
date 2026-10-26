// Behaviour test for retail 0x006338d0: the candidate source below is embedded verbatim (from src/006338d0.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// CPlayer::InitModeBerserk(EPlayerMode, CProcessedInput*) -- attach the berserk mode's input processes to the player interface
// Retail: Fable.exe (TLC, VC7.1) @ 0x006338D0, 224 bytes. Each process gets the virtual
// CAInputProcess::AddToInterface(CGamePlayerInterface*) call (vtable slot 1), with PlayerInterface passed as its argument.
// Layout: engine/CPlayer.h (the scoped_ptr<CInputProcess*> members, PlayerInterface +0x0C); CPlayer_Methods is the lane's
// method-carrier convention for generated data-only headers (see the landed CPlayer::InitModeControlFreeCamera 0x006335E0).
#include "engine/CPlayer.h"
typedef long EPlayerMode; class CProcessedInput;
class CAInputProcess { public: virtual void VFunc00(); virtual bool AddToInterface(CGamePlayerInterface*); };   // slot 1 (+0x04)
struct CPlayer_Methods : CPlayer { void InitModeBerserk(EPlayerMode, CProcessedInput*); };
void CPlayer_Methods::InitModeBerserk(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreature_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreatureActivateZTarget_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreatureRightStick_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessRightStickLookAround_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessCycleSpecialCameraModes_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessCreatureMovement_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessBlock_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessDebugControls_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessQuickAccessItems_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessCombat_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessUseRangedWeapon_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessSpecialAbilities_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessJumpingAndRolling_ptr)->AddToInterface(PlayerInterface);
    reinterpret_cast<CAInputProcess*>(InputProcessSetRangedWeaponMode_ptr)->AddToInterface(PlayerInterface);
}
// ===== harness =====
#include <stdio.h>
#include <string.h>
static void* g_seen[32]; static int g_n; static CGamePlayerInterface* g_iface;
void CAInputProcess::VFunc00() {}
bool CAInputProcess::AddToInterface(CGamePlayerInterface* i) { return false; }
struct TProcess : CAInputProcess { virtual bool AddToInterface(CGamePlayerInterface* i) { g_iface = i; g_seen[g_n++] = this; return true; } };
static CPlayer_Methods g_player;
static TProcess g_procs[64];
int main() {
    memset(&g_player, 0, sizeof g_player);
    *(void**)&g_player.InputProcessControlCreature_ptr = &g_procs[0];
    *(void**)&g_player.InputProcessControlCreatureActivateZTarget_ptr = &g_procs[1];
    *(void**)&g_player.InputProcessControlCreatureRightStick_ptr = &g_procs[2];
    *(void**)&g_player.InputProcessRightStickLookAround_ptr = &g_procs[3];
    *(void**)&g_player.InputProcessCycleSpecialCameraModes_ptr = &g_procs[4];
    *(void**)&g_player.InputProcessCreatureMovement_ptr = &g_procs[5];
    *(void**)&g_player.InputProcessBlock_ptr = &g_procs[6];
    *(void**)&g_player.InputProcessDebugControls_ptr = &g_procs[7];
    *(void**)&g_player.InputProcessQuickAccessItems_ptr = &g_procs[8];
    *(void**)&g_player.InputProcessCombat_ptr = &g_procs[9];
    *(void**)&g_player.InputProcessUseRangedWeapon_ptr = &g_procs[10];
    *(void**)&g_player.InputProcessSpecialAbilities_ptr = &g_procs[11];
    *(void**)&g_player.InputProcessJumpingAndRolling_ptr = &g_procs[12];
    *(void**)&g_player.InputProcessSetRangedWeaponMode_ptr = &g_procs[13];
    g_player.PlayerInterface = (CGamePlayerInterface*)0x1234;
    g_player.InitModeBerserk((EPlayerMode)3, 0);
    int ok = g_n == 14;
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
    ok = ok && g_iface == (CGamePlayerInterface*)0x1234;
    printf(ok ? "006338d0_TEST PASS\n" : "006338d0_TEST FAIL\n"); return ok ? 0 : 1;
}