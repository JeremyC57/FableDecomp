// CThingCreatureBase::GetAnimFromName(const CCharString& name, const CAnimFilterBase& filter, long index) const -- the index-th appearance animation called `name` passing the filter, else one of any group, else the first "DEFAULT"
// Leaf: ?GetAnimFromName@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@ABVCAnimFilterBase@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00662B10, 242 bytes (+14 int3), `ret 0Ch`.
// Identity: Ego_r.pdb ?GetAnimFromName@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@ABVCAnimFilterBase@@J@Z (donor 0x0053DA10;
// the index overload 0x00662A00 is donor 0x0053D8E0).
// Callees: CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2, CAnimationSet::AnimExists 0x005DC2E0 and CAnimationSet::GetAnim(const
//   CCharString&, long, const CAnimFilterBase&) const 0x005DC340, CCharString(const char*, long) 0x0099EBF0 ("DEFAULT" 0x01230554),
//   ~CCharString 0x0099EAE0, the def's virtual Delete at a zero count.
// Identity: the fallback filter is CAnimFilterNoGroup (vtable 0x0122FFDC) destroyed through the CAnimFilterBase store 0x0122FFCC.
// Layout: CThing PBaseDef +0x70; CAppearanceDef Animation +0x34.
// Shape: the sub-def out pointer is the zeroed `push ecx` word released at each of the three returns; the fallback filters are
//   temporaries in the dead filter argument slot; "DEFAULT" is passed as a plain literal so its implicit CCharString temporary is
//   built before the other arguments are pushed.
// Name fix (agentNF8 2026-11-11): moved off the local CDefClassBase / CDefPointer / CAnimationSet / CThingCreatureBase stand-ins (and a
// same-TU stand-in GetSubDef body) onto the shared headers: 0x0042B0A2 binds GetSubDef<CAppearanceDef> (was
// ?GetSubDef@CDefClassBase@@QAE_NAAV?$CDefPointer@VCAppearanceDef@@@@@Z), 0x005DC340 binds the PDB
// ?GetAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@JABVCAnimFilterBase@@@Z (was GetAnimation, returning a pointer), and the
// row's own symbol is the PDB const one returning a reference (was QAEPAVCAnimationEntry@@...).
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
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CRESOURCELIST
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAppearanceDef.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CAnimFilterNoGroup.h"

const CAnimationEntry& CThingCreatureBase::GetAnimFromName(const CCharString& name, const CAnimFilterBase& filter, long index) const
{
    CDefPointer<const CAppearanceDef> def;
    PBaseDef->GetSubDef(&def);
    const CAnimationSet& animations = def->Animation;
    if (animations.AnimExists(name, filter))
        return animations.GetAnim(name, index, filter);
    if (animations.AnimExists(name, CAnimFilterNoGroup()))
        return animations.GetAnim(name, index, CAnimFilterNoGroup());
    return animations.GetAnim("DEFAULT", 0, CAnimFilterNoGroup());
}
