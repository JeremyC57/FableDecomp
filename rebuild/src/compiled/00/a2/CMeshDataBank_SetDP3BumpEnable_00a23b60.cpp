// CMeshDataBank::SetDP3BumpEnable(bool enable) -- stores the DP3 bump flag (+0x3D8)
// Leaf: ?SetDP3BumpEnable@CMeshDataBank@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A23B60, length 13 bytes (`mov al,[esp+4] ; mov [ecx+3D8h],al ; ret 4`).
// Name fix (NF34, 2026-11-16): FableWin.pdb + Ego_r.pdb ?SetDP3BumpEnable@CMeshDataBank@@QAEX_N@Z (engine/CMeshDataBank.h opt-in, field
// DP3BumpEnable +0x3D8); caller 0x00B3470D. Was a placeholder stand-in class (C00a23b60::M).
#define FABLE_RETAIL_CMESHDATABANK_SETDP3BUMPENABLE
#include "engine/CMeshDataBank.h"

void CMeshDataBank::SetDP3BumpEnable(bool enable)
{
    DP3BumpEnable = enable;
}
