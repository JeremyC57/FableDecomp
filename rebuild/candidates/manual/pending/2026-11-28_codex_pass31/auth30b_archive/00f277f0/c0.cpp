// Flags: /Og- /d2vol5
// Inline-budget: retail expands both backward-copy wrappers, the temporary element copy, and both placement loops. Each loop calls the element copy constructor.
#define FABLE_RETAIL_PASS22_SCOREBOARD_ORIGINAL_OVERFLOW_SCOPE
#ifdef _MSC_VER
#pragma optimize("g",on)
#endif
#define FABLE_RETAIL_STLP_FALSE_TAG_POD
#define FABLE_RETAIL_WAVE_DEFAULT_DTOR_PASS29
#define FABLE_RETAIL_DEFS_FILL_INSERT_PASS29
#define FABLE_RETAIL_CHEROSOULSCREATUREDEF_COPY
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
// Leaf: ?_M_fill_insert@?$vector@VCHeroSoulsCreatureDef@@V?$allocator@VCHeroSoulsCreatureDef@@@std@@@std@@IAEXPAVCHeroSoulsCreatureDef@@IABV3@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_UNINITIALIZED_FILL_N_DECL_ONLY
#define FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
#define FABLE_RETAIL_CHEROSOULSCREATUREDEF_OPERATOR_ASSIGN

#include "engine/CHeroSoulsCreatureDef.h"
#include "stlport/fable_containers.h"
inline CHeroSoulsCreatureDef::CHeroSoulsCreatureDef(const CHeroSoulsCreatureDef& x) : CSubDefClassBase(x), CreatureType(x.CreatureType), Location(x.Location), NumCreatures(x.NumCreatures) {}

namespace std { template void vector<CHeroSoulsCreatureDef>::_M_fill_insert(CHeroSoulsCreatureDef*, unsigned int, const CHeroSoulsCreatureDef&); }
