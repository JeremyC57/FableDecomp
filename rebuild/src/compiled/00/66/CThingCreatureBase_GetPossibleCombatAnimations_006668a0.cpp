// CThingCreatureBase::GetPossibleCombatAnimations(const CCharString& name, std::vector<const CAnimationEntry*>& anim_list) const -- collect the combat-animation-set entries named `name` into the current mode's filter / weapon filter / no-group buckets, copy the first non-empty bucket into anim_list, else fall back to the "DEFAULT" animation and return false
// Leaf: ?GetPossibleCombatAnimations@CThingCreatureBase@@QBE_NABVCCharString@@AAV?$vector@PBVCAnimationEntry@@V?$allocator@PBVCAnimationEntry@@@std@@@std@@@Z
// Grade: RELOCATION_MATCH (688 bytes) -- the same-TU helpers it expands / calls grade RELOCATION_MATCH at their own addresses from this
//   source: GetCombatAnimationSet 0x00663260 (COMBATANIMSET_SAMETU body) and vector<const CAnimationEntry*>::operator= 0x00669BB0
//   (STLP_VECTOR_C body).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006668A0, 688 bytes, thiscall `ret 8`, the bool in al.
// Identity: Ego_r.pdb release\thing_creature_base.obj #136 (0x00540A20, 763 bytes) and FableWin.pdb debug\thing_creature_base.obj #227
//   (0x01D9B740, 1386 bytes) ?GetPossibleCombatAnimations@CThingCreatureBase@@QBE_N..; Ego_d locals found_anims, anim_set, anims,
//   panim_filter_best, panim_filter_weapon, no_group_filter, ptc_cmm, best_list, weapon_based_list, no_group_list, start_it, it,
//   anim_entry, entry (Ego_d also has an unused combat_anims vector: retail frees three vectors, so this revision has three).
// Callees (PDB spellings): CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2 (inside the auto-inlined GetCombatAnimationSet),
//   CThing::PeekTC<const CTCCreatureModeManager> (inline: HasTC bit 49, CVectorMap::LowerBound 0x004365B0),
//   CTCCreatureModeManager::GetCurrentMode 0x0073A6E0 (twice), CModeBase::GetAnimFilter (slot 4), GetDefaultAnimFilter (inline, mode+8),
//   CAnimationSet::FindFirstAnimWithName 0x005DC020, the filters' operator() (slot 0), vector::push_back 0x005E1EB0 (the third
//   bucket, out of line) / _M_insert_overflow 0x005E1420, vector::operator= 0x00669BB0, CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, CThingCreatureBase::GetAnimFromName(const CCharString&, long) 0x00662A00, free 0x00BFEA14.
// Shape: FableWin's statement order and names. The retail revision has no OnMissingAnimation / debug error message before the
//   fallback (FableWin has both) and no combat_anims vector. The fallback name is an implicit conversion of "DEFAULT" (retail builds
//   the CCharString temporary first, then pushes the index; the explicit CCharString("DEFAULT") argument pushes 0 first: 669v673).
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
#define FABLE_RETAIL_CTHINGCREATUREBASE_POSSIBLECOMBATANIMS
#define FABLE_RETAIL_CTHINGCREATUREBASE_COMBATANIMSET_SAMETU
#define FABLE_RETAIL_CANIMATIONSET_ANIMEXISTS
#define FABLE_RETAIL_CANIMATIONSET_QUERIES
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CANIMATIONSET_ANIMS
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_DEFAULTANIMFILTER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CAPPEARANCEDEF_SIZE_BODY   // NF357: GetSubDef<CAppearanceDef> pick-any copy = size-mode owner 0x0042B0A2 (scaffolding, not retail)
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_STLP_VECTOR_C
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAppearanceDef.h"
#include "engine/CAnimFilterNoGroup.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CDefinitionManager.h"
#include "engine/CThingCreatureBase.h"

bool CThingCreatureBase::GetPossibleCombatAnimations(const CCharString& name, std::vector<const CAnimationEntry*>& anim_list) const
{
    bool found_anims = true;
    const CAnimationSet& anim_set = GetCombatAnimationSet();
    const CAnimationSet::TAnimMap& anims = anim_set.Anims;
    const CAnimFilterBase* panim_filter_best = 0;
    const CAnimFilterBase* panim_filter_weapon = 0;
    CAnimFilterNoGroup no_group_filter;
    const CTCCreatureModeManager* ptc_cmm;
    if (PeekTC(&ptc_cmm)) {
        panim_filter_best = &ptc_cmm->GetCurrentMode().GetAnimFilter();
        panim_filter_weapon = &ptc_cmm->GetCurrentMode().GetDefaultAnimFilter();
    }
    std::vector<const CAnimationEntry*> best_list;
    std::vector<const CAnimationEntry*> weapon_based_list;
    std::vector<const CAnimationEntry*> no_group_list;
    CAnimationSet::TAnimMap::const_iterator start_it = anim_set.FindFirstAnimWithName(name);
    for (CAnimationSet::TAnimMap::const_iterator it = start_it; it != anims.end() && it->first == start_it->first; ++it) {
        const CAnimationEntry& anim_entry = it->second;
        if (!panim_filter_best || (*panim_filter_best)(anim_entry))
            best_list.push_back(&anim_entry);
        else if (!panim_filter_weapon || (*panim_filter_weapon)(anim_entry))
            weapon_based_list.push_back(&anim_entry);
        else if (no_group_filter(anim_entry))
            no_group_list.push_back(&anim_entry);
    }
    if (!best_list.empty())
        anim_list = best_list;
    else if (!weapon_based_list.empty())
        anim_list = weapon_based_list;
    else if (!no_group_list.empty())
        anim_list = no_group_list;
    if (anim_list.empty()) {
        found_anims = false;
        const CAnimationEntry& entry = GetAnimFromName("DEFAULT", 0);
        anim_list.push_back(&entry);
    }
    return found_anims;
}