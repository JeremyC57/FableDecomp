// CThingCreatureBase::GetRandomAnimFromNameWithFilter(const CCharString& name, const CAnimFilterBase& filter) const -- a random animation called `name` that passes the filter from the creature's appearance animation set, else the set's first "DEFAULT" animation of any group
// Leaf: ?GetRandomAnimFromNameWithFilter@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@ABVCAnimFilterBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00662EF0, length 167 bytes, `ret 8`.
// Identity: Ego_r.pdb / FableWin.pdb ?GetRandomAnimFromNameWithFilter@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@ABVCAnimFilterBase@@@Z
// (engine/CThingCreatureBase.h FABLE_RETAIL_CTHINGCREATUREBASE_ANIMFILTER; the landed caller 0x008A8C60 binds it).
// Callees: CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2, CAnimationSet::AnimExists 0x005DC2E0, CAnimationSet::GetRandomAnim 0x005DCC00
//   (name, seed, filter, -1), CAnimationSet::GetAnim 0x005DC340, CThing::GetPWorldSeed 0x004C7AE0, CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, the def's virtual Delete.
// Identity: the fallback filter is CAnimFilterNoGroup (vtable 0x0122FFDC) destroyed through the CAnimFilterBase store 0x0122FFCC.
// Layout: CThing PBaseDef +0x70; CAppearanceDef Animation +0x34.
// Shape: the sub-def out pointer is the zeroed `push ecx` frame word; the `-1` push is shared by the random pick's last argument and
//   the "DEFAULT" string's length; the fallback string and filter temporaries land in the dead argument slots.
// Name fix (agentNF8 2026-11-11): moved off the local CDefClassBase / CDefPointer / CAnimationSet / CThingCreatureBase stand-ins (and a
// same-TU stand-in GetSubDef body) onto the shared headers: 0x0042B0A2 binds GetSubDef<CAppearanceDef> (was
// ?GetSubDef@CThingBaseDef@@...), 0x005DC340 / 0x005DCC00 bind the PDB GetAnim / GetRandomAnim (were GetAnimation /
// GetRandomAnimation_005dcc00), 0x004C7AE0 binds ?GetPWorldSeed@CThing@@IBEPAKXZ (was GetRandomSeed_004c7ae0), and the row's own symbol
// is the PDB const one returning a reference (was QAEPBVCAnimationEntry@@...).
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
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CANIMATIONSET_ANIMS
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMFILTER
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CRESOURCELIST
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAppearanceDef.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CAnimFilterNoGroup.h"
#include "engine/CTCCreatureModeManager.h"

const CAnimationEntry& CThingCreatureBase::GetRandomAnimFromNameWithFilter(const CCharString& name, const CAnimFilterBase& filter) const
{
    CDefPointer<const CAppearanceDef> def;
    PBaseDef->GetSubDef(&def);
    const CAnimationSet& animations = def->Animation;
    if (animations.AnimExists(name, filter))
        return animations.GetRandomAnim(name, *GetPWorldSeed(), filter, -1);
    return animations.GetAnim("DEFAULT", 0, CAnimFilterNoGroup());
}
