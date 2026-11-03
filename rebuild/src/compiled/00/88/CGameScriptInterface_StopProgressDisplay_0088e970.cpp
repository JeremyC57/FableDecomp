// CGameScriptInterface::StopProgressDisplay() const -- script call: close the progress display (NProgressDisplay::StopProgress(true, false))
// Leaf: ?StopProgressDisplay@CGameScriptInterface@@UBEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088E970, 9 bytes: `xor dl,dl ; mov cl,1 ; jmp 0x009E9F20`.
// Identity: retail CGameScriptInterface vtable 0x01260F0C slot 392 (engine/CGameScriptInterface.h; Ego_r.pdb / FableWin.pdb
//   ?StopProgressDisplay@CGameScriptInterface@@UBEXXZ); the twin of the landed slot 391 StartProgressDisplay 0x0088E950.
// Callee: 0x009E9F20 = Ego_r.pdb / FableWin.pdb ?StopProgress@NProgressDisplay@@YIX_N0@Z (a namespace-scope __fastcall
//   function: the two bools travel in cl / dl, so `this` is never loaded). The bool parameter names are not in the PDB
//   name sets (descriptive).
namespace NProgressDisplay {
    void __fastcall StopProgress(bool finished, bool keepScreen);    // 0x009E9F20
}
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::StopProgressDisplay() const
{
    NProgressDisplay::StopProgress(true, false);
}