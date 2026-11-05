// Leaf: @CAIStateGroup_LightWorkCandle_ShouldBeLit@0
#include "rebuild_abi.h"
struct M { bool Check_006bb2c0(); };
struct S { char p[0x1c]; void* f1c; };
extern "C" S* FABLE_FASTCALL GetS_946630();
extern "C" bool FABLE_FASTCALL CAIStateGroup_LightWorkCandle_ShouldBeLit(){ return !((M*)(GetS_946630()->f1c))->Check_006bb2c0(); }
// Stand-in (owner rule 2026-11-05): CAIStateGroup_LightWorkCandle_ShouldBeLit for CAIStateGroup_LightWorkCandle::ShouldBeLit; RTTI vtable slot ?ShouldBeLit@CAIStateGroup_LightWorkCandle@@MBE_NXZ + PDB name set (egor/fablewin).
