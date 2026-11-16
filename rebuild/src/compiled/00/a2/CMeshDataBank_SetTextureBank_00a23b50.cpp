// CMeshDataBank::SetTextureBank(const CGraphicDataBank* bank) -- stores the texture bank pointer (+0x3D4)
// Leaf: ?SetTextureBank@CMeshDataBank@@QAEXPBVCGraphicDataBank@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A23B50, length 13 bytes (`mov eax,[esp+4] ; mov [ecx+3D4h],eax ; ret 4`).
// Name fix (NF34, 2026-11-16): Ego_r.pdb + FableWin.pdb ?SetTextureBank@CMeshDataBank@@QAEXPBVCGraphicDataBank@@@Z (engine/CMeshDataBank.h
// declares it at this address); callers 0x00B3CD13 / 0x00B3CE71. Was a placeholder free function (AddExclusiveObserver).
#define FABLE_RETAIL_CMESHDATABANK_SETTEXTUREBANK
#include "engine/CMeshDataBank.h"

void CMeshDataBank::SetTextureBank(const CGraphicDataBank* bank)
{
    TextureBank = const_cast<CGraphicDataBank*>(bank);
}
