// CThingCreatureBase::OnKill() -- when the creature has a mode manager (component 0x31), drop all its modes; then run the base thing's OnKill
// Leaf: ?OnKill@CThingCreatureBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00664370, length 70 bytes (+ int3 padding).
// Name: Ego_r.pdb 0x00540300 / FableWin.pdb 0x01D935F0 ?OnKill@CThingCreatureBase@@UAEXXZ (slot 12 of the CThingPlayerCreature vtable 0x012457FC).
// NF120 (PRIORITY q3944): component 0x31 is TCI_ENTITY_MODE_MANAGER (CTCCreatureModeManager::StatGetInterfaceType) and 0x0073AE70 is its
//   ?RemoveAllModes@CTCCreatureModeManager@@QAEXXZ (both PDBs); the row moved from local CThing / CVectorMap / CTCPrimitiveOwner stand-ins
//   (CTCPrimitiveOwner::ClearAllChildPrimitives, the propagated manifest label) to the shared headers.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inline GetTC), CTCCreatureModeManager::RemoveAllModes @ 0x0073AE70,
//   CThing::OnKill @ 0x004C7CF0 (the qualified base call).
// Shape: `if (HasTC(k)) GetTC(k).f();` with the inlined map find whose component is loaded straight into ecx, then the qualified base call.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ONKILL
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER_REMOVEALLMODES
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCCreatureModeManager.h"

void CThingCreatureBase::OnKill()
{
    if (HasTC(TCI_ENTITY_MODE_MANAGER))
        static_cast<CTCCreatureModeManager&>(GetTC(TCI_ENTITY_MODE_MANAGER)).RemoveAllModes();
    CThing::OnKill();
}
