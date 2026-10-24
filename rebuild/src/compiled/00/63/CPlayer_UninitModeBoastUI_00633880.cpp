// CPlayer::UninitModeBoastUI(EPlayerMode mode, CProcessedInput* input) -- leaving the boast-UI mode detaches the boast-UI, control-creature, right-stick look-around, special-camera-cycle, creature-movement and control-angle-watch input processes from the player's interface
// Leaf: UninitModeBoastUI
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00633880, length 73 bytes (+ int3 padding), `ret 8`.
// Identity: the PlayerModeInfo table filled by the dynamic initialiser at 0x0121B6xx stores 0x00633F10
// (CPlayer::InitModeBoastUI, landed) at 0x013B9088 and this function at 0x013B9098 -- the paired UninitFunc. The manifest
// label (_Dest_val<allocator<CEditLevelMerger::CConflict>>) is propagated across a byte-identical shape and wrong.
// Callees: the non-virtual CAInputProcess::RemoveFromInterface @ 0x00A0D2D0 on six scoped_ptr<CInputProcess*> members,
// in the same order InitModeBoastUI adds them (+0x1B4, +0x104, +0x170, +0x174, +0x150, +0x1E8).
// Layout: engine/CPlayer.h with the lane's CPlayer_Methods method carrier (as the landed UninitModeBerserk 0x006339B0).
// Shape: both hook arguments unused (`ret 8`); each member pointer goes straight into ecx.
#include "engine/CPlayer.h"
typedef long EPlayerMode; struct CProcessedInput;
struct CAInputProcess { void RemoveFromInterface(); };          // 0x00A0D2D0
struct CPlayer_Methods : CPlayer { void UninitModeBoastUI(EPlayerMode, CProcessedInput*); };
void CPlayer_Methods::UninitModeBoastUI(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    reinterpret_cast<CAInputProcess*>(InputProcessBoastUI_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreature_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessRightStickLookAround_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessCycleSpecialCameraModes_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessCreatureMovement_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessWatchForControlAngleChange_ptr)->RemoveFromInterface();
}