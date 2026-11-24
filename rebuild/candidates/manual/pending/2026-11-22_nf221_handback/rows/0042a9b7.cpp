// NUISystem::CManager::SetGraphicsBank(CCountedPointer<CGraphicDataBank> graphics_bank) -- share the bank's object and count block into the manager's PGraphicsBank (+0x10, 0x00419134), the by-value argument then released
// Leaf: ?SetGraphicsBank@CManager@NUISystem@@QAEXV?$CCountedPointer@VCGraphicDataBank@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042A9B7, length 28 bytes, `ret 8`.
// Callees: CCountedPointer<CGraphicDataBank>::ShareData @ 0x00419134 (the inlined assignment), ReleaseData<CGraphicDataBank> @ 0x00419108
//   (the by-value argument's inline destructor).
// NF221 (family q12226 / q12402): was ?SetGraphicsBank@CManager@NUISystem@@QAEXUCCountedPointer_CGraphicDataBank@@@Z on a local
//   CCountedPointer_CGraphicDataBank stand-in; the leaf is the FableWin.pdb / Ego_r.pdb spelling the landed caller 0x0041E3F6 and
//   0x0041E641 already bind, the member is Ego_r.pdb PGraphicsBank (+0x20 there, +0x10 in retail). The counted pointer is now the shared
//   engine/CCountedPointer.h (RELEASEDATA_DECL_ONLY + SHAREDATA_DECL_ONLY: both callees out of line in this size TU).
// Layout: the HEAD row's local CManager model (engine/NUISystem_CManager.h does not reproduce the retail offsets), members respelled in place.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#define FABLE_RETAIL_CCOUNTEDPOINTER_SHAREDATA_DECL_ONLY
#include "engine/CCountedPointer.h"
class CGraphicDataBank;

namespace NUISystem {
class CManager { public:
    unsigned char                     _pad[0x10];
    CCountedPointer<CGraphicDataBank> PGraphicsBank;   // +0x10
    void SetGraphicsBank(CCountedPointer<CGraphicDataBank> graphics_bank);
};
}

void NUISystem::CManager::SetGraphicsBank(CCountedPointer<CGraphicDataBank> graphics_bank)
{
    PGraphicsBank = graphics_bank;
}
