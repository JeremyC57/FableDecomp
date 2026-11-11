// CThingCreatureBase::GetRandomCombatAnimationFromName(const CCharString& name) const -- collect every combat-animation-set entry called `name` and return the one GetAppropriateCombatAnimationFromList picks
// Leaf: ?GetRandomCombatAnimationFromName@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00667B30, 204 bytes, `ret 4`, the chosen entry's address in eax.
// Identity: Ego_r.pdb / FableWin.pdb ?GetRandomCombatAnimationFromName@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@@Z: Ego_r
//   donor order GetRandomCombatAnimationFromName 0x00542250 / GetPrefferedCombatDistance 0x00542380 = retail 0x00667B30 / 0x00667C00
//   (landed); the body returns a reference to the picked vector element (const member, `ret 4`). The landed spelling
//   "GetAppropriateCombatAnimation" was descriptive (RENAMES.tsv).
// Callees (PDB spellings): CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2 (in the auto-inlined same-TU GetCombatAnimationSet,
//   out of line at 0x00663260), CAnimationSet::FindFirstAnimWithName 0x005DC020, vector<const CAnimationEntry*>::_M_insert_overflow
//   0x005E1420 (the push_back growth), CThingCreatureBase::GetAppropriateCombatAnimationFromList 0x00666B50, free 0x00BFEA14 (the
//   vector's storage), the def's virtual release.
// Layout: CThing PBaseDef +0x70; CAppearanceDef Animation +0x34 (CAnimationSet Anims: 0x14-byte pair<unsigned long, CAnimationEntry>
//   records, finish +4).
// Shape: the def is released inside GetCombatAnimationSet before the lookup (retail `lea edi,[ecx+34h]` then the release); the
//   candidates are the entries from the first one with the name's key while the key holds.
// Name fix (agentNF11 2026-11-11): moved off the local CDefPointer / CAppearanceDef / CAnimationSet / SAnimationSetEntry / vector /
// CThingCreatureBase stand-ins (and a same-TU stand-in GetSubDef body) onto the shared headers. 0x0042B0A2 binds GetSubDef<CAppearanceDef>
// (was ?GetSubDef@CThingBaseDef@@..V?$CDefPointer@VCAppearanceDef@@@@@Z); the row's own symbol is the PDB const one returning a
// reference (was ?GetAppropriateCombatAnimation@CThingCreatureBase@@QAEPBVCAnimationEntry@@ABVCCharString@@@Z, descriptive).
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
#define FABLE_RETAIL_CTHINGCREATUREBASE_COMBATANIM
#define FABLE_RETAIL_CTHINGCREATUREBASE_COMBATANIMSET_SAMETU
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

const CAnimationEntry& CThingCreatureBase::GetRandomCombatAnimationFromName(const CCharString& name) const
{
    const CAnimationSet& animations = GetCombatAnimationSet();
    std::vector<const CAnimationEntry*> candidates;
    CAnimationSet::TAnimMap::const_iterator first = animations.FindFirstAnimWithName(name);
    for (CAnimationSet::TAnimMap::const_iterator it = first; it != animations.Anims.end() && it->first == first->first; ++it)
        candidates.push_back(&it->second);
    return *candidates[GetAppropriateCombatAnimationFromList(candidates)];
}
