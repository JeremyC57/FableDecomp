// CWideScreenMagicPauseEntities::CWideScreenMagicPauseEntities(CGameScriptInterfaceBase& game) -- scope guard for the wide-screen magic cut: keep the game interface and pause every non-scripted entity
// Leaf: ??0CWideScreenMagicPauseEntities@@QAE@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBE09A, 25 bytes, thiscall, `ret 4`.
// Layout: Ego_r.pdb CWideScreenMagicPauseEntities (4 bytes): CGameScriptInterfaceBase* Game +0x00. No vtable.
// Callees: Game's virtual slot 379 (+0x5EC) PauseAllNonScriptedEntities(bool) -- the name the landed
//   CDF_ShineLight::Main 0x00E6EA30 gives the same GameCalls slot; the destructor calls it with false.
// Names (agentNF182, 2026-11-20, q4630): Ego_r.pdb 0x00C9EC04 + FableWin.pdb 0x0359FD38 ??0CWideScreenMagicPauseEntities@@QAE@AAVCGameScriptInterfaceBase@@@Z
//   (by reference; the row was a local model taking a pointer). Every landed referencer (0x00CBF6DC, 0x00F0FB70, 0x00F14A90, 0x00F15660,
//   0x00F162B0, 0x00F170C0, 0x00F17C70, 0x00F19BB0, 0x00F1B9C0) includes engine/CWideScreenMagicPauseEntities.h and binds that spelling.
// Shape: the constructor is inline in the shared header (retail calls it as a COMDAT); a TU-internal scaffolding function uses it so this
//   TU emits the pick-any COMDAT (the 0x00BB1740 / 0x00BCA060 pattern).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
namespace {
// Fidelity: scaffolding, not retail: the smallest use of the header's inline constructor, so this TU emits it (pick-any COMDAT).
// Internal to this TU.
void ReferenceWideScreenMagicPauseEntities(CGameScriptInterfaceBase& game)
{
    CWideScreenMagicPauseEntities pause(game);
}
}
