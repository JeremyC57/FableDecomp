// CThingCreatureBase::IsCombatAnimationValid(const CCharString& name) const -- does the creature's appearance animation set hold this animation (no group filter)
// Leaf: ?IsCombatAnimationValid@CThingCreatureBase@@QBE_NABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006632A0, 154 bytes (`ret 4`).
// Identity: Ego_r.pdb ?IsCombatAnimationValid@CThingCreatureBase@@QBE_NABVCCharString@@@Z (donor 0x0053E1F0, right after
// GetCombatAnimationSet 0x0053E1B0 = retail 0x00663260); the callee 0x005DC2E0 is ?AnimExists@CAnimationSet@@QBE_NABVCCharString@@ABVCAnimFilterBase@@@Z,
// so the argument is the animation's name (the landed `long` was a 4-byte stand-in for the reference).
// Callees: CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2 (twice), CAnimationSet::AnimExists 0x005DC2E0.
// Identity: the frame word 0x0122FFDC is a CAnimFilterNoGroup temporary.
// Layout: CThing PBaseDef +0x70; CAppearanceDef Animation +0x34.
// Shape: the first GetSubDef only tests that the creature has an appearance def; the animation set is then taken from a second
//   CDefPointer in its own block (GetCombatAnimationSet's body), released before the set is used; each return arm releases the
//   first def through its own inline dtor copy.
// Name fix (agentNF8 2026-11-11): moved off the local stand-ins (a ReleaseDef_s7 stand-in callee, local CDefClassBase / CAnimationSet)
// onto the shared headers: 0x0042B0A2 binds GetSubDef<CAppearanceDef>, 0x005DC2E0 the PDB AnimExists (was HasAnimation(long, ...)), and
// the row's own symbol is the PDB const one taking the name (was QAE_NJ@Z).
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

bool CThingCreatureBase::IsCombatAnimationValid(const CCharString& name) const
{
    CDefPointer<const CAppearanceDef> def;
    if (PBaseDef->GetSubDef(&def)) {
        const CAnimationSet* set;
        {
            CDefPointer<const CAppearanceDef> appearance;
            PBaseDef->GetSubDef(&appearance);
            set = &appearance->Animation;
        }
        if (set->AnimExists(name, CAnimFilterNoGroup()))
            return true;
    }
    return false;
}
