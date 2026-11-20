// CPlayer::UninitModeParalysed(EPlayerMode mode, CProcessedInput* input) -- leaving the paralysed mode detaches the paralysed, right-stick creature, right-stick look-around, special-camera-cycle, creature-movement and debug input processes from the player's interface
// Leaf: ?UninitModeParalysed@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00633830, length 73 bytes (+ int3 padding), `ret 8`.
// Identity: the PlayerModeInfo table filled by the dynamic initialiser at 0x0121B4xx stores 0x006337C0
// (CPlayer::InitModeParalysed, landed) at 0x013B9068 and this function at 0x013B9078 -- the paired UninitFunc. The manifest
// label (_Dest_val<allocator<CEditLevelMerger::CConflict>>) is propagated across a byte-identical shape and wrong.
// Callees: the non-virtual CAInputProcess::RemoveFromInterface @ 0x00A0D2D0 on six scoped_ptr<CInputProcess*> members,
// in the same order InitModeParalysed adds them (+0x1B0, +0x16C, +0x170, +0x174, +0x150, +0x15C).
// Layout: engine/CPlayer.h, retail branch (FABLE_RETAIL_PLAYER + FABLE_RETAIL_PLAYER_INPUTPROCESSES: the scoped_ptr<CInputProcess...> members),
//   as the landed UninitModeFishing 0x00633780 / UninitModeDigging 0x006337B0.
// NF113 (PRIORITY, q4615): was the lane carrier CPlayer_Methods (?UninitModeParalysed@CPlayer_Methods@@QAEXJPAVCProcessedInput@@@Z); now the private
//   ?UninitModeParalysed@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z of Ego_r.pdb + FableWin.pdb, which the landed PlayerModeInfo initialiser
//   0x0121AF20 already binds; each member goes through its own scoped_ptr operator-> (no casts).
// Shape: both hook arguments unused (`ret 8`); each member pointer goes straight into ecx.
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_INPUTPROCESSES
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_REMOVEFROMINTERFACE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CPlayer.h"
#include "engine/CInputProcessParalysed.h"
#include "engine/CInputProcessControlCreatureRightStick.h"
#include "engine/CInputProcessCameraLookAround.h"
#include "engine/CInputProcessCycleSpecialCameraModes.h"
#include "engine/CInputProcessCreatureMovement.h"
#include "engine/CInputProcessDebugControls.h"

void CPlayer::UninitModeParalysed(EPlayerMode mode, CProcessedInput* input)
{
    InputProcessParalysed->RemoveFromInterface();
    InputProcessControlCreatureRightStick->RemoveFromInterface();
    InputProcessRightStickLookAround->RemoveFromInterface();
    InputProcessCycleSpecialCameraModes->RemoveFromInterface();
    InputProcessCreatureMovement->RemoveFromInterface();
    InputProcessDebugControls->RemoveFromInterface();
}
