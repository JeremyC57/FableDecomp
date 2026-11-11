// CThingCreatureBase::GetCombatAnimationSet() const -- the animation set of the creature's appearance definition
// Leaf: ?GetCombatAnimationSet@CThingCreatureBase@@QBEABVCAnimationSet@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00663260, length 49 bytes (+ int3 padding).
// Identity: Ego_r.pdb ?GetCombatAnimationSet@CThingCreatureBase@@QBEABVCAnimationSet@@XZ (donor 0x0053E1B0).
// Callees: CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2, the def's virtual Delete at a zero count.
// Layout: CThing PBaseDef +0x70; CAppearanceDef Animation +0x34.
// Shape: the def is released before the function returns the address of its animation set (as retail: the set lives as long as the
// definition manager keeps the def).
// Name fix (agentNF8 2026-11-11): moved off the local CDefClassBase / CDefPointer / CThingCreatureBase stand-ins (and a same-TU stand-in
// GetSubDef body) onto the shared headers, so 0x0042B0A2 binds GetSubDef<CAppearanceDef> (was
// ?GetSubDef@CDefClassBase@@QAE_NAAV?$CDefPointer@VCAppearanceDef@@@@@Z).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMQUERY
#define FABLE_RETAIL_CANIMATIONSET_ANIMEXISTS
#define FABLE_RETAIL_CANIMATIONSET_QUERIES
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CANIMATIONSET_ANIMS
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CRESOURCELIST
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAppearanceDef.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CAnimFilterNoGroup.h"
#include "engine/CTCCreatureModeManager.h"

const CAnimationSet& CThingCreatureBase::GetCombatAnimationSet() const
{
    CDefPointer<const CAppearanceDef> appearance;
    PBaseDef->GetSubDef(&appearance);
    return appearance->Animation;
}
