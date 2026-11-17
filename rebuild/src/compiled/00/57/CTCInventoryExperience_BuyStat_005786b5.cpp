// CTCInventoryExperience::BuyStat(EHeroTrainableStatType stat, long amount) -- buys one level of a trainable stat for `amount` experience: snapshots the experience values for the count-down, finds (or inserts) the stat's undo record, records the hero's experience before and after raising the stat level and spending, counts the bought level and asks for the screen to be rebuilt
// Leaf: ?BuyStat@CTCInventoryExperience@@QAEXW4EHeroTrainableStatType@@J@Z
// Lever: `#pragma optimize("s", on)` without `("y", off)` gives retail's biased frame (`lea ebp,[esp-70h]` ... `add ebp,70h ; leave`); adding y-off gives the plain `mov ebp,esp` frame (GOTCHAS chunk 302/471/521).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005786B5, length 226 bytes (`ret 8`). Size TU: optimize("s",on) alone (default FPO setting);
// the frame is wider than 0x80 bytes so VC biases ebp (`lea ebp,[esp-70h]` ... `add ebp,70h ; leave`) to keep every local in a byte displacement.
// Callees: CTCInventoryExperience::SaveExperienceValuesForCountDown() @ 0x00570161, map<EHeroTrainableStatType,SExpSpendUndo>::find
// @ 0x00579114 (catalog "BinaryTree_FindLowerBound"), SExpSpendUndo::SExpSpendUndo() @ 0x005797A4 (catalog "CHeroExperienceDef::
// CHeroExperienceDef", propagated), SExpSpendUndo(const SExpSpendUndo&) @ 0x00579C35, ~SExpSpendUndo @ 0x005797FA (catalog "_Dest_val<CMacro>",
// propagated), the value_type converting pair ctor @ 0x00579E4E (catalog "pair<unsigned long,CAnimationEntry>", propagated), the map's
// insert (insert_unique, pair<iterator,bool> through a hidden pointer) @ 0x0057A124 (catalog "BuyStat_FindOrInsert"), CTCInventoryExperience::
// GetTCHeroExperience() @ 0x0056FF3D, SExpSpendUndo::SaveBefore(CTCHeroExperience*) @ 0x0056E93D and SaveAfter @ 0x0056F597, CTCHeroExperience::
// ModifyTrainableStatLevel(EHeroTrainableStatType, long) @ 0x006D6810 and SpendExperience(EHeroTrainableStatType, long) @ 0x006D6F60.
// Layout: retail CTCInventoryExperience UndoableStatBuys at +0x140 (PDB +0x15C; header pointer first), DestroyEverything +0x21C (PDB +0x270);
// retail SExpSpendUndo is 52 bytes (four 4-byte list heads, then Levels +0x10 and the eight experience values); the map node holds the
// key at +0x10 and the record at +0x14.
// Shape: the record is a block-scoped default-constructed local ([ebp-4], destroyed after the re-find at the end of its block); make_pair
// builds pair<EHeroTrainableStatType,SExpSpendUndo> straight into its result ([ebp+30h]: key copy then record copy) and the value_type
// conversion copies it again ([ebp-3Ch]) for insert, whose pair<iterator,bool> result ([ebp+68h]) is discarded; the two pair temporaries
// die in reverse order before the re-find. The map lives in ebx as a reference, the record reference in ebx after it, the node in esi.
// NF61 (round 7, q10573): moved onto the shared headers engine/CTCInventoryExperience.h (FABLE_RETAIL_CTCINVENTORYEXPERIENCE: the
//   std::map<EHeroTrainableStatType, SExpSpendUndo> UndoableStatBuys +0x140, SExpSpendUndo with its four STLport lists, DestroyEverything
//   +0x21C; NF61 opt-in FABLE_RETAIL_CTCINVENTORYEXPERIENCE_BUY declares BuyStat / BuyAbility) and engine/CTCHeroExperience.h (NF61 opt-in
//   FABLE_RETAIL_CTCHEROEXPERIENCE_SPENDEXPERIENCE) with stlport/fable_map.h; the local CUndoMap / SUndoNode / SExpSpendUndo / pair
//   models are gone. The insert binds NF58's _Rb_tree::insert_unique 0x0057A124, the find the tree's _M_find 0x00579114 (the
//   spelling the landed 0x005729D8 binds), the SExpSpendUndo special members and the converting pair ctor their implicit / STLport
//   spellings. Self leaf unchanged (Ego_r.pdb).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_MODIFYLEVEL
#define FABLE_RETAIL_CTCHEROEXPERIENCE_SPENDEXPERIENCE
#define FABLE_RETAIL_EHEROTRAINABLESTATTYPE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_BUY
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CTCInventoryExperience.h"

void CTCInventoryExperience::BuyStat(EHeroTrainableStatType stat, long amount)
{
    SaveExperienceValuesForCountDown();
    std::map<EHeroTrainableStatType, SExpSpendUndo>& undos = UndoableStatBuys;
    std::map<EHeroTrainableStatType, SExpSpendUndo>::iterator it = undos.find(stat);
    if (it == undos.end()) {
        SExpSpendUndo undo;
        undos.insert(std::make_pair(stat, undo));
        it = undos.find(stat);
    }
    SExpSpendUndo& undo = it->second;
    undo.SaveBefore(GetTCHeroExperience());
    GetTCHeroExperience()->ModifyTrainableStatLevel(stat, 1);
    GetTCHeroExperience()->SpendExperience(stat, amount);
    undo.SaveAfter(GetTCHeroExperience());
    ++it->second.Levels;
    DestroyEverything = true;
}
