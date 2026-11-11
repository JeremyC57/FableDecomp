// CThingCreatureBase::GetAnimFromName(const CCharString& name, long index) const -- the index-th appearance animation called `name` passing the current creature mode's filter, else a random one of any group, else the first "DEFAULT"
// Leaf: ?GetAnimFromName@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00662A00, 259 bytes (+13 int3), `ret 8`.
// Identity: Ego_r.pdb ?GetAnimFromName@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@J@Z (donor 0x0053D8E0; engine/CThingCreatureBase.h
// FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION).
// Callees: CTCCreatureModeManager::GetCurrentMode 0x0073A6E0 and the mode's virtual GetAnimFilter (slot 4, +0x10),
//   CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2, CAnimationSet::AnimExists 0x005DC2E0, CAnimationSet::GetAnim 0x005DC340,
//   CAnimationSet::GetRandomAnim 0x005DCC00 (name, seed, filter, -1), CThing::GetPWorldSeed 0x004C7AE0, CCharString(const char*, long)
//   0x0099EBF0 ("DEFAULT" 0x01230554), ~CCharString 0x0099EAE0, the def's virtual Delete at a zero count.
// Identity: CAnimFilterNoGroup (vtable 0x0122FFDC) destroyed through the CAnimFilterBase store 0x0122FFCC. The filter overload is 0x00662B10.
// Layout: CThing PBaseDef +0x70; CThingCreatureBase PTCModeManager +0x134; CAppearanceDef Animation +0x34.
// Shape: the mode's filter is fetched before the sub-def; the def is released at the shared exit; the NoGroup filters are temporaries
//   in the dead index slot; VC7.1 hoists the common `push -1` above the second test.
// Name fix (agentNF8 2026-11-11): moved off the local CDefClassBase / CDefPointer / CAnimationSet / CTCCreatureModeManager /
// CThingCreatureBase stand-ins (and a same-TU stand-in GetSubDef body) onto the shared headers: 0x0042B0A2 binds GetSubDef<CAppearanceDef>
// (was ?GetSubDef@CDefClassBase@@QAE_NAAV?$CDefPointer@VCAppearanceDef@@@@@Z), 0x005DC340 / 0x005DCC00 bind the PDB GetAnim / GetRandomAnim
// (were GetAnimation / GetRandomAnimation returning pointers), 0x004C7AE0 binds ?GetPWorldSeed@CThing@@IBEPAKXZ (was
// ?GetRandomSeed@CThingCreatureBase@@QAEPAKXZ), and the row's own symbol is the PDB const one returning a reference.
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

const CAnimationEntry& CThingCreatureBase::GetAnimFromName(const CCharString& name, long index) const
{
    const CAnimFilterBase& filter = PTCModeManager->GetCurrentMode().GetAnimFilter();
    CDefPointer<const CAppearanceDef> def;
    PBaseDef->GetSubDef(&def);
    const CAnimationSet& animations = def->Animation;
    if (animations.AnimExists(name, filter))
        return animations.GetAnim(name, index, filter);
    if (animations.AnimExists(name, CAnimFilterNoGroup()))
        return animations.GetRandomAnim(name, *GetPWorldSeed(), CAnimFilterNoGroup(), -1);
    return animations.GetAnim("DEFAULT", 0, CAnimFilterNoGroup());
}
