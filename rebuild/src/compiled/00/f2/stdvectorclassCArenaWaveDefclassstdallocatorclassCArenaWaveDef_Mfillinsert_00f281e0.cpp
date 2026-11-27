// 00f281e0 std::vector<CArenaWaveDef>::_M_fill_insert
// Leaf: ?_M_fill_insert@?$vector@VCArenaWaveDef@@V?$allocator@VCArenaWaveDef@@@std@@@std@@IAEXPAVCArenaWaveDef@@IABV3@@Z
// Flags: /Og- /d2vol4
// Inline-budget: the two placement copies at 00f2828c and 00f28337 call the element copy constructor directly.
#define FABLE_RETAIL_ARENA_FILL_UNINITIALIZED_COPY_PASS29
#define FABLE_RETAIL_STLP_FALSE_TAG_POD
#define FABLE_RETAIL_ARENA_INSERT_ADVANCE_PASS29
#ifdef _MSC_VER
#pragma optimize("g",on)
#endif

#define FABLE_RETAIL_ARENA_WAVE_ASSIGNMENT_DECL_PASS29
#define FABLE_RETAIL_CHEROSOULSCREATUREDEF_COPY
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#define FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY
#define FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_DECL_ONLY
#define FABLE_RETAIL_STLP_UNINITIALIZED_FILL_N_DECL_ONLY
#define FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE
#define FABLE_RETAIL_CHEROSOULSCREATUREDEF_OPERATOR_ASSIGN
#define FABLE_RETAIL_CARENAWAVEDEF_IMPLICIT_DTOR

#include "engine/CArenaWaveDef.h"
#include "stlport/fable_containers.h"
inline CArenaWaveDef::CArenaWaveDef(const CArenaWaveDef& x) : CSubDefClassBase(x), NumWaveCreatures(x.NumWaveCreatures), Creatures(x.Creatures), ShortWave(x.ShortWave) {}
namespace std { template void vector<CArenaWaveDef>::_M_fill_insert(CArenaWaveDef*, unsigned int, const CArenaWaveDef&); }