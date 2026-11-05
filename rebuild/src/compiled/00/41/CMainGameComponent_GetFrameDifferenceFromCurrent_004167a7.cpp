// CMainGameComponent::GetFrameDifferenceFromCurrent(long frame) const -- whole world frames from `frame` to the current game time
// Leaf: ?GetFrameDifferenceFromCurrent@CMainGameComponent@@IBEJJ@Z
// Flags: /O1
// Retail: Fable.exe @ 0x004167A7, 33 bytes, thiscall `ret 4`; one caller (0x0041843A). Ego_r.pdb / FableWin.pdb protected
// ?GetFrameDifferenceFromCurrent@CMainGameComponent@@IBEJJ@Z.
// Body: GetCurrentGameTime() (0x00416231, out of line) converted to world frames by the inline ConvertGTToWF
// (gameTime / (1.0f / ConstantFPS)); the float result minus `frame` as a float, truncated through __ftol2.
// Shape: the parenthesised float cast of the converted time, `(float)(ConvertGTToWF(...))`, is what gives retail's
// `fdivr st,st(1); fstp st(1)` (the dead game time popped right after the divide, before fisub). Without the
// parentheses, or with a plain float/double local, VC7.1 folds the pop into `fdivp` (31 bytes) or pops after __ftol2.
// The `(float)frame` operand is needed as well (with `- frame` the pop folds into fdivp).
// Opt: /O1 TU (unaligned entry, no padding after the previous function's `ret 4`; `leave` frames and `and [mem],0`
// elsewhere in the TU). `#pragma optimize("s", on)` under the base /O2 gives the same bytes.
// Replaces the byte-baked oracle (asm round 2b parked it after 60+ spellings under /O2).
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GAMETIME
#include "engine/CMainGameComponent.h"

long CMainGameComponent::GetFrameDifferenceFromCurrent(long frame) const
{
    float currentWorldFrame = (float)(ConvertGTToWF(GetCurrentGameTime()));
    return (long)(currentWorldFrame - (float)frame);
}
