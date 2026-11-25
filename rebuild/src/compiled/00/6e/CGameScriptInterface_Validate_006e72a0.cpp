// CGameScriptInterface::Validate() const -- the world's thing manager validates its classes, a tail jump
// Leaf: ?Validate@CGameScriptInterface@@UBEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E72A0. Identity (NF435, 2026-11-25): the retail RTTI vftable placement of
//   CGameScriptInterface (vtable 0x01260F0C, ghidra_out/labels_rtti_port.tsv) names this PDB member here, and Ego_r.pdb /
//   FableWin.pdb list the symbol (two sources); defined on the real class of engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT).
// FableWin's /Od body: World (+0x04) ?GetThingManager@CWorld@@UAEAAVCThingManager@@XZ (CWorld slot 3, +0x0C), then ?Validate@CClassFactory@@QAEXXZ
//   (CThingManager : CClassFactory; retail 0x00A36FE0, the jmp target).
// Naming (NF446, 2026-11-25): NF435's held draft (pending/2026-11-25_nf435_handback/hold_drafts/006e72a0) landed with the 0x00A36FE0 respell to
//   ?Validate@CClassFactory@@QAEXXZ: the self moves from ?Validate@CGameScriptInterface@@QAEXXZ (non-virtual, non-const stand-in on a local model)
//   to the PDB ?Validate@CGameScriptInterface@@UBEXXZ, and the callee from ?InterruptAllNavigators@CNavigatorManager@@QAEXXZ to the PDB name.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CCLASSFACTORY_VALIDATE
#include "engine/CGameScriptInterface.h"
#include "engine/CWorld.h"
#include "engine/CThingManager.h"

void CGameScriptInterface::Validate() const
{
    World->GetThingManager().Validate();
}
