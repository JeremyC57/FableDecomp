// CTCCombat::GetCombatAbilityData(iterator& ittybitty, ECombatAbilityInterfaceType interface_type) -- walk the combat-ability multimap with the caller's iterator and stop at the first ability whose data has the wanted interface type: true with the iterator on it, else false with it at end()
// Leaf: ?GetCombatAbilityData@CTCCombat
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00733620, 106 bytes (112 with padding), `ret 8`.
// Label correction: catalog "CTCCombat_FindBlockAbility" -- Ego_r.pdb has the non-const
//   ?GetCombatAbilityData@CTCCombat@@AAE_NAAV?$_Tree_iterator...@W4ECombatAbilityInterfaceType@@@Z (bool, iterator&,
//   enum); callers GetBlockAbilityIterator (landed 0x00733750, calls it "FindBlockAbility") and 0x00733D05.
// Re-port (p25N, 2026-11-20): onto the shared engine/CTCCombat.h (FABLE_RETAIL_CTCCOMBAT + the add-only opt-in
//   FABLE_RETAIL_CTCCOMBAT_GETHITCOUNTERATTACK, which declares the member private with the enum argument, as both PDBs spell it), for the
//   same-TU copy in 0x00733CF0. Symbol fix: the old local model defined ?GetCombatAbilityData@CTCCombat@@QAE_NAAU_Rb_tree_iterator@@J@Z
//   (public, long); now ?GetCombatAbilityData@CTCCombat@@AAE_NAAU?$_Rb_tree_iterator@..@std@@W4ECombatAbilityInterfaceType@@@Z (STLport
//   iterator in retail where the PDBs have Dinkumware's _Tree_iterator). Ego_d.pdb parameters ittybitty, interface_type.
// Callees: none (STLport tree iterator ++ inlined: stlport/fable_map.h `_M_node = _Rb_global_inst::_M_increment(_M_node)`, static
//   node in / node out, so the walk runs in eax and stores through the reference once per step).
// Layout: retail CTCCombat::CombatAbilities multimap at +0x70 (STLport, header node pointer first); node value +0x10:
//   key, CCountedPointer<CCombatAbilityData> +0x14 (8 bytes), CCombatAbility +0x1c; CCombatAbilityData::InterfaceType
//   +0x14 (Ego_r.pdb; read through the shared header's GetInterfaceType()).
// Shape: the iterator is the caller's reference, so begin() and every increment are stored through it and the node
//   re-read from it at the top of each pass; end() is reloaded each time.
// Define block: the tc_combat.cpp rows' common block (0x00733CF0 UpdateGetHitCounterAttack and its TU-mates), so that every same-TU copy
//   compiles from the same text.
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_GETHITCOUNTERATTACK
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITY_MEMBERS
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CTCENTITYEVENTS
#define FABLE_RETAIL_STLP_MULTIMAP_BEGIN_END
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_STLP_NOT1
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_NENTITYEVENTS_CISHITEVENTHITBY
#include "engine/ECombatAbilityInterfaceType.h"
#include "engine/CTCCombat.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatAbilityData.h"
#include "engine/CTCEntityEvents.h"
#include "engine/NEntityEvents_EventFilters.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/GFRandom.h"

bool CTCCombat::GetCombatAbilityData(TCombatAbilityList::iterator& ittybitty, ECombatAbilityInterfaceType interface_type)
{
    for (ittybitty = CombatAbilities.begin(); ittybitty != CombatAbilities.end(); ++ittybitty) {
        if (ittybitty->second.first->GetInterfaceType() == interface_type)
            return true;
    }
    return false;
}
