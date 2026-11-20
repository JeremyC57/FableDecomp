// CTCFishing::CancelButtonPressed() -- the player cancelled fishing: finish without a catch
// Leaf: ?CancelButtonPressed@CTCFishing@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B0CC0, 8 bytes: push 0 ; call 0x007B0950 ; ret
// Identity (NF120, PRIORITY q6450 binder): Ego_r.pdb 0x007B2600 / FableWin.pdb 0x026D9B60 ?CancelButtonPressed@CTCFishing@@QAEXXZ. Ego_r
//   orders FinishFishing 0x007B2250, ReelInButtonPressed 0x007B24D0, CancelButtonPressed 0x007B2600 (16 bytes with padding), GoFishing,
//   FrameUpdate; retail has FinishFishing 0x007B0950, ReelInButtonPressed 0x007B0BA0, this 8-byte body, FrameUpdate 0x007B0CD0. The body
//   is FinishFishing(false), and its only caller CInputProcessFishing::ProcessInput 0x00690B20 calls it beside ReelInButtonPressed.
//   Was the generated placeholder CClass_007b0cc0::CallWith_007b0cc0 (calling ?Method@CClass_007b0cc0@@QAEXJ@Z).
// Callee: the protected ?FinishFishing@CTCFishing@@IAEX_N@Z 0x007B0950 (both PDBs; the landed FrameUpdate 0x007B0CD0 binds it).
// Header: engine/CTCFishing.h (Codex pass 22 class) with the NF120 opt-in FABLE_RETAIL_CTCFISHING_CANCELBUTTONPRESSED.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CFISHINGDEF_PASS22
#define FABLE_RETAIL_CTCFISHING_PASS22
#define FABLE_RETAIL_CTCFISHING_CANCELBUTTONPRESSED
#include "engine/ETCInterfaceType.h"
#include "engine/CTCFishing.h"

void CTCFishing::CancelButtonPressed()
{
    FinishFishing(false);
}
