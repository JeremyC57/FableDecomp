// CMainGameComponent::GetCurrentWorldFrame() const -- the world's frame counter: tail call of the static CWorld::GetFrame()
// Leaf: ?GetCurrentWorldFrame@CMainGameComponent@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004167C8, 5 bytes: `jmp 0x0049D870` (CWorld::GetFrame, static __fastcall, no argument).
// NF104 (q4991, 2026-11-19): moved off its `extern "C" Forward()` stand-in (catalog GFGetPreMainMemoryUsed, propagated) onto the shared
//   engine/CMainGameComponent.h (FABLE_RETAIL_CMAINGAMECOMPONENT + _WORLDFRAME declare it, as the landed caller 0x008AF500) and
//   engine/CWorld.h (?GetFrame@CWorld@@SIJXZ 0x0049D870, bound). Name: Ego_r.pdb 0x00415A9F ?GetCurrentWorldFrame@CMainGameComponent@@QBEJXZ;
//   the 18 landed referencing rows bind this symbol.
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLDFRAME
#define FABLE_RETAIL_CWORLD
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"

long CMainGameComponent::GetCurrentWorldFrame() const
{
    return CWorld::GetFrame();
}
