// std::pair<EHeroTrainableStatType const, SExpSpendUndo>::pair(const pair&) -- STLport 4.6 _pair.h copy constructor: the key copied
//   inline, then SExpSpendUndo's copy constructor 0x00579C35 out of line; returns `this`
// Leaf: ??0?$pair@$$CBW4EHeroTrainableStatType@@USExpSpendUndo@@@std@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00579CDE, 29 bytes (speed TU), `ret 4`.
// NF64 (round 7, q10683): an explicit instantiation of the stlport/fable_containers.h pair copy constructor for the map<EHeroTrainableStatType,
//   SExpSpendUndo> value type (Ego_r.pdb SExpSpendUndo through engine/CTCInventoryExperience.h FABLE_RETAIL_SEXPSPENDUNDO_ONLY); the landed
//   _M_create_node 0x00579F3E (NF61) binds this spelling. The member copy now binds the Ego_r.pdb implicit ??0SExpSpendUndo@@QAE@ABU0@@Z
//   0x00579C35. Was the generated stand-in `SPairLongCSecond_00579c35::SPairLongCSecond_00579c35` (a CSecond_00579c35 member stand-in).
//   Descriptive-by-library (retail is STLport 4.6; both PDBs are Dinkumware builds).
#define FABLE_RETAIL_SEXPSPENDUNDO_ONLY
#include "stlport/fable_containers.h"
#include "engine/EHeroTrainableStatType.h"
#include "engine/CTCInventoryExperience.h"
template std::pair<EHeroTrainableStatType const, SExpSpendUndo>::pair(const std::pair<EHeroTrainableStatType const, SExpSpendUndo>&);
