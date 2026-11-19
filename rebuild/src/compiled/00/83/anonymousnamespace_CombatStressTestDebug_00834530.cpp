// (anonymous namespace)::CombatStressTestDebug(CThingAICreature& ai_creature, const CThingSearchTools& search_tools, unsigned long* pworld_seed) -- console combat stress test: suspend the creature's AI, drop it from the combat manager when it is in melee as an attacker, then register it to fight a random sample (up to half the count draws, duplicates merged) of all scan-visible AI creatures in the world
// Leaf: ?CombatStressTestDebug@?A0x
// Retail: Fable.exe (TLC, VC7.1) @ 0x00834530, 558 bytes, __fastcall (ai_creature in ecx, search_tools in edx), `ret 4`.
// Identity: Ego_r.pdb / FableWin.pdb ?CombatStressTestDebug@?A0x4ed42fe4@@YIXAAVCThingAICreature@@ABVCThingSearchTools@@PAK@Z (anonymous
//   namespace of the thing_ai_creature TU; FableWin debug 0x01D6D8F0); its only caller is the landed CThingAICreature::WorldUpdate
//   0x00834760 behind NGlobalConsole::GCombatStressTestDebug. Locals: Ego_d.pdb's record (debug 461): ai_creature, search_tools,
//   pworld_seed, ais; { random_indices, rnd_num; { c1 } { it; { ptarget } } }.
// Callees: _Rb_tree<void*,..>::insert_unique 0x00835560 (the inline SuspendAI(true, 0)), GetAllThings<CThingAICreature,
//   vector<CThingAICreature*>, CThingFilter_None> 0x0052B270 (this TU's visible noinline copy grades RELOCATION_MATCH at that address:
//   overlay FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY_EGOD), CThingCreatureBase::IsInMeleeAsAttacker 0x00662410, GetCombatManager
//   0x00661ED0, CCombatManager::DeregisterCombatant 0x006ED9C0 / RegisterCombatant 0x006EDC50, GFROR13 0x00497890 (inline GFRandom),
//   _Rb_tree<long,..>::insert_unique 0x00512DA0 and _M_erase 0x004A9CB0 (also RELOCATION_MATCH at its address from this TU), malloc /
//   free (STLport node and vector storage).
// Shape: FableWin's statements: the filter is the CThingFilter_None() temporary; ais.size() is read inside the empty test (VC7.1 keeps it
//   for the draws); the visible GetAllThings body is the lever that lets VC7.1 keep the vector and the filter temporary uncaptured
//   (with the declaration only, 539 v 558 and a 0x28 frame). SuspendAI is inline (overlay FABLE_RETAIL_CTHINGAICREATURE_SUSPENDERS);
//   its dead erase arm needs the tree's erase(key) (STLP_TREE_ERASE + _ERASE_KEY; _POSTINC so erase(first, last) has STLport's
//   erase(first++), not the prefix fallback VC7.1 warns about, C4620).
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_SUSPENDERS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY_EGOD
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_SET_EMPTY
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#define FABLE_RETAIL_STLP_TREE_POSTINC
#include "engine/CThingAICreature.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_None.h"
#include "engine/CCombatManager.h"
#include "engine/GFRandom.h"
#include "stlport/fable_map.h"

namespace {

void __fastcall CombatStressTestDebug(CThingAICreature& ai_creature, const CThingSearchTools& search_tools, unsigned long* pworld_seed)
{
    ai_creature.SuspendAI(true, 0);
    std::vector<CThingAICreature*> ais;
    search_tools.GetAllThings<CThingAICreature>(CThingFilter_None(), ais);
    if (ai_creature.IsInMeleeAsAttacker())
        ai_creature.GetCombatManager().DeregisterCombatant(ai_creature);
    if (!ais.empty()) {
        long rnd_num = GFRandom(ais.size() / 2, pworld_seed);
        std::set<long> random_indices;
        for (long c1 = 0; c1 != rnd_num; ++c1)
            random_indices.insert(GFRandom(ais.size(), pworld_seed));
        for (std::set<long>::iterator it = random_indices.begin(); it != random_indices.end(); ++it) {
            CThingAICreature* ptarget = ais[*it];
            if (ptarget != &ai_creature)
                ai_creature.GetCombatManager().RegisterCombatant(ai_creature, *ptarget, true);
        }
    }
}

}

// Fidelity: the anonymous-namespace function's only caller is WorldUpdate (another row); the pointer keeps this TU's copy emitted.
void (__fastcall* g_pCombatStressTestDebug)(CThingAICreature&, const CThingSearchTools&, unsigned long*) = &CombatStressTestDebug;