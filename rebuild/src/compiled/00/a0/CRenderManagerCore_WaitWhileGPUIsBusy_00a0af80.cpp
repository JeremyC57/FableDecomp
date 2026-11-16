// CRenderManagerCore::WaitWhileGPUIsBusy() -- empty on the PC build (`ret`).
// Leaf: ?WaitWhileGPUIsBusy@CRenderManagerCore@@QAEXXZ
// NF37 (2026-11-16): moved off the C00a0af80::M stand-in onto the shared engine/CRenderManagerCore.h (WAITWHILEGPUISBUSY opt-in, which names
// 0x00A0AF80; Ego_r.pdb 0x00B74540 / FableWin.pdb ?WaitWhileGPUIsBusy@CRenderManagerCore@@QAEXXZ; called on GPRenderManager at 0x00B2D392).
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_WAITWHILEGPUISBUSY
#include "engine/CRenderManagerCore.h"

void CRenderManagerCore::WaitWhileGPUIsBusy()
{
}
