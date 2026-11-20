// NCreatureMode::CModeDragonBase::OnActivate() -- the dragon starts flying: gravity off (the physics component's slot 38,
//   EnableGravity(false)), then the base activation (CModeBase::OnActivate 0x0073AAC0, a tail jump)
// Leaf: ?OnActivate@CModeDragonBase@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E7290; CModeDragonBase vtable slot 5 (.rdata 0x012684F8 + 0x14).
// Name: Ego_r.pdb 0x0090EE30 + FableWin.pdb ?OnActivate@CModeDragonBase@NCreatureMode@@UAEXXZ (public virtual, slot 5; engine/CModeDragonBase.h
//   FABLE_RETAIL_CMODEDRAGONBASE_MODEBASE). The mirror of the landed OnDeactivate 0x008E72B0 (gravity back on, then the base call).
// Naming (NF126, 2026-11-20): was a July local model (`struct CModeBase { void* vfp; }`, `Mid` / `VObj` with v00..v37 placeholder virtuals,
//   ParentThing typed Mid*) defining the global ?OnActivate@CModeDragonBase@@QAEXXZ; moved onto engine/CModeDragonBase.h (the shared nested
//   CTCCreatureModeManager::CModeBase, CThing::GetPhysicsTC, CTCPhysicsBase::EnableGravity, as the landed 0x008E72B0). The base call now binds
//   ?OnActivate@CModeBase@CTCCreatureModeManager@@UAEXXZ.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEDRAGONBASE_MODEBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CModeDragonBase.h"

void NCreatureMode::CModeDragonBase::OnActivate()
{
    ParentThing.GetPhysicsTC()->EnableGravity(false);
    CTCCreatureModeManager::CModeBase::OnActivate();
}
