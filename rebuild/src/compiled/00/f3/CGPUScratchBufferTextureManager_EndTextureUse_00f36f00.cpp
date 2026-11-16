// CGPUScratchBufferTextureManager::EndTextureUse(long handle) -- nothing to release on the PC build: the empty `ret 4`.
// Leaf: ?EndTextureUse@CGPUScratchBufferTextureManager@@QAEXJ@Z
// NF37 (2026-11-16): moved off the C00f36f00::M stand-in onto the shared engine/CGPUScratchBufferTextureManager.h (TEXTURES opt-in, which
// names 0x00F36F00; Ego_r.pdb / FableWin.pdb ?EndTextureUse@CGPUScratchBufferTextureManager@@QAEXJ@Z, paired with StartTextureUse on the
// same handle at 0x00B2B70F / 0x00B2D97B).
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_TEXTURES
#include "engine/CGPUScratchBufferTextureManager.h"

void CGPUScratchBufferTextureManager::EndTextureUse(long handle)
{
}
