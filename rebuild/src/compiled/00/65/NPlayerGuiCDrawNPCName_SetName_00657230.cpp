// NPlayerGui::CDrawNPCName::SetName(const CWideString& name) -- set the NPC name the player GUI shows over the targeted creature
// Leaf: ?SetName@CDrawNPCName@NPlayerGui@@QAEXABVCWideString@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00657230, 8 bytes: `add ecx,28h ; jmp 0x0099B7D0` (Name = name, tail call).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?SetName@CDrawNPCName@NPlayerGui@@QAEXABVCWideString@@@Z, first of the Ego_r donor run
//   SetName 0x00699420 / IsShowingScreenshotMessage 0x00699430 / ShowScreenshotMessage 0x00699480 / Draw 0x006994A0, as retail 0x00657230
//   precedes the landed IsShowingScreenshotMessage 0x00657240 and Draw 0x006572A0; it assigns the CWideString Name (+0x28); the sole
//   caller is CPlayerGui's forwarder 0x004379D0 (`mov ecx,[ecx+248h]`: the CDrawNPCName counted pointer; FableWin
//   ?SetNPCNameDisplay@CPlayerGui@@QAEXABVCWideString@@@Z), itself called by CTCTargetingPlayer. The catalog's "_Cons_val<..>" was propagated.
// Callees: ??4CWideString@@QAEABV0@ABV0@@Z 0x0099B7D0.
// Header: engine/NPlayerGui_CDrawNPCName.h (new) over engine/NPlayerGui_CDrawBase.h (new).
#include "engine/NPlayerGui_CDrawNPCName.h"
namespace NPlayerGui {
void CDrawNPCName::SetName(const CWideString& name)
{
    Name = name;
}
}