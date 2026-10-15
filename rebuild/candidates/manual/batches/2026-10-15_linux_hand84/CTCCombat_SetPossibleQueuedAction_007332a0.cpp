// CTCCombat::SetPossibleQueuedAction(CCreatureAction*) -- when the parent creature already has queued actions, the action is queued behind them and true is returned; otherwise false
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007332A0, length 40 bytes (+ int3 padding).
// Callees: CThingCreatureBase "has queued actions" @ 0x00661F20 (the action list at +0x108 is non-empty;
// landed under the propagated label CTCUserActionPointBase::IsActionPointFree), CThingCreatureBase queue
// action @ 0x00664490 (appends a 0x10-byte node to that list; manifest "PhysicsSegment_AssignSegment" is
// propagated).
// Layout = engine/CTCCombat.h: ParentCreature +0x24.
// Shape: `if (test) { call; return true; } return false;` -- the parent creature is re-read from `this`
// for each call (esi = this), `mov al,1` / `xor al,al` byte returns.
#include "engine/CTCCombat.h"

struct CCreatureAction;

struct CThingCreatureBase {
    bool HasQueuedActions();                                // 0x00661F20
    void QueueAction(CCreatureAction* action);              // 0x00664490
};

struct CTCCombat_Methods : CTCCombat {
    bool SetPossibleQueuedAction(CCreatureAction* action);
};

bool CTCCombat_Methods::SetPossibleQueuedAction(CCreatureAction* action)
{
    if (ParentCreature->HasQueuedActions()) {
        ParentCreature->QueueAction(action);
        return true;
    }
    return false;
}
