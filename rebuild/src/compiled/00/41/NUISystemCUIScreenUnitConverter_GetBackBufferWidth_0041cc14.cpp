// NUISystem::CUIScreenUnitConverter::GetBackBufferWidth() -- the UI back-buffer width in UI units: the current back buffer's when the converter uses relative coordinates, else the default
// Leaf: ?GetBackBufferWidth@CUIScreenUnitConverter@NUISystem@@SIMXZ
// Flags: /Os
// Retail: Fable.exe (TLC, VC7.1) @ 0x0041CC14, 23 bytes: `cmp byte ptr [13B8768h],0 ; je ; fld [013B876Ch] ; ret ; fld [01375CD4h] ; ret`.
// Identity: Ego_r.pdb / FableWin.pdb ?GetBackBufferWidth@CUIScreenUnitConverter@NUISystem@@SIMXZ (static __fastcall float, no arguments); the
//   out-of-line COMDAT copy of the inline header getter that the landed CText::Draw 0x0054EF00 expands in place; every landed caller
//   (0x0041CF47 and the CTCInGameMenu / CManager rows) already binds this spelling.
// Data: IsUsingRelativeCoords 0x013B8768, BackBufferWidth 0x013B876C, DefaultBackBufferWidth 0x01375CD4: the Ego_d.pdb PUB32 spellings
//   (?IsUsingRelativeCoords@CUIScreenUnitConverter@NUISystem@@0_NA, ?BackBufferWidth@...@@0MA, ?DefaultBackBufferWidth@...@@0MA: private
//   static) through engine/NUISystem_CUIScreenUnitConverter.h FABLE_RETAIL_CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS (as the landed
//   0x0052E470 / 0x0052E4D0 / 0x0054EF00 use them).
// Size-optimised (`cmp byte ptr [..],0`, not `mov al ; test al`): the old row had `#pragma optimize("s",on)`, here the /Os flag.
// NF112 (2026-11-20, q4769): re-landed as the header getter; the row was a free __fastcall stand-in
//   CTCLook__GetHeadAngularVelocityMaxXY over invented g_HeadAngVelMaxXY_* globals (a propagated label).
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_ORIGINAL_DIMENSIONS
#include "engine/NUISystem_CUIScreenUnitConverter.h"

namespace {
// Fidelity: scaffolding, not retail: takes the address of the header's inline static getter so this object emits it out of line as a
// pick-any COMDAT (retail's 0x0041CC14). Internal to this TU; an ordinary (not inline) function that VC7.1 compiles though nothing calls it.
float (__fastcall* GetBackBufferWidthReference())();
float (__fastcall* GetBackBufferWidthReference())()
{
    return &NUISystem::CUIScreenUnitConverter::GetBackBufferWidth;
}
}
