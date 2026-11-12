// CMainGameComponent::GetCurrentServerFrame() const -- returns CurrentServerFrame (+0x48).
// Leaf: ?GetCurrentServerFrame@CMainGameComponent@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00416225: `mov eax,[ecx+48h] ; ret` (was the placeholder _global::GetMember_dword0x48_00416225).
// Name (agentNF32, 2026-11-12): Ego_r.pdb 0x0041548A + FableWin.pdb 0x018A9243 ?GetCurrentServerFrame@CMainGameComponent@@QBEJXZ;
//   Ego_r places it right before ?GetCurrentGameTime@CMainGameComponent@@IBENXZ (0x00415496), as retail 0x00416225 sits right before
//   CMainGameComponent::GetCurrentGameTime 0x00416231; Ego_r struct_layouts and engine/CMainGameComponent.h put CurrentServerFrame at +0x48.
//   The landed 0x006893D0 binds this address under this symbol; the other binders (CWorld::GetFrame, CMainGameComponent::GetFrame /
//   GetWorldFrame) are stand-in spellings of the same one-instruction COMDAT.
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SERVERFRAME_OUTOFLINE
#include "engine/CMainGameComponent.h"

long CMainGameComponent::GetCurrentServerFrame() const { return CurrentServerFrame; }
