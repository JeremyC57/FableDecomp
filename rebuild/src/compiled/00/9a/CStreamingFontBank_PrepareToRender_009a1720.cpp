// CStreamingFontBank::PrepareToRender(bool) -- set the render state for streamed text: alpha blend (src alpha / inv src alpha), alpha test >= 8, solid fill, CCW cull, z test on, fog off, clipping on, stage 0 linear min/mag filter with point mip and no texture transform; with the flag set, stage 0 colour = diffuse and alpha = texture * diffuse, stage 1 off
// Leaf: ?PrepareToRender@CStreamingFontBank@@UBEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A1720, 2260 bytes, thiscall `ret 4`; `this` is never read.
// Identity (two sources): vtable 0x0129AB54 (RTTI .?AVCStreamingFontBank@@) slot 3 = 0x009A1720, and the Ego_r.pdb / FableWin.pdb
//   symbol ?PrepareToRender@CStreamingFontBank@@UBEX_N@Z (public virtual const, bool argument); CFontBank slot 3 is the pure
//   PrepareToRender(bool = true) that Render2DDrawList 0x009DA9F0 calls through the vtable.
// Callees: GFGetSystemManager 0x009A4EC0 (?GFGetSystemManager@@YIPAVCSystemManager@@XZ). Everything else is the inline
//   CRenderStateManager setters (engine/CRenderStateManager.h retail branch) on the render manager reached through
//   system +0x60 PDisplayManager -> +0x08 PRenderManager (the landed CSurfaceCompositor::SetBlendMode 0x00F38A10 chain). States
//   in retail order (DeviceState offset, value): AlphaBlendEnable +0xA0 = 1, SourceAlphaBlendMode +0x104 = D3DAlphaBlendModeTable[4]
//   0x01396F78, DestAlphaBlendMode +0x118 = [5] 0x01396F7C, AlphaTestEnable +0xB4 = 1, AlphaRef +0xC8 = 8, AlphaTestFunc +0x8C =
//   D3DAlphaFuncModeTable[6] 0x01396DF0, PolyFillMode +0x14 = D3DPolyFillModeTable[0] 0x01396FA4, PolyCullMode +0x78 =
//   D3DPolyCullModeTable[0] 0x01396FB0, ZBufferEnable +0x50 = 1, FogEnable +0x12C = 0, ClippingEnable +0x208 = 1,
//   TextureFilterModeMin[0] +0x8AC / Mag[0] +0x94C = D3DTextureFilterModeTable[1] 0x01396E5C, TextureMipMapMode[0] +0x62C =
//   D3DTextureMipMapModeTable[0] 0x01396E64, TextureTransformMode[0] +0xE4C = D3DTextureTransformModeTable[0] 0x01396FCC; then,
//   if the flag is set: TextureOperation[0] +0x9EC = D3DTextureOperationModeTable[1] 0x01396E74, TextureArgument1[0] +0xB2C =
//   D3DTextureArgumentModeTable[1] 0x01396EEC, TextureAlphaOperation[0] +0xA8C = op[3] 0x01396E7C, TextureAlphaArgument1[0]
//   +0xC6C = arg[2] 0x01396EF0, TextureAlphaArgument2[0] +0xD0C = arg[1], TextureOperation[1] +0xA00 / TextureAlphaOperation[1]
//   +0xAA0 = op[0] 0x01396E70.
// The argument name is descriptive (no PDB parameter names).
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CSTREAMINGFONTBANK
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CStreamingFontBank.h"

void CStreamingFontBank::PrepareToRender(bool textured) const
{
    CRenderManager& rm = GFGetSystemManager()->GetDisplayManager()->GetRenderManager();
    rm.SetAlphaBlendEnable(true);
    rm.SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
    rm.SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
    rm.SetAlphaTestEnable(true);
    rm.SetAlphaRef(8);
    rm.SetAlphaTestFunc(ALPHA_TEST_GREATER_EQUAL);
    rm.SetPolyFillMode(POLY_FILL_SOLID);
    rm.SetPolyCullMode(POLY_CULL_CCW);
    rm.SetZBufferEnable(true);
    rm.SetFogEnable(false);
    rm.SetClippingEnable(true);
    rm.SetTextureFilterModeMin(0, TEXTURE_FILTER_LINEAR);
    rm.SetTextureFilterModeMag(0, TEXTURE_FILTER_LINEAR);
    rm.SetTextureMipMapMode(0, TEXTURE_MIPMAP_POINT);
    rm.SetTextureTransformMode(0, TEXTURE_TRANSFORM_DIABLED);
    if (textured) {
        rm.SetTextureOperation(0, TEXTURE_OP_SELECT_ARG1);
        rm.SetTextureArgument1(0, TEXTURE_ARG_DIFFUSE);
        rm.SetTextureAlphaOperation(0, TEXTURE_OP_MODULATE);
        rm.SetTextureAlphaArgument1(0, TEXTURE_ARG_TEXTURE);
        rm.SetTextureAlphaArgument2(0, TEXTURE_ARG_DIFFUSE);
        rm.SetTextureOperation(1, TEXTURE_OP_DISABLE);
        rm.SetTextureAlphaOperation(1, TEXTURE_OP_DISABLE);
    }
}