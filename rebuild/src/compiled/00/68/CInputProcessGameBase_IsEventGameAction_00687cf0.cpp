// CInputProcessGameBase::IsEventGameAction(EGameAction action, const CInputEvent& event) const -- forward to the player interface (+0x24).
// Leaf: ?IsEventGameAction@CInputProcessGameBase@@QBE_NW4EGameAction@@ABVCInputEvent@@@Z
// Retail: `mov ecx,[ecx+24h] ; jmp 0x00445BD0` (the two stack arguments are passed through untouched).
// NF37 (2026-11-16): moved off the T::Run stand-in onto the shared engine/CInputProcessGameBase.h (FABLE_RETAIL_INPUTPROCESS_ZTARGET;
// Ego_r.pdb / FableWin.pdb ?IsEventGameAction@CInputProcessGameBase@@QBE_NW4EGameAction@@ABVCInputEvent@@@Z, Ego_r order IsEventGameAction /
// GetControlMovementFromGameActionEvent 0x10 apart as retail 0x00687CF0 / 0x00687D00). Callee CGamePlayerInterface::IsEventGameAction
// 0x00445BD0 (Ego_r 0x004439F0 / FableWin 0x01A993C0).
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_GAMEACTIONQUERIES
#include "engine/CGamePlayerInterface.h"
#include "engine/CInputProcessGameBase.h"

bool CInputProcessGameBase::IsEventGameAction(EGameAction action, const CInputEvent& event) const
{
    return PGamePlayerInterface->IsEventGameAction(action, event);
}
