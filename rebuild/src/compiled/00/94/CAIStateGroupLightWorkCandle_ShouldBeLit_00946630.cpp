// Leaf: @CAIStateGroup_LightWorkCandle_ShouldBeLit@0
#include "rebuild_abi.h"
// NF122 (2026-11-20, q4490): the callee 0x006BB2C0 is Ego_r.pdb / FableWin.pdb ?IsDay@CGameTimeManager@@QBE_NXZ on the +0x1C member
//   (CWorld::PGameTimeManager), declared by engine/CGameTimeManager.h (FABLE_RETAIL_CGAMETIMEMANAGER + _ISDAY); was the stand-in
//   M::Check_006bb2c0() reached through a cast of a void* member.
#define FABLE_RETAIL_CGAMETIMEMANAGER
#define FABLE_RETAIL_CGAMETIMEMANAGER_ISDAY
#include "engine/CGameTimeManager.h"
struct S { char p[0x1c]; CGameTimeManager* f1c; };
extern "C" S* FABLE_FASTCALL GetS_946630();
extern "C" bool FABLE_FASTCALL CAIStateGroup_LightWorkCandle_ShouldBeLit(){ return !GetS_946630()->f1c->IsDay(); }
// Stand-in (owner rule 2026-11-05): CAIStateGroup_LightWorkCandle_ShouldBeLit for CAIStateGroup_LightWorkCandle::ShouldBeLit; RTTI vtable slot ?ShouldBeLit@CAIStateGroup_LightWorkCandle@@MBE_NXZ + PDB name set (egor/fablewin).
