// CPlayerGui::ChangeGraphicsBankScene() -- has the player GUI's graphic data bank (Graphics, +0x14) change scene
// Leaf: ?ChangeGraphicsBankScene@CPlayerGui@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00437570, 8 bytes: `mov ecx,[ecx+14h] ; jmp 0x009FD0C0` (CGraphicDataBank::ChangeScene).
// Identity (NF179, PRIORITY q11975): Ego_r.pdb 0x0044A200 + FableWin.pdb 0x01A7CC80 ?ChangeGraphicsBankScene@CPlayerGui@@QAEXXZ
//   (engine/CPlayerGui.h FABLE_RETAIL_CPLAYERGUI_GRAPHICBANK declares it for 0x00437570); its two landed callers 0x00498490
//   (CProgressDisplay::DrawRetailDisplay) and 0x00499D90 (CProgressDisplay::StopProgress) already bind this spelling. Was the
//   gapscan stand-in `T::Run` (?Run@T@@QAEXXZ) on a local `struct T` / CGraphicDataBank model; catalog
//   _global::Forward_ChangeScene_via0x14_00437570. The callee keeps its landed spelling ?ChangeScene@CGraphicDataBank@@QAEXXZ
//   (Ego_r.pdb / FableWin.pdb; engine/CGraphicDataBank.h).
// Fidelity: cast, header debt: Ego_r.pdb types CPlayerGui::Graphics (+0x14) as `CGraphicDataBank*` (struct_layouts_egor.tsv), but
//   engine/CPlayerGui.h (FABLE_RETAIL_CPLAYERGUI_CTOR) declares it `const CGraphicDataBank*`; ChangeScene is non-const, so the call
//   goes through const_cast (no code). Retyping the header member is a coordinated header item (NOTES).
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_CTOR
#define FABLE_RETAIL_CPLAYERGUI_GRAPHICBANK
#include "engine/CGraphicDataBank.h"
#include "engine/CPlayerGui.h"

void CPlayerGui::ChangeGraphicsBankScene()
{
    const_cast<CGraphicDataBank*>(Graphics)->ChangeScene();
}
