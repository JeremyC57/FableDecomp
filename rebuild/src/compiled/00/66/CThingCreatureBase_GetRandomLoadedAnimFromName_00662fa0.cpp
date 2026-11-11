// CThingCreatureBase::GetRandomLoadedAnimFromName(const CCharString& name, bool flag) const -- a random loaded appearance animation called `name` passing the current creature mode's filter, else one of any group, else the first "DEFAULT"
// Leaf: ?GetRandomLoadedAnimFromName@CThingCreatureBase@@QBEPBVCAnimationEntry@@ABVCCharString@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00662FA0.
// Identity: Ego_r.pdb ?GetRandomLoadedAnimFromName@CThingCreatureBase@@QBEPBVCAnimationEntry@@ABVCCharString@@_N@Z (donor 0x0053DEC0); the
// flag is forwarded to ?GetRandomLoadedAnim@CAnimationSet@@QBEPBVCAnimationEntry@@ABVCCharString@@AAK_NABVCAnimFilterBase@@@Z.
// Callees: CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2, CTCCreatureModeManager::GetCurrentMode 0x0073A6E0 and the mode's
//   GetAnimFilter (slot 4), CAnimationSet::AnimExists 0x005DC2E0, CAnimationSet::GetRandomLoadedAnim 0x005DCD10 (name, seed, flag, filter),
//   CAnimationSet::GetAnim 0x005DC340, CThing::GetPWorldSeed 0x004C7AE0, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0.
// Layout: CThing PBaseDef +0x70; CThingCreatureBase PTCModeManager +0x134; CAppearanceDef Animation +0x34.
// Shape: the sub-def is fetched before the mode's filter (the reverse of 0x00662A00).
// Name fix (agentNF8 2026-11-11): moved off the local stand-ins (and a same-TU stand-in GetSubDef body) onto the shared headers: 0x0042B0A2
// binds GetSubDef<CAppearanceDef>, 0x005DCD10 / 0x005DC340 the PDB GetRandomLoadedAnim / GetAnim (were GetRandomLoadedAnimation /
// GetAnimation), 0x004C7AE0 ?GetPWorldSeed@CThing@@IBEPAKXZ; the row's own symbol is the PDB const one with the bool flag (was
// QAEPAVCAnimationEntry@@ABVCCharString@@J@Z).
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

const CAnimationEntry* CThingCreatureBase::GetRandomLoadedAnimFromName(const CCharString& name, bool flag) const
{
    CDefPointer<const CAppearanceDef> def;
    PBaseDef->GetSubDef(&def);
    const CAnimFilterBase& filter = PTCModeManager->GetCurrentMode().GetAnimFilter();
    const CAnimationSet& animations = def->Animation;
    if (animations.AnimExists(name, filter))
        return animations.GetRandomLoadedAnim(name, *GetPWorldSeed(), flag, filter);
    if (animations.AnimExists(name, CAnimFilterNoGroup()))
        return animations.GetRandomLoadedAnim(name, *GetPWorldSeed(), flag, CAnimFilterNoGroup());
    return &animations.GetAnim("DEFAULT", 0, CAnimFilterNoGroup());
}
