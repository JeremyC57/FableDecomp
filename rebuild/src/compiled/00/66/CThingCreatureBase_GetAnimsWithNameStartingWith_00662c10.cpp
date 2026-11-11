// CThingCreatureBase::GetAnimsWithNameStartingWith(const CCharString& prefix, vector<const CAnimationEntry*>& anims) const -- collect the appearance animations whose names start with the prefix (any group)
// Leaf: ?GetAnimsWithNameStartingWith@CThingCreatureBase@@QBEXABVCCharString@@AAV?$vector@PBVCAnimationEntry@@V?$allocator@PBVCAnimationEntry@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00662C10.
// Identity: Ego_r.pdb ?GetAnimsWithNameStartingWith@CThingCreatureBase@@QBEXABVCCharString@@AAV?$vector@PBVCAnimationEntry@@V?$allocator@PBVCAnimationEntry@@@std@@@std@@@Z
// (donor 0x0053DB10).
// Callees: CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2, CAnimationSet::GetAnimsWithNameStartingWith(const CCharString&, vector&,
// const CAnimFilterBase&) const 0x005DCB00 with a CAnimFilterNoGroup temporary, the def's virtual Delete at a zero count.
// Layout: CThing PBaseDef +0x70; CAppearanceDef Animation +0x34.
// Name fix (agentNF8 2026-11-11): moved off the local CDefClassBase / CDefPointer / CAnimationSet / CThingCreatureBase stand-ins (and a
// same-TU stand-in GetSubDef body) onto the shared headers, so 0x0042B0A2 binds GetSubDef<CAppearanceDef> (was
// ?GetSubDef@CDefClassBase@@QAE_NAAV?$CDefPointer@VCAppearanceDef@@@@@Z) and 0x005DCB00 the PDB spelling.
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

void CThingCreatureBase::GetAnimsWithNameStartingWith(const CCharString& prefix, std::vector<const CAnimationEntry*>& anims) const
{
    CDefPointer<const CAppearanceDef> def;
    PBaseDef->GetSubDef(&def);
    def->Animation.GetAnimsWithNameStartingWith(prefix, anims, CAnimFilterNoGroup());
}
