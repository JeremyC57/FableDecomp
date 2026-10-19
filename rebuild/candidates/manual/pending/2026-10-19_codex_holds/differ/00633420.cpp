// Grade: HELD(bytes match retail) -- test exercises a hand-written stand-in, not the candidate | Codex source 00/63/CPlayer_UninitModeControlCreature_00633420.cpp, flags , pass pattern 00633420_TEST PASS
// CPlayer::UninitModeControlCreature(EPlayerMode, CProcessedInput*) -- remove all control-mode input processes from the interface
// Retail: Fable.exe @ 0x00633420, 208 bytes. The twenty calls are the non-virtual
// CAInputProcess::RemoveFromInterface helper, in the retail member order.
#include "engine/CPlayer.h"
typedef long EPlayerMode; struct CProcessedInput;
struct CAInputProcessCalls { void RemoveFromInterface(); };
struct CPlayer_Methods : CPlayer { void UninitModeControlCreature(EPlayerMode, CProcessedInput*); };
void CPlayer_Methods::UninitModeControlCreature(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    reinterpret_cast<CAInputProcessCalls*>(InputProcessControlCreature_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcessCalls*>(InputProcessControlCreatureActivateZTarget_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcessCalls*>(InputProcessControlCreatureRightStick_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcessCalls*>(InputProcessRightStickLookAround_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcessCalls*>(InputProcessCycleSpecialCameraModes_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcessCalls*>(InputProcessCreatureMovement_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcessCalls*>(InputProcessUseEnvironment_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcessCalls*>(InputProcessBlock_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcessCalls*>(InputProcessDebugControls_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcessCalls*>(InputProcessQuickAccessItems_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcessCalls*>(InputProcessCombat_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcessCalls*>(InputProcessSpecialAbilities_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcessCalls*>(InputProcessJumpingAndRolling_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcessCalls*>(InputProcessSetRangedWeaponMode_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcessCalls*>(InputProcessSetRangedWeaponThirdPerson_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcessCalls*>(InputProcessQuickAccessMenu_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcessCalls*>(InputProcessWatchForControlAngleChange_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcessCalls*>(InputProcessToggleViewHeroMode_ptr)->RemoveFromInterface();
}