// CTCInventoryExperience::BuyAbility(EHeroAbility ability, long cost) -- buys one level of an ability: snapshots the count-down values, finds (or inserts) the ability's undo record, records the experience before and after raising the level and spending the cost, counts the bought level and asks for the screen to be rebuilt
// Leaf: ?BuyAbility@CTCInventoryExperience@@QAEXW4EHeroAbility@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005785B3, 258 bytes, `ret 8`; biased ebp frame (`lea ebp,[esp-70h]`, 0xAC bytes of
//   locals): optimize("s", on) alone, as the landed twin CTCInventoryExperience::BuyStat 0x005786B5.
// Callees: CTCInventoryExperience::SaveExperienceValuesForCountDown 0x00570161 (landed), map<EHeroAbility, SExpSpendUndo>::find
//   0x0057914C (returns the node), SExpSpendUndo() 0x005797A4 / SExpSpendUndo(const SExpSpendUndo&) 0x00579C35 (landed) /
//   ~SExpSpendUndo 0x005797FA (landed), the value_type converting pair ctor 0x00579E1B, the map's insert (insert_unique,
//   pair<iterator,bool> through a hidden pointer) 0x0057A1AA, CTCInventoryExperience::GetTCHeroExperience 0x0056FF3D /
//   GetTCInventoryAbilities 0x0056FFC5 (private, PDB AAE), SExpSpendUndo::SaveBefore 0x0056E93D / SaveAfter 0x0056F597
//   (landed), CTCInventoryAbilities::GetAbilityLevel 0x005EDE93 (const), SetAbilityLevelTo 0x005EE01A (the PDB setter with
//   this signature; the retail body is the plain store `Abilities[ability].Level = level`), GetAbilityStatType 0x005EDEEB
//   (const), CTCHeroExperience::SpendExperience(EHeroExperienceStatCategory, long) 0x006D6590.
// Layout: engine/CTCInventoryExperience.h (retail STLport offsets; CTCHeroExperience / CTCInventoryAbilities from their
//   headers too): UndoableAbilityBuys +0x14C (std::map<EHeroAbility, SExpSpendUndo>), DestroyEverything +0x21C. Map node = 16-byte tree links + key +0x10 +
//   SExpSpendUndo +0x14; retail SExpSpendUndo = four 4-byte STLport lists + Levels +0x10 + eight longs (52 bytes).
// Shape: the twin BuyStat's: a missing record is inserted from a block-scoped default SExpSpendUndo through make_pair and the
//   out-of-line converting value_type ctor, then the find is repeated. Lever (chunk 609): every record access goes through
//   the node (`it->second`), so the node stays live in esi to the end (`inc [esi+24h]`), &it->second is CSE'd into ebx for
//   SaveBefore/SaveAfter, and VC7.1 colours this edi / map ebx / node esi as retail. A `SExpSpendUndo& undo = it->second`
//   reference (the parked form) frees the node early and permutes all three registers.
// NF61 (round 7, q10574): the class itself now (FABLE_RETAIL_CTCINVENTORYEXPERIENCE: std::map<EHeroAbility, SExpSpendUndo>
//   UndoableAbilityBuys +0x14C typed in the header, SExpSpendUndo with its four STLport lists; NF61 opt-in
//   FABLE_RETAIL_CTCINVENTORYEXPERIENCE_BUY declares BuyStat / BuyAbility), engine/CTCHeroExperience.h (NF61 opt-in
//   FABLE_RETAIL_CTCHEROEXPERIENCE_SPENDEXPERIENCE) and engine/CTCInventoryAbilities.h (SETLEVEL / LEVELUP opt-ins) on
//   stlport/fable_map.h; the renamed-layout include and the local CAbilityUndoMap / node / pair / CTCInventoryAbilities models are
//   gone. The insert binds NF58's _Rb_tree::insert_unique 0x0057A1AA. Self leaf unchanged (Ego_r.pdb).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_SPENDEXPERIENCE
#define FABLE_RETAIL_EHEROTRAINABLESTATTYPE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_BUY
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_SETLEVEL
#define FABLE_RETAIL_CTCINVENTORYABILITIES_LEVELUP
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCInventoryExperience.h"
#include "engine/CTCInventoryAbilities.h"

void CTCInventoryExperience::BuyAbility(EHeroAbility ability, long cost)
{
    SaveExperienceValuesForCountDown();
    std::map<EHeroAbility, SExpSpendUndo>& undos = UndoableAbilityBuys;
    std::map<EHeroAbility, SExpSpendUndo>::iterator it = undos.find(ability);
    if (it == undos.end()) {
        SExpSpendUndo undo;
        undos.insert(std::make_pair(ability, undo));
        it = undos.find(ability);
    }
    it->second.SaveBefore(GetTCHeroExperience());
    GetTCInventoryAbilities()->SetAbilityLevelTo(ability, GetTCInventoryAbilities()->GetAbilityLevel(ability) + 1);
    GetTCHeroExperience()->SpendExperience(GetTCInventoryAbilities()->GetAbilityStatType(ability), cost);
    it->second.SaveAfter(GetTCHeroExperience());
    ++it->second.Levels;
    DestroyEverything = true;
}
