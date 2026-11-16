// CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures() -- for each of the three dynamic textures still held (OutlineTexture +0x0C /
// Blur1Texture +0x10 / Blur2Texture +0x14), end its use in the resource manager's GPU scratch-buffer texture manager (by its handle +0x18 /
// +0x1C / +0x20) and clear the pointer
// Leaf: ?FinishWithDynamicTextures@CEngineScreenEffectOutlineGlow@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B89080, length 94 bytes.
// Callees: CGPUScratchBufferTextureManager::EndTextureUse 0x00F36F00 three times, on NEngineInternal::GPResourceManager (0x01436E98) + 0x3190, kept
//   in edi. Shape: the manager address is formed before the first test; each arm pushes its handle, calls, and clears its pointer with an
//   immediate.
// NF39 (2026-11-16): moved off its `ReleaseTextures_00b89080` stand-in (local manager / engine-global / class models) onto the shared
//   engine/CEngineScreenEffectOutlineGlow.h (MEMBERS opt-in, which names 0x00B89080), engine/CEngineResourceManager.h and
//   engine/CGPUScratchBufferTextureManager.h (TEXTURES: EndTextureUse 0x00F36F00). Evidence: Ego_r.pdb donor order SetupDepthBufferTransform
//   0x00A2FD30, FinishWithDynamicTextures 0x00A2FD40 (size 0x60 = retail 94 bytes + pad), FinishOutlineRendering 0x00A2FDF0; FableWin.pdb has the same
//   private member; the landed caller already binds it. Global spelled as in the landed constructor 0x00B891B0 (reloc_bindings.tsv).
// Defines: the landed 0x00B891B0 constructor preamble.
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_MEMBERS
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CGPUSCRATCHBUFFERTEXTUREMANAGER_TEXTURES
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE_CTOR
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CRenderTarget.h"
#include "engine/CGPUScratchBufferTextureManager.h"
#include "fable_string.h"
#include "engine/CEngineScreenEffectOutlineGlow.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CShaderResource.h"
#include "engine/CEngineInit.h"
#include "engine/CPixelFormat.h"
#include "engine/C2DExtentsI.h"

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }    // 0x01436E98

void CEngineScreenEffectOutlineGlow::FinishWithDynamicTextures()
{
    CGPUScratchBufferTextureManager& scratch = NEngineInternal::GPResourceManager->GetGPUScratchBufferTextureManager();
    if (OutlineTexture) {
        scratch.EndTextureUse(OutlineTextureHandle);
        OutlineTexture = 0;
    }
    if (Blur1Texture) {
        scratch.EndTextureUse(Blur1TextureHandle);
        Blur1Texture = 0;
    }
    if (Blur2Texture) {
        scratch.EndTextureUse(Blur2TextureHandle);
        Blur2Texture = 0;
    }
}
