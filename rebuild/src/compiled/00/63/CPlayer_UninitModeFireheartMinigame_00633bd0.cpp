// CPlayer::UninitModeFireheartMinigame(EPlayerMode mode, CProcessedInput* input) -- leave the mode: take the mode's input process (InputProcessFireheartMinigame, +0x1F0) off the player interface
// Leaf: ?UninitModeFireheartMinigame@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00633BD0, `ret 8`.
// Identity: Ego_r.pdb / FableWin.pdb ?UninitModeFireheartMinigame@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z (private; one of the 94 PlayerModeInfo
//   Init/Uninit members, engine/CPlayer.h); the landed caller 0x0121AF20 (CPlayer::`dynamic initializer for 'PlayerModeInfo'' (object symbol _$E1 of its TU)) binds this exact spelling (rebuild/oracles/
//   reloc_bindings.tsv). Ego_r CPlayer InputProcessFireheartMinigame is boost::scoped_ptr<CInputProcessFireheartMinigame> at +0x1F0 (engine/CPlayer.h).
//   Was the stand-in CClass_00633bd0::ForwardThroughMember_00633bd0.
// Body: InputProcessFireheartMinigame->RemoveFromInterface() (CAInputProcess::RemoveFromInterface 0x00A0D2D0, Ego_r.pdb ?RemoveFromInterface@CAInputProcess@@QAEXXZ).
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_INPUTPROCESSES
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_REMOVEFROMINTERFACE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CPlayer.h"
#include "engine/CInputProcessFireheartMinigame.h"

void CPlayer::UninitModeFireheartMinigame(EPlayerMode mode, CProcessedInput* input)
{
    InputProcessFireheartMinigame->RemoveFromInterface();
}
