// CLandscapeBackgroundPatch::ClearTextures() -- hand the patch texture to the landscape memory manager's delayed disposal and clear it
// Leaf: ?ClearTextures@CLandscapeBackgroundPatch@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE5C30, 42 bytes: `mov eax,[esi+0B8h] ; test ; je`, push the texture,
//   `mov eax,[01436EA8h] ; mov ecx,[eax+6B0h] ; call 0x00BDA110`, then `mov dword ptr [esi+0B8h],0`.
// Names (NF265, 2026-11-22; two sources each):
//   - self: ?ClearTextures@CLandscapeBackgroundPatch@@QAEXXZ (Ego_r.pdb 009c5c00, FableWin.pdb 02cdccf0; Ego_r donor order
//     ClearTextures / GetEdgeStrip / InvalidateEdge = retail 0x00BE5C30 / 0x00BE5C60 / 0x00BE5CA0; `this` in ecx). Was the free
//     stand-in ?CLandscapeBackgroundPatch_ClearTextures@@YIXPAVCLandscapeBackgroundPatch@@@Z; no landed row references 0x00BE5C30.
//   - the global 0x01436EA8 = ?GPLandscapeRenderer@NEngineInternal@@3PAVCEngineLandscapeRenderer@@A (nf256 / nf260 proof), its
//     MemoryManager at +0x6B0 (was the ?g_obj_01436ea8@@3PAUCGlobal@@A stand-in);
//   - the callee 0x00BDA110 = ?DeleteTexture@CEngineLandscapeMemoryManager@@QAEXPAVCTexture@@@Z (Ego_r.pdb 009c26a0, FableWin.pdb
//     02cb7490; was the ?ReleaseTexture@CTexMgr@@QAEXPAX@Z stand-in).
// Layout: engine/CLandscapeBackgroundPatch.h retail branch (Texture +0xB8, the landed 0x00BE5C60 GetEdgeStrip shape) with the
//   ClearTextures declaration opt-in; engine/CEngineLandscapeMemoryManager.h retail branch with the DeleteTexture declaration opt-in.
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER                 // the retail manager class (0x44 bytes, two bases)
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETETEXTURE   // DeleteTexture 0x00BDA110 (Ego_r + FableWin)
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER  // NEngineInternal::GPLandscapeRenderer, 0x01436EA8
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH_CLEARTEXTURES     // ClearTextures 0x00BE5C30 (Ego_r + FableWin)
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CLandscapeBackgroundPatch.h"

void CLandscapeBackgroundPatch::ClearTextures()
{
    CTexture* texture = Texture;
    if (texture) {
        NEngineInternal::GPLandscapeRenderer->MemoryManager->DeleteTexture(texture);
        Texture = 0;
    }
}
