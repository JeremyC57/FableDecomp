// CPlayer::UninitModeBoastUI(EPlayerMode mode, CProcessedInput* input) -- leaving the boast-UI mode detaches the boast-UI, control-creature, right-stick look-around, special-camera-cycle, creature-movement and control-angle-watch input processes from the player's interface
// Leaf: ?UninitModeBoastUI@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00633880, length 73 bytes (+ int3 padding), `ret 8`.
// Identity: the PlayerModeInfo table filled by the dynamic initialiser at 0x0121B6xx stores 0x00633F10
// (CPlayer::InitModeBoastUI, landed) at 0x013B9088 and this function at 0x013B9098 -- the paired UninitFunc. The manifest
// label (_Dest_val<allocator<CEditLevelMerger::CConflict>>) is propagated across a byte-identical shape and wrong.
// Callees: the non-virtual CAInputProcess::RemoveFromInterface @ 0x00A0D2D0 on six scoped_ptr<CInputProcess*> members,
// in the same order InitModeBoastUI adds them (+0x1B4, +0x104, +0x170, +0x174, +0x150, +0x1E8).
// Layout: engine/CPlayer.h, retail branch (FABLE_RETAIL_PLAYER + FABLE_RETAIL_PLAYER_INPUTPROCESSES: the scoped_ptr<CInputProcess...> members),
//   as the landed UninitModeFishing 0x00633780 / UninitModeDigging 0x006337B0.
// NF113 (PRIORITY, q4616): was the lane carrier CPlayer_Methods (?UninitModeBoastUI@CPlayer_Methods@@QAEXJPAVCProcessedInput@@@Z); now the private
//   ?UninitModeBoastUI@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z of Ego_r.pdb + FableWin.pdb, which the landed PlayerModeInfo initialiser
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
#include "engine/CInputProcessBoastUI.h"
#include "engine/CInputProcessControlCreature.h"
#include "engine/CInputProcessCameraLookAround.h"
#include "engine/CInputProcessCycleSpecialCameraModes.h"
#include "engine/CInputProcessCreatureMovement.h"
#include "engine/CInputProcessCreatureMovementWatchForControlAngleChange.h"

void CPlayer::UninitModeBoastUI(EPlayerMode mode, CProcessedInput* input)
{
    InputProcessBoastUI->RemoveFromInterface();
    InputProcessControlCreature->RemoveFromInterface();
    InputProcessRightStickLookAround->RemoveFromInterface();
    InputProcessCycleSpecialCameraModes->RemoveFromInterface();
    InputProcessCreatureMovement->RemoveFromInterface();
    InputProcessWatchForControlAngleChange->RemoveFromInterface();
}
