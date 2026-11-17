// std::pair<EHeroAbility const, SExpSpendUndo>::pair(const pair<EHeroAbility, SExpSpendUndo>&) -- STLport 4.6 _pair.h converting constructor
//   (the make_pair -> value_type conversion of CTCInventoryExperience::BuyAbility)
// Leaf: ??$?0W4EHeroAbility@@USExpSpendUndo@@@?$pair@$$CBW4EHeroAbility@@USExpSpendUndo@@@std@@QAE@ABU?$pair@W4EHeroAbility@@USExpSpendUndo@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00579E1B, 29 bytes (speed TU): the first member copied inline, then SExpSpendUndo(const SExpSpendUndo&) 0x00579C35 (the implicit copy, out of line); caller 0x005785B3.
// NF64 (round 7, q10685): an explicit instantiation of the stlport/fable_containers.h pair constructor (STLport 4.6 _pair.h); the landed
//   caller binds this spelling (see the entry). Was the generated stand-in `SPairLongCSecond_00579c35::SPairLongCSecond_00579c35`. Descriptive-by-library (retail is STLport 4.6; both
//   PDBs are Dinkumware builds).
#define FABLE_RETAIL_SEXPSPENDUNDO_ONLY
#include "stlport/fable_containers.h"
#include "engine/EHeroAbility.h"
#include "engine/CTCInventoryExperience.h"
template std::pair<EHeroAbility const, SExpSpendUndo>::pair(const std::pair<EHeroAbility, SExpSpendUndo>&);
