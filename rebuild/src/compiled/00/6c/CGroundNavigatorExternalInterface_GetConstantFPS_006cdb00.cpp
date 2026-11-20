// Global getter: `mov eax,[g]; ret`. Free function, no `this`.
// Leaf: _CGroundNavigatorExternalInterface_GetConstantFPS
// NF166 (round 10, q11679 / q11691 / q11725): 0x01375550 is ?ConstantFPS@CMainGameComponent@@1JA (protected static long,
//   engine/CMainGameComponent.h), read through the inline public accessor GetConstantFPS() (Ego_r + FableWin
//   ?GetConstantFPS@CMainGameComponent@@SIJXZ) as retail code outside the class does; was the row-local stand-in g_value.
#include "engine/CMainGameComponent.h"
extern "C" int CGroundNavigatorExternalInterface_GetConstantFPS() { return CMainGameComponent::GetConstantFPS(); }
// Stand-in (owner rule 2026-11-05): CGroundNavigatorExternalInterface_GetConstantFPS for CGroundNavigatorExternalInterface::GetConstantFPS; RTTI vtable slot ?GetConstantFPS@CGroundNavigatorExternalInterface@@UBEJXZ + PDB name set (egor/fablewin).
