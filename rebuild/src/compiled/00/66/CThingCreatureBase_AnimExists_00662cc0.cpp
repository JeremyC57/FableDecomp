// CThingCreatureBase::AnimExists(const CCharString& name, bool useModeFilterOnly) const -- whether the appearance animation set has an animation called `name` passing the current creature mode's filter, else (unless restricted to the mode filter) one of any group
// Leaf: ?AnimExists@CThingCreatureBase@@UBE_NABVCCharString@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00662CC0, length 181 bytes, `ret 8`, bool in al; vtable slot 73 (CThingPlayerCreature
//   vtable 0x012457FC inherits it).
// Identity: Ego_r.pdb / FableWin.pdb ?AnimExists@CThingCreatureBase@@UBE_NABVCCharString@@_N@Z (public const virtual).
// Callees: CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2, CTCCreatureModeManager::GetCurrentMode 0x0073A6E0, the mode's
//   GetAnimFilter (CModeBase virtual slot 4, +0x10), CAnimationSet::AnimExists 0x005DC2E0, the def's Delete (slot 1).
// Identity: the fallback filter is a CAnimFilterNoGroup temporary (vtable 0x0122FFDC).
// Layout: CThing PBaseDef +0x70; CThingCreatureBase PTCModeManager +0x134 (Ego_r +0x158); CAppearanceDef Animation +0x34.
// Name fix (agentNF9 2026-11-11): moved off the local CThingCreatureBase / CTCCreatureModeManager / CAnimationSet / CAppearanceDef models
//   (a same-TU stand-in GetSubDef body, a CHeldDef holder, the descriptive GetAnimFilter_slot4) onto the shared headers; 0x0042B0A2 binds
//   ??$GetSubDef@VCAppearanceDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCAppearanceDef@@@@@Z, the mode filter is
//   CTCCreatureModeManager::CModeBase::GetAnimFilter (engine/CTCCreatureModeManager.h FABLE_RETAIL_CMODEBASE_VIRTUALS, slot 4).
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
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMEXISTS
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CAPPEARANCEDEF_COMPLETE
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

bool CThingCreatureBase::AnimExists(const CCharString& name, bool useModeFilterOnly) const
{
    CDefPointer<const CAppearanceDef> appearance;
    PBaseDef->GetSubDef(&appearance);
    const CTCCreatureModeManager::CModeBase& mode = PTCModeManager->GetCurrentMode();
    const CAnimationSet& set = appearance->Animation;
    if (set.AnimExists(name, mode.GetAnimFilter()))
        return true;
    if (!useModeFilterOnly)
        return set.AnimExists(name, CAnimFilterNoGroup());
    return false;
}
