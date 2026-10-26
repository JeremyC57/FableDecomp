// CPlayer::UninitModeParalysed(EPlayerMode mode, CProcessedInput* input) -- leaving the paralysed mode detaches the paralysed, right-stick creature, right-stick look-around, special-camera-cycle, creature-movement and debug input processes from the player's interface
// Leaf: UninitModeParalysed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00633830, length 73 bytes (+ int3 padding), `ret 8`.
// Identity: the PlayerModeInfo table filled by the dynamic initialiser at 0x0121B4xx stores 0x006337C0
// (CPlayer::InitModeParalysed, landed) at 0x013B9068 and this function at 0x013B9078 -- the paired UninitFunc. The manifest
// label (_Dest_val<allocator<CEditLevelMerger::CConflict>>) is propagated across a byte-identical shape and wrong.
// Callees: the non-virtual CAInputProcess::RemoveFromInterface @ 0x00A0D2D0 on six scoped_ptr<CInputProcess*> members,
// in the same order InitModeParalysed adds them (+0x1B0, +0x16C, +0x170, +0x174, +0x150, +0x15C).
// Layout: engine/CPlayer.h with the lane's CPlayer_Methods method carrier (as the landed UninitModeBerserk 0x006339B0).
// Shape: both hook arguments unused (`ret 8`); each member pointer goes straight into ecx.
#include "engine/CPlayer.h"
typedef long EPlayerMode; class CProcessedInput;
class CAInputProcess { public: void RemoveFromInterface(); };          // 0x00A0D2D0
struct CPlayer_Methods : CPlayer { void UninitModeParalysed(EPlayerMode, CProcessedInput*); };
void CPlayer_Methods::UninitModeParalysed(EPlayerMode mode, CProcessedInput* input)
{
    (void)mode; (void)input;
    reinterpret_cast<CAInputProcess*>(InputProcessParalysed_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessControlCreatureRightStick_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessRightStickLookAround_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessCycleSpecialCameraModes_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessCreatureMovement_ptr)->RemoveFromInterface();
    reinterpret_cast<CAInputProcess*>(InputProcessDebugControls_ptr)->RemoveFromInterface();
}