// CGamePlayerInterface::IsGameActionEventInQueue(EGameAction, EInputButtonEventType, long) const -- the same query without the analogue value
// Leaf: ?IsGameActionEventInQueue@CGamePlayerInterface@@QBE_NW4EGameAction@@W4EInputButtonEventType@@J@Z
// Retail: Fable.exe @ 0x00446B10, 37 bytes. Forwards its three arguments plus a zeroed float local (by reference) to the four-argument
// overload (0x00445CB0, landed catalog row IsGameActionEventInQueue(EGameAction, EInputButtonEventType, long, float&)) and returns its result.
// Identity (two sources): Ego_r.pdb 0x00443F70 + FableWin.pdb ?IsGameActionEventInQueue@CGamePlayerInterface@@QBE_NW4EGameAction@@W4EInputButtonEventType@@J@Z
//   (const; enum parameters), declared by engine/CGamePlayerInterface.h; Ego_r order IsToggleSneak 0x004431D0 .. IsGameActionEventInQueue(3)
//   0x00443F70 matches retail 0x00445BA0 .. 0x00446B10; the landed binders 0x004A5A40, 0x00747DA0, 0x0077A430, 0x00805600, 0x008905B0,
//   0x008905E0, 0x00890610, 0x00890640 bind this spelling.
// NF171 (2026-11-20, q11704): defined on the shared header's own class (engine/CGamePlayerInterface.h retail path: FABLE_RETAIL_CGAMEPLAYERINTERFACE
//   declares this overload, _GAMEACTIONQUERIES the four-argument one; both enums MS-forward-declared there); was ?IsGameActionEventInQueue@CGamePlayerInterface_Methods@@.. on a local derived stand-in
//   with local EGameAction / EInputButtonEventType enums.
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_GAMEACTIONQUERIES
#include "engine/CGamePlayerInterface.h"

bool CGamePlayerInterface::IsGameActionEventInQueue(EGameAction action, EInputButtonEventType event_type, long player) const
{
    float value = 0.0f;
    return IsGameActionEventInQueue(action, event_type, player, value);
}
