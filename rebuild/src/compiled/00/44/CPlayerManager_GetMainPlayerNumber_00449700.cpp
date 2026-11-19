// CPlayerManager::GetMainPlayerNumber() const -- the main player's number (MainPlayer +0x1C)
// Leaf: ?GetMainPlayerNumber@CPlayerManager@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00449700, 4 bytes: `mov eax,[ecx+1Ch] ; ret`.
// NF104 (q6156, 2026-11-19): moved off its `T::Get()` pad stand-in (file CWorld_DrawGetEnvironment, a propagated label) onto the shared
//   engine/CPlayerManager.h (default path: GetMainPlayerNumber() const declared there, MainPlayer +0x1C asserted). Name: Ego_r.pdb 0x00444EE0
//   + FableWin.pdb 0x01A592C0 ?GetMainPlayerNumber@CPlayerManager@@QBEJXZ; Ego_r lists it first in the retail-ordered GetMainPlayerNumber /
//   GetPlayer / GetMainPlayer run (header note); CTCCoopSpirit::SwapToHero 0x0066FF20 and 43 landed rows bind this symbol.
#include "engine/CPlayerManager.h"

long CPlayerManager::GetMainPlayerNumber() const
{
    return MainPlayer;
}
