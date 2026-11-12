// CPlayer::UninitModeFishing(EPlayerMode mode, CProcessedInput* input) -- leave the mode: take the mode's input process (InputProcessFishing, +0x1A8) off the player interface
// Leaf: ?UninitModeFishing@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00633780, `ret 8`.
// Identity: Ego_r.pdb / FableWin.pdb ?UninitModeFishing@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z (private; one of the 94 PlayerModeInfo
//   Init/Uninit members, engine/CPlayer.h); the landed caller 0x0121AF20 (CPlayer::`dynamic initializer for 'PlayerModeInfo'' (object symbol _$E1 of its TU)) binds this exact spelling (rebuild/oracles/
//   reloc_bindings.tsv). Ego_r CPlayer InputProcessFishing is boost::scoped_ptr<CInputProcessFishing> at +0x1A8 (engine/CPlayer.h).
//   Was the stand-in CClass_00633780::ForwardThroughMember_00633780.
// Body: InputProcessFishing->RemoveFromInterface() (CAInputProcess::RemoveFromInterface 0x00A0D2D0, Ego_r.pdb ?RemoveFromInterface@CAInputProcess@@QAEXXZ).
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_INPUTPROCESSES
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_REMOVEFROMINTERFACE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CPlayer.h"
#include "engine/CInputProcessFishing.h"

void CPlayer::UninitModeFishing(EPlayerMode mode, CProcessedInput* input)
{
    InputProcessFishing->RemoveFromInterface();
}
