// CRenderStateManager::SetupRenderStates -- retail 0x00A04630, 4609 bytes (CRenderStateManager.cpp).
// Reads the device caps (IDirect3DDevice9::GetDeviceCaps, vtable slot 7), then registers every CStateInfo of DeviceState in
// AllStatesList through the three private Setup* helpers (out-of-line copies 0x00A04580 / 0x00A045B0 / 0x00A045F0, defined just
// before this function in the same TU and auto-inlined here): the render states in retail order, the eight D3DRS_WRAPn states from
// the private table D3DTextureCoordWrapStageTable 0x01396DA8, depth bias states only when RasterCaps allows them, then per stage the
// sampler and texture-stage states (MIPMAPLODBIAS only with the caps bit, TEXTURETRANSFORMFLAGS only without InitFlags bit 2).
// Callees: IDirect3DDevice9::GetDeviceCaps ([[this+4]]+0x1C). No other calls.
// Leaf: ?SetupRenderStates@CRenderStateManager@@AAEXXZ
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#include "engine/IDirect3DDevice9.h"
#include "engine/Direct3D9Types.h"
#include "engine/CRenderStateManager.h"

// d3d9types.h enumerators used here (the engine headers only forward-declare these enums).
enum _D3DRENDERSTATETYPE {
    D3DRS_ZENABLE = 7,
    D3DRS_FILLMODE = 8,
    D3DRS_SHADEMODE = 9,
    D3DRS_ZWRITEENABLE = 14,
    D3DRS_ALPHATESTENABLE = 15,
    D3DRS_SRCBLEND = 19,
    D3DRS_DESTBLEND = 20,
    D3DRS_CULLMODE = 22,
    D3DRS_ZFUNC = 23,
    D3DRS_ALPHAREF = 24,
    D3DRS_ALPHAFUNC = 25,
    D3DRS_DITHERENABLE = 26,
    D3DRS_ALPHABLENDENABLE = 27,
    D3DRS_FOGENABLE = 28,
    D3DRS_SPECULARENABLE = 29,
    D3DRS_FOGCOLOR = 34,
    D3DRS_FOGTABLEMODE = 35,
    D3DRS_FOGSTART = 36,
    D3DRS_FOGEND = 37,
    D3DRS_FOGDENSITY = 38,
    D3DRS_STENCILENABLE = 52,
    D3DRS_STENCILFAIL = 53,
    D3DRS_STENCILZFAIL = 54,
    D3DRS_STENCILPASS = 55,
    D3DRS_STENCILFUNC = 56,
    D3DRS_STENCILREF = 57,
    D3DRS_STENCILMASK = 58,
    D3DRS_STENCILWRITEMASK = 59,
    D3DRS_TEXTUREFACTOR = 60,
    D3DRS_CLIPPING = 136,
    D3DRS_LIGHTING = 137,
    D3DRS_FOGVERTEXMODE = 140,
    D3DRS_NORMALIZENORMALS = 143,
    D3DRS_DIFFUSEMATERIALSOURCE = 145,
    D3DRS_SPECULARMATERIALSOURCE = 146,
    D3DRS_AMBIENTMATERIALSOURCE = 147,
    D3DRS_VERTEXBLEND = 151,
    D3DRS_POINTSIZE = 154,
    D3DRS_POINTSIZE_MIN = 155,
    D3DRS_POINTSPRITEENABLE = 156,
    D3DRS_POINTSCALEENABLE = 157,
    D3DRS_POINTSCALE_A = 158,
    D3DRS_POINTSCALE_B = 159,
    D3DRS_POINTSCALE_C = 160,
    D3DRS_POINTSIZE_MAX = 166,
    D3DRS_COLORWRITEENABLE = 168,
    D3DRS_BLENDOP = 171,
    D3DRS_SLOPESCALEDEPTHBIAS = 175,
    D3DRS_DEPTHBIAS = 195,
    D3DRS_FORCE_DWORD = 0x7fffffff
};
enum _D3DTEXTURESTAGESTATETYPE {
    D3DTSS_COLOROP = 1,
    D3DTSS_COLORARG1 = 2,
    D3DTSS_COLORARG2 = 3,
    D3DTSS_ALPHAOP = 4,
    D3DTSS_ALPHAARG1 = 5,
    D3DTSS_ALPHAARG2 = 6,
    D3DTSS_BUMPENVMAT00 = 7,
    D3DTSS_BUMPENVMAT01 = 8,
    D3DTSS_BUMPENVMAT10 = 9,
    D3DTSS_BUMPENVMAT11 = 10,
    D3DTSS_TEXCOORDINDEX = 11,
    D3DTSS_TEXTURETRANSFORMFLAGS = 24,
    D3DTSS_FORCE_DWORD = 0x7fffffff
};
enum _D3DSAMPLERSTATETYPE {
    D3DSAMP_ADDRESSU = 1,
    D3DSAMP_ADDRESSV = 2,
    D3DSAMP_ADDRESSW = 3,
    D3DSAMP_BORDERCOLOR = 4,
    D3DSAMP_MAGFILTER = 5,
    D3DSAMP_MINFILTER = 6,
    D3DSAMP_MIPFILTER = 7,
    D3DSAMP_MIPMAPLODBIAS = 8,
    D3DSAMP_MAXANISOTROPY = 10,
    D3DSAMP_FORCE_DWORD = 0x7fffffff
};
#define D3DPRASTERCAPS_MIPMAPLODBIAS        0x00002000L
#define D3DPRASTERCAPS_SLOPESCALEDEPTHBIAS  0x02000000L
#define D3DPRASTERCAPS_DEPTHBIAS            0x04000000L

// 0x00A04580
void CRenderStateManager::SetupRenderState(CStateInfo& info, _D3DRENDERSTATETYPE state)
{
    info.CurrentState = 0;
    info.DesiredState = 0;
    info.DirtyListFlag = false;
    info.StateType = 1;
    info.RenderStateParam = state;
    AllStatesList[AllStatesListSize++] = &info;
}

// 0x00A045B0
void CRenderStateManager::SetupTextureStageState(CStateInfo& info, _D3DTEXTURESTAGESTATETYPE type, long stage)
{
    info.CurrentState = 0;
    info.DesiredState = 0;
    info.DirtyListFlag = false;
    info.StateType = 2;
    info.TextureStageStateParam = type;
    info.TextureStage = (unsigned char)stage;
    AllStatesList[AllStatesListSize++] = &info;
}

// 0x00A045F0
void CRenderStateManager::SetupSamplerStageState(CStateInfo& info, _D3DSAMPLERSTATETYPE type, long stage)
{
    info.CurrentState = 0;
    info.DesiredState = 0;
    info.DirtyListFlag = false;
    info.StateType = 3;
    info.SamplerStageStateParam = type;
    info.TextureStage = (unsigned char)stage;
    AllStatesList[AllStatesListSize++] = &info;
}

void CRenderStateManager::SetupRenderStates()
{
    D3DCAPS9 caps;
    PD3DDevice->GetDeviceCaps(&caps);

    SetupRenderState(DeviceState.PolyShadeMode, D3DRS_SHADEMODE);
    SetupRenderState(DeviceState.PolyFillMode, D3DRS_FILLMODE);
    SetupRenderState(DeviceState.ColourWriteEnable, D3DRS_COLORWRITEENABLE);
    SetupRenderState(DeviceState.ZBufferWriteEnable, D3DRS_ZWRITEENABLE);
    SetupRenderState(DeviceState.ZBufferFunc, D3DRS_ZFUNC);
    SetupRenderState(DeviceState.ZBufferEnable, D3DRS_ZENABLE);
    SetupRenderState(DeviceState.PolyCullMode, D3DRS_CULLMODE);
    SetupRenderState(DeviceState.SourceAlphaBlendMode, D3DRS_SRCBLEND);
    SetupRenderState(DeviceState.DestAlphaBlendMode, D3DRS_DESTBLEND);
    SetupRenderState(DeviceState.AlphaBlendEnable, D3DRS_ALPHABLENDENABLE);
    SetupRenderState(DeviceState.AlphaTestEnable, D3DRS_ALPHATESTENABLE);
    SetupRenderState(DeviceState.AlphaTestFunc, D3DRS_ALPHAFUNC);
    SetupRenderState(DeviceState.AlphaRef, D3DRS_ALPHAREF);
    SetupRenderState(DeviceState.VertexBlendMode, D3DRS_VERTEXBLEND);
    SetupRenderState(DeviceState.FogEnable, D3DRS_FOGENABLE);
    SetupRenderState(DeviceState.DitherEnable, D3DRS_DITHERENABLE);
    SetupRenderState(DeviceState.FogStart, D3DRS_FOGSTART);
    SetupRenderState(DeviceState.FogEnd, D3DRS_FOGEND);
    SetupRenderState(DeviceState.FogDensity, D3DRS_FOGDENSITY);
    SetupRenderState(DeviceState.FogColour, D3DRS_FOGCOLOR);
    SetupRenderState(DeviceState.FogMode, D3DRS_FOGTABLEMODE);
    SetupRenderState(DeviceState.FogVertexMode, D3DRS_FOGVERTEXMODE);
    SetupRenderState(DeviceState.TextureFactor, D3DRS_TEXTUREFACTOR);
    SetupRenderState(DeviceState.SpecularEnable, D3DRS_SPECULARENABLE);
    SetupRenderState(DeviceState.LightingEnable, D3DRS_LIGHTING);
    SetupRenderState(DeviceState.NormalizeNormals, D3DRS_NORMALIZENORMALS);
    SetupRenderState(DeviceState.AmbientMaterialColourSource, D3DRS_AMBIENTMATERIALSOURCE);
    SetupRenderState(DeviceState.DiffuseMaterialColourSource, D3DRS_DIFFUSEMATERIALSOURCE);
    SetupRenderState(DeviceState.SpecularMaterialColourSource, D3DRS_SPECULARMATERIALSOURCE);
    SetupRenderState(DeviceState.PointSpriteEnable, D3DRS_POINTSPRITEENABLE);
    SetupRenderState(DeviceState.PointScaleEnable, D3DRS_POINTSCALEENABLE);
    SetupRenderState(DeviceState.PointSize, D3DRS_POINTSIZE);
    SetupRenderState(DeviceState.PointSizeMin, D3DRS_POINTSIZE_MIN);
    SetupRenderState(DeviceState.PointSizeMax, D3DRS_POINTSIZE_MAX);
    SetupRenderState(DeviceState.PointScaleConstantFactor, D3DRS_POINTSCALE_A);
    SetupRenderState(DeviceState.PointScaleDistanceFactor, D3DRS_POINTSCALE_B);
    SetupRenderState(DeviceState.PointScaleDistanceSqrFactor, D3DRS_POINTSCALE_C);
    SetupRenderState(DeviceState.BlendOpMode, D3DRS_BLENDOP);
    SetupRenderState(DeviceState.StencilEnable, D3DRS_STENCILENABLE);
    SetupRenderState(DeviceState.StencilOpFail, D3DRS_STENCILFAIL);
    SetupRenderState(DeviceState.StencilOpZFail, D3DRS_STENCILZFAIL);
    SetupRenderState(DeviceState.StencilOpPass, D3DRS_STENCILPASS);
    SetupRenderState(DeviceState.StencilFunc, D3DRS_STENCILFUNC);
    SetupRenderState(DeviceState.StencilRef, D3DRS_STENCILREF);
    SetupRenderState(DeviceState.StencilMask, D3DRS_STENCILMASK);
    SetupRenderState(DeviceState.StencilWriteMask, D3DRS_STENCILWRITEMASK);
    SetupRenderState(DeviceState.TextureCoordWrapMode[0], D3DTextureCoordWrapStageTable[0]);
    SetupRenderState(DeviceState.TextureCoordWrapMode[1], D3DTextureCoordWrapStageTable[1]);
    SetupRenderState(DeviceState.TextureCoordWrapMode[2], D3DTextureCoordWrapStageTable[2]);
    SetupRenderState(DeviceState.TextureCoordWrapMode[3], D3DTextureCoordWrapStageTable[3]);
    SetupRenderState(DeviceState.TextureCoordWrapMode[4], D3DTextureCoordWrapStageTable[4]);
    SetupRenderState(DeviceState.TextureCoordWrapMode[5], D3DTextureCoordWrapStageTable[5]);
    SetupRenderState(DeviceState.TextureCoordWrapMode[6], D3DTextureCoordWrapStageTable[6]);
    SetupRenderState(DeviceState.TextureCoordWrapMode[7], D3DTextureCoordWrapStageTable[7]);
    SetupRenderState(DeviceState.ClippingEnable, D3DRS_CLIPPING);
    if (caps.RasterCaps & D3DPRASTERCAPS_DEPTHBIAS)
        SetupRenderState(DeviceState.DepthOffset, D3DRS_DEPTHBIAS);
    if (caps.RasterCaps & D3DPRASTERCAPS_SLOPESCALEDEPTHBIAS)
        SetupRenderState(DeviceState.DepthOffsetSlope, D3DRS_SLOPESCALEDEPTHBIAS);
    for (long stage = 0; stage < 8; ++stage)
    {
        SetupSamplerStageState(DeviceState.TextureMipMapMode[stage], D3DSAMP_MIPFILTER, stage);
        SetupSamplerStageState(DeviceState.TextureUVModeU[stage], D3DSAMP_ADDRESSU, stage);
        SetupSamplerStageState(DeviceState.TextureUVModeV[stage], D3DSAMP_ADDRESSV, stage);
        SetupSamplerStageState(DeviceState.TextureUVModeW[stage], D3DSAMP_ADDRESSW, stage);
        SetupSamplerStageState(DeviceState.TextureFilterModeMag[stage], D3DSAMP_MAGFILTER, stage);
        SetupSamplerStageState(DeviceState.TextureFilterModeMin[stage], D3DSAMP_MINFILTER, stage);
        if (caps.RasterCaps & D3DPRASTERCAPS_MIPMAPLODBIAS)
            SetupSamplerStageState(DeviceState.TextureMipmapLODBias[stage], D3DSAMP_MIPMAPLODBIAS, stage);
        SetupSamplerStageState(DeviceState.TextureBorderColour[stage], D3DSAMP_BORDERCOLOR, stage);
        SetupSamplerStageState(DeviceState.TextureMaxAnisotropy[stage], D3DSAMP_MAXANISOTROPY, stage);
        SetupTextureStageState(DeviceState.TextureOperation[stage], D3DTSS_COLOROP, stage);
        SetupTextureStageState(DeviceState.TextureAlphaOperation[stage], D3DTSS_ALPHAOP, stage);
        SetupTextureStageState(DeviceState.TextureArgument1[stage], D3DTSS_COLORARG1, stage);
        SetupTextureStageState(DeviceState.TextureArgument2[stage], D3DTSS_COLORARG2, stage);
        SetupTextureStageState(DeviceState.TextureAlphaArgument1[stage], D3DTSS_ALPHAARG1, stage);
        SetupTextureStageState(DeviceState.TextureAlphaArgument2[stage], D3DTSS_ALPHAARG2, stage);
        SetupTextureStageState(DeviceState.TextureCoordIndex[stage], D3DTSS_TEXCOORDINDEX, stage);
        if (!(InitFlags & 2))
            SetupTextureStageState(DeviceState.TextureTransformMode[stage], D3DTSS_TEXTURETRANSFORMFLAGS, stage);
        SetupTextureStageState(DeviceState.TextureBumpEnvMat00[stage], D3DTSS_BUMPENVMAT00, stage);
        SetupTextureStageState(DeviceState.TextureBumpEnvMat10[stage], D3DTSS_BUMPENVMAT10, stage);
        SetupTextureStageState(DeviceState.TextureBumpEnvMat01[stage], D3DTSS_BUMPENVMAT01, stage);
        SetupTextureStageState(DeviceState.TextureBumpEnvMat11[stage], D3DTSS_BUMPENVMAT11, stage);
    }
}