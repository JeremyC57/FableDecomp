// Leaf: @CCreatureActionHeroJump_IsPreventingNormalMovement@4
#include "rebuild_abi.h"
struct C { char p10[0x10]; int f10; char p14[0x1c-0x14]; int f1c; };
extern "C" char FABLE_FASTCALL CCreatureActionHeroJump_IsPreventingNormalMovement(C *action){ return (char)((action->f1c + 1) < action->f10); }
// Stand-in (owner rule 2026-11-05): CCreatureActionHeroJump_IsPreventingNormalMovement for CCreatureActionHeroJump::IsPreventingNormalMovement; RTTI vtable slot ?IsPreventingNormalMovement@CCreatureActionHeroJump@@UBE_NXZ + PDB name set (egor/fablewin).
