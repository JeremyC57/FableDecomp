#include "rebuild_abi.h"
// Leaf: @FableDualDestval_009c2520@4
// _Dest_val<...> @ 0x009c2520
// push esi; mov esi,ecx; lea ecx,[esi+0x18]; call member_dtor; lea ecx,[esi+0x14]; pop esi; jmp base_dtor
// Destroy the embedded member at +0x18, then tail-jump the base destructor at +0x14.
// Naming (NF142, 2026-11-20, q4296): the FableMember / FableBase stand-ins replaced by engine/CQuickDrawTriInfo.h. The body is the
//   implicit CQuickDrawTriInfo destructor: UploadedTexture (CTexture +0x18; inline ~CTexture = ?Uninitialise@CTexture@@QAEXXZ 0x009F9F70,
//   Ego_r.pdb 0x00B69E70 + FableWin.pdb 0x030DE280), then String (CWideString +0x14; ??1CWideString@@QAE@XZ 0x0099B510). Ego_r.pdb
//   CQuickDrawTriInfo (60 bytes) is the only PDB class with CWideString at +0x14 and CTexture at +0x18. The propagated _Dest_val label and
//   the own extern "C" leaf are unchanged (0x009C1AC0 is a byte twin; one of them may be the FableWin.pdb ??1CQuickDrawTriInfo@@QAE@XZ).
#include "engine/CQuickDrawTriInfo.h"
extern "C" void FABLE_FASTCALL FableDualDestval_009c2520(CQuickDrawTriInfo *self)
{
    self->~CQuickDrawTriInfo();
}
