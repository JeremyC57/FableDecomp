// CHeroAbilityStatPackage::AddStatThreshold(EHeroTrainableStatType stat, long threshold) -- insert (stat, threshold) into StatThresholds (map::insert -> _Rb_tree::insert_unique 0x00463E2E; the pair<iterator,bool> result discarded)
// Leaf: ?AddStatThreshold@CHeroAbilityStatPackage@@QAEXW4EHeroTrainableStatType@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00851CC0, length 38 bytes (thiscall, `ret 8`).
// Shape: the 8-byte value_type is a stack local at [esp] (the two arguments copied through eax/edx), the pair<iterator,bool>
//   result a second frame local at [esp+8]; `add esp,10h` releases the frame because insert_unique pops its own two arguments.
// Identity (NF61, round 7, q10561): Ego_r.pdb / FableWin.pdb ?AddStatThreshold@CHeroAbilityStatPackage@@QAEXW4EHeroTrainableStatType@@J@Z
//   (the only PDB member taking (EHeroTrainableStatType, long) whose class holds this map); Ego_r.pdb CHeroAbilityStatPackage has one
//   member, StatThresholds map<EHeroTrainableStatType, long> at +0 (`this` passed straight to the tree), and the body inserts into
//   exactly that instance (0x00463E2E, NF58's map<EHeroTrainableStatType, long> insert_unique); its TU sibling 0x00851B50 is the landed
//   CHeroAbilityStatPackage::IsValid. Unreferenced in Fable.exe (no call or data reference). Was the stand-in
//   CRetailMap_00851cc0::Add(long, long) with a local tree model (the chunk label "CTCLook::IsTurningBody" was propagated).
#include "engine/CHeroAbilityStatPackage.h"

void CHeroAbilityStatPackage::AddStatThreshold(EHeroTrainableStatType stat, long threshold)
{
    StatThresholds.insert(std::map<EHeroTrainableStatType, long>::value_type(stat, threshold));
}
