// CTCBerserk::CancelForCutscene() -- once only: mark IsInCutscene, finish the parent creature's current action, clean up the ability and remove this TC (TCI_BERSERK) from the parent
// Leaf: ?CancelForCutscene@CTCBerserk@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078D210, length 49 bytes (+ int3 padding).
// Identity: Ego_r.pdb / FableWin.pdb ?CancelForCutscene@CTCBerserk@@QAEXXZ (public).
// Callees: CThingCreatureBase::FinishCurrentAction @ 0x00662650, CTCBerserk::CleanupAbility(bool) @ 0x0078CD80
// (?CleanupAbility@CTCBerserk@@QAEX_N@Z), CThing::RemoveTC(ETCInterfaceType) @ 0x004C9840.
// Layout: engine/CTCBerserk.h retail branch (IsInCutscene +0x72); CTCBase ParentThing +0x04.
// Shape: the parent pointer is read into a local (edi) before the flag store; the guard is an `if`
// around the body (the early-out shares the single exit).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CTCBERSERK_HIT_EFFECT
#define FABLE_RETAIL_CTCBERSERK_FRAMEUPDATE
#define FABLE_RETAIL_CTCBERSERK_CANCELFORCUTSCENE
#include "fable_compiler.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCBerserk.h"

void CTCBerserk::CancelForCutscene()
{
    if (!IsInCutscene) {
        CThingCreatureBase* parent = (CThingCreatureBase*)ParentThing;
        IsInCutscene = true;
        parent->FinishCurrentAction();
        CleanupAbility(true);
        parent->RemoveTC(TCI_BERSERK);
    }
}
