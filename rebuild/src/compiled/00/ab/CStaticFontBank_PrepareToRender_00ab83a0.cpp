// CStaticFontBank::PrepareToRender(bool) -- set the render state for text from the static glyph texture: alpha blend (src alpha / inv src alpha), alpha test >= 8, solid fill, CCW cull, z test on, fog off, clipping on, stage 0 linear min/mag filter, point mip, wrap U/V, no texture transform; with the flag set, the XYZRHW|DIFFUSE|TEX1 vertex format, stage 0 coordinate set 0, colour and alpha = texture * diffuse, stage 1 off
// Leaf: ?PrepareToRender@CStaticFontBank@@UBEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB83A0, 2672 bytes, thiscall `ret 4`; `this` is never read.
// Identity (two sources): vtable 0x0129EAD4 (RTTI .?AVCStaticFontBank@@) slot 3 = 0x00AB83A0, and the Ego_r.pdb / FableWin.pdb
//   symbol ?PrepareToRender@CStaticFontBank@@UBEX_N@Z (public virtual const, bool argument). Sibling of
//   CStreamingFontBank::PrepareToRender 0x009A1720 (the same first eleven states; this one adds the UV wrap modes, the vertex format
//   and the coordinate index, and a modulated colour stage).
// Callees: GFGetSystemManager 0x009A4EC0 (?GFGetSystemManager@@YIPAVCSystemManager@@XZ) and CRenderManagerCore::SetVertexFormat
//   0x00A0AA30 (out of line, landed: SetFVF through the device; 0x144 = D3DFVF_XYZRHW 0x004 | D3DFVF_DIFFUSE 0x040 | D3DFVF_TEX1
//   0x100). Everything else is the inline CRenderStateManager setters (engine/CRenderStateManager.h retail branch) on the render
//   manager (system +0x60 PDisplayManager -> +0x08 PRenderManager, kept in esi across the call). States in retail order
//   (DeviceState offset, value): AlphaBlendEnable +0xA0 = 1, SourceAlphaBlendMode +0x104 = D3DAlphaBlendModeTable[4], DestAlphaBlendMode
//   +0x118 = [5], AlphaTestEnable +0xB4 = 1, AlphaRef +0xC8 = 8, AlphaTestFunc +0x8C = D3DAlphaFuncModeTable[6], PolyFillMode +0x14 =
//   [0], PolyCullMode +0x78 = [0], ZBufferEnable +0x50 = 1, FogEnable +0x12C = 0, ClippingEnable +0x208 = 1, TextureFilterModeMin[0]
//   +0x8AC / Mag[0] +0x94C = filter[1], TextureMipMapMode[0] +0x62C = mip[0], TextureUVModeU[0] +0x6CC / V[0] +0x76C =
//   D3DTextureUVModeTable[0] 0x01396DC8, TextureTransformMode[0] +0xE4C = [0]; then, if the flag is set: SetVertexFormat(0x144),
//   TextureCoordIndex[0] +0xDAC = 0, TextureOperation[0] +0x9EC = op[3] 0x01396E7C, TextureArgument1[0] +0xB2C = arg[2] 0x01396EF0,
//   TextureArgument2[0] +0xBCC = arg[1] 0x01396EEC, TextureAlphaOperation[0] +0xA8C = op[3], TextureAlphaArgument1[0] +0xC6C = arg[2],
//   TextureAlphaArgument2[0] +0xD0C = arg[1], TextureOperation[1] +0xA00 / TextureAlphaOperation[1] +0xAA0 = op[0] 0x01396E70.
// The argument name is descriptive (no PDB parameter names).
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CSTATICFONTBANK
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CStaticFontBank.h"

void CStaticFontBank::PrepareToRender(bool textured) const
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
    rm.SetTextureUVModeU(0, TEXTURE_UV_WRAP);
    rm.SetTextureUVModeV(0, TEXTURE_UV_WRAP);
    rm.SetTextureTransformMode(0, TEXTURE_TRANSFORM_DIABLED);
    if (textured) {
        rm.SetVertexFormat(0x144);
        rm.SetTextureCoordIndex(0, 0);
        rm.SetTextureOperation(0, TEXTURE_OP_MODULATE);
        rm.SetTextureArgument1(0, TEXTURE_ARG_TEXTURE);
        rm.SetTextureArgument2(0, TEXTURE_ARG_DIFFUSE);
        rm.SetTextureAlphaOperation(0, TEXTURE_OP_MODULATE);
        rm.SetTextureAlphaArgument1(0, TEXTURE_ARG_TEXTURE);
        rm.SetTextureAlphaArgument2(0, TEXTURE_ARG_DIFFUSE);
        rm.SetTextureOperation(1, TEXTURE_OP_DISABLE);
        rm.SetTextureAlphaOperation(1, TEXTURE_OP_DISABLE);
    }
}