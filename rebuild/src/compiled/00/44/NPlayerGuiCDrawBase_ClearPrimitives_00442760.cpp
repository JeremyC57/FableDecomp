// NPlayerGui::CDrawBase::ClearPrimitives() -- does nothing (the base draw element keeps no primitives of its own to clear)
// Leaf: ?ClearPrimitives@CDrawBase@NPlayerGui@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00442760, thiscall, a bare `ret`.
// Name (NF432 from NF430's NAME_FIXES; two sources): retail slot 1 of every NPlayerGui draw vftable that does not override it (0x0125A538 ..
//   [0x00644627, 0x00442760]; RTTI COL present), and FableWin.pdb's vftables of CDrawBase / CDrawSpiritsHealth / CDrawDamage put
//   ?ClearPrimitivesIfNecessary@CDrawBase@NPlayerGui@@UAEXXZ at slot 0 and ?ClearPrimitives@CDrawBase@NPlayerGui@@UAEXXZ at slot 1; CDrawMiniMap
//   overrides slot 1 with ClearPrimitives (landed 0x00644660). FableWin's CDrawBase::ClearPrimitives body is empty (/Od prologue + ret),
//   ClearPrimitivesIfNecessary has the Drawing test and lives at 0x00644627 (already bound to that name).
// Was: ?ClearPrimitivesIfNecessary@CDrawBase@NPlayerGui@@UAEXXZ (an Xbox 360 Test PDB slot order; the PC PDB of the same code disagrees).
#include "engine/NPlayerGui_CDrawBase.h"
namespace NPlayerGui {
void CDrawBase::ClearPrimitives()
{
}
}
