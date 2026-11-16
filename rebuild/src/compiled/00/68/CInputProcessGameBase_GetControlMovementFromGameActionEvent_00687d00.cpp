// CInputProcessGameBase::GetControlMovementFromGameActionEvent(EGameAction, const CInputEvent&, C2DVector&) const -- forward to the player interface (+0x24).
// Leaf: ?GetControlMovementFromGameActionEvent@CInputProcessGameBase@@QBE_NW4EGameAction@@ABVCInputEvent@@AAVC2DVector@@@Z
// Retail: `mov ecx,[ecx+24h] ; jmp 0x00445C30` (the three stack arguments are passed through untouched).
// NF37 (2026-11-16): moved off the T::Run stand-in onto the shared engine/CInputProcessGameBase.h (FABLE_RETAIL_INPUTPROCESS_MOVEMENT;
// Ego_r.pdb ?GetControlMovementFromGameActionEvent@CInputProcessGameBase@@QBE_NW4EGameAction@@ABVCInputEvent@@AAVC2DVector@@@Z, the header
// names retail 0x00687D00). Callee CGamePlayerInterface::GetControlMovementFromGameActionEvent 0x00445C30 (Ego_r 0x00443A60 / FableWin 0x01A995F0).
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_MOVEMENT
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_GAMEACTIONQUERIES
#include "engine/CGamePlayerInterface.h"
#include "engine/CInputProcessGameBase.h"

bool CInputProcessGameBase::GetControlMovementFromGameActionEvent(EGameAction action, const CInputEvent& event, C2DVector& movement) const
{
    return PGamePlayerInterface->GetControlMovementFromGameActionEvent(action, event, movement);
}
