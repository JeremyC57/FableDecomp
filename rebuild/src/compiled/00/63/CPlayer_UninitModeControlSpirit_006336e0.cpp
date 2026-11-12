// CPlayer::UninitModeControlSpirit(EPlayerMode mode, CProcessedInput* input) -- leave the mode: take the mode's input process (InputProcessControlSpirit, +0x198) off the player interface
// Leaf: ?UninitModeControlSpirit@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006336E0, `ret 8`.
// Identity: Ego_r.pdb / FableWin.pdb ?UninitModeControlSpirit@CPlayer@@AAEXW4EPlayerMode@@PAVCProcessedInput@@@Z (private; one of the 94 PlayerModeInfo
//   Init/Uninit members, engine/CPlayer.h); the landed caller 0x0121AF20 (CPlayer::`dynamic initializer for 'PlayerModeInfo'' (object symbol _$E1 of its TU)) binds this exact spelling (rebuild/oracles/
//   reloc_bindings.tsv). Ego_r CPlayer InputProcessControlSpirit is boost::scoped_ptr<CInputProcessControlSpirit> at +0x198 (engine/CPlayer.h).
//   Was the stand-in CClass_006336e0::ForwardThroughMember_006336e0.
// Body: InputProcessControlSpirit->RemoveFromInterface() (CAInputProcess::RemoveFromInterface 0x00A0D2D0, Ego_r.pdb ?RemoveFromInterface@CAInputProcess@@QAEXXZ).
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_INPUTPROCESSES
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_REMOVEFROMINTERFACE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CPlayer.h"
#include "engine/CInputProcessControlSpirit.h"

void CPlayer::UninitModeControlSpirit(EPlayerMode mode, CProcessedInput* input)
{
    InputProcessControlSpirit->RemoveFromInterface();
}
