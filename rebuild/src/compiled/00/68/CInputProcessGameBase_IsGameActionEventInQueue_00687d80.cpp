// Leaf: ?IsGameActionEventInQueue@CInputProcessGameBase@@IBE_NW4EGameAction@@W4EInputButtonEventType@@AAM@Z
// CInputProcessGameBase::IsGameActionEventInQueue(long action, long buttonEventType, float* pTime) -- forward to the player interface (+0x24) with this player's number (CPlayer +0x228) inserted as the third argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00687D80, length 37 bytes (+ int3 padding).
// Callees: CPlayer::GetJoystickDeviceNumber @ 0x00487510 (`mov eax,[ecx+228h]`; the label "CWorld::DrawGetEnvironment" is propagated),
// CGamePlayerInterface::IsGameActionEventInQueue(long, long, long, float*) @ 0x00445CB0.
// Layout: engine/CInputProcessGameBase.h (MyPlayer +0x18, PGamePlayerInterface +0x24). Shape: the arguments are pushed right to left --
// the time pointer first, then the player number call, then the two words reloaded from their slots; the bool rides through al, `ret 0Ch`.
// NF37 (2026-11-16): moved onto the shared engine/CInputProcessGameBase.h (FABLE_RETAIL_INPUTPROCESS_EVENTINQUEUE; Ego_r.pdb / FableWin.pdb
// ?IsGameActionEventInQueue@CInputProcessGameBase@@IBE_NW4EGameAction@@W4EInputButtonEventType@@AAM@Z, protected const) and the callee
// CGamePlayerInterface::IsGameActionEventInQueue(EGameAction, EInputButtonEventType, long, float&) const 0x00445CB0 (Ego_r 0x00443AF0).
#define FABLE_RETAIL_PLAYER
#include "engine/CPlayer.h"
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_GAMEACTIONQUERIES
#include "engine/CGamePlayerInterface.h"
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_EVENTINQUEUE
#include "engine/CInputProcessGameBase.h"

bool CInputProcessGameBase::IsGameActionEventInQueue(EGameAction action, EInputButtonEventType type, float& time) const
{
    return PGamePlayerInterface->IsGameActionEventInQueue(action, type, MyPlayer->GetJoystickDeviceNumber(), time);
}
