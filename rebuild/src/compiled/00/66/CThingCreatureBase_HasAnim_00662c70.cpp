// CThingCreatureBase::HasAnim(const CCharString& name, const CAnimFilterBase& filter) const -- whether the animation set of the creature's appearance definition holds an animation of that name passing the filter
// Leaf: ?HasAnim@CThingCreatureBase@@QBE_NABVCCharString@@ABVCAnimFilterBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00662C70, length 72 bytes (+ int3 padding).
// Identity: Ego_r.pdb ?HasAnim@CThingCreatureBase@@QBE_NABVCCharString@@ABVCAnimFilterBase@@@Z (donor 0x0053DB70).
// Callees: CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2 (on CThing::PBaseDef +0x70),
// CAnimationSet::AnimExists(const CCharString&, const CAnimFilterBase&) const 0x005DC2E0, the def's virtual Delete at a zero count.
// Layout: CThing PBaseDef +0x70 (engine/CThing.h), CAppearanceDef::Animation +0x34 (engine/CAppearanceDef.h).
// Shape: the def pointer is zero-initialised in the `push ecx` slot and filled by GetSubDef (the header's noinline template body,
// out of line in retail); retail loads it ONCE into esi before the query (`lea ecx,[esi+34h]`) and releases through that register
// (`dec [esi+4] ; jne`), the bool result riding in bl.
// Name fix (agentNF8 2026-11-11): moved off the local CDefClassBase / CDefPointer / CThing stand-ins (with a same-TU stand-in
// GetSubDef body binding ?GetSubDef@CDefClassBase@@QAE_NAAV?$CDefPointer@VCAppearanceDef@@@@@Z) onto the shared headers, so
// 0x0042B0A2 binds ??$GetSubDef@VCAppearanceDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCAppearanceDef@@@@@Z.
#define FABLE_RETAIL_CCHARSTRING
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
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CRESOURCELIST
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAppearanceDef.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CDefinitionManager.h"

bool CThingCreatureBase::HasAnim(const CCharString& name, const CAnimFilterBase& filter) const
{
    CDefPointer<const CAppearanceDef> def;
    PBaseDef->GetSubDef(&def);
    return def->Animation.AnimExists(name, filter);
}
