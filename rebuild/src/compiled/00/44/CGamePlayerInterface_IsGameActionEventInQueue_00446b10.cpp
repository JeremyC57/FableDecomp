// CGamePlayerInterface::IsGameActionEventInQueue(EGameAction, EInputButtonEventType, long) const -- the same query without the analogue value
// Leaf: ?IsGameActionEventInQueue@CGamePlayerInterface_Methods@@QBE_NW4EGameAction@@W4EInputButtonEventType@@J@Z
// Retail: Fable.exe @ 0x00446B10, 37 bytes; Ego_r.pdb
// ?IsGameActionEventInQueue@CGamePlayerInterface@@QBE_NW4EGameAction@@W4EInputButtonEventType@@J@Z. Forwards its three
// arguments plus a zeroed float local (by reference) to the four-argument overload (0x00445CB0, landed catalog row
// IsGameActionEventInQueue(EGameAction, EInputButtonEventType, long, float&)) and returns its result.
// Layout: engine/CGamePlayerInterface.h (no members touched).
#include "engine/CGamePlayerInterface.h"
enum EGameAction { GAME_ACTION_NULL };
enum EInputButtonEventType { INPUT_BUTTON_EVENT_NULL };
struct CGamePlayerInterface_Methods : CGamePlayerInterface {
    bool IsGameActionEventInQueue(EGameAction action, EInputButtonEventType event_type, long player, float& value) const;  // 0x00445CB0
    bool IsGameActionEventInQueue(EGameAction action, EInputButtonEventType event_type, long player) const;
};
bool CGamePlayerInterface_Methods::IsGameActionEventInQueue(EGameAction action, EInputButtonEventType event_type, long player) const
{
    float value = 0.0f;
    return IsGameActionEventInQueue(action, event_type, player, value);
}