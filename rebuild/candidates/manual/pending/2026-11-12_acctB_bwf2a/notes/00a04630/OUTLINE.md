# OUTLINE 0x00A04630 CRenderStateManager::SetupRenderStates  (retail order)

## Frame
- Prologue `sub esp,134h ; push ebx,ebp,esi,edi ; mov esi,ecx` (no ebp frame, /Oy). Epilogue 0xA05826 `pop edi,esi,ebp,ebx ;
  add esp,134h ; ret`. No EH, no /GS. Saved order ebx, ebp, esi, edi.
- [esp+10h] long stage (loop counter home; ebx in the loop) ; [esp+14h..144h) D3DCAPS9 caps (no ctor/dtor; RasterCaps at [esp+38h]).
- Register constants: esi = this, eax = 0, edx = 1 (StateType 1 and value 1).

## Helpers (define in the row before SetupRenderStates, plain member functions; auto-inlined)
SR(info,state)   = SetupRenderState(DeviceState.<info>, (_D3DRENDERSTATETYPE)state)            -> StateType 1
SS(info,t,stage) = SetupSamplerStageState(DeviceState.<info>[stage], (_D3DSAMPLERSTATETYPE)t, stage) -> StateType 3
ST(info,t,stage) = SetupTextureStageState(DeviceState.<info>[stage], (_D3DTEXTURESTAGESTATETYPE)t, stage) -> StateType 2
(out-of-line oracles 0x00A04580 / 0x00A045F0 / 0x00A045B0)

## Body
```
void CRenderStateManager::SetupRenderStates()
{
    D3DCAPS9 caps;
    PD3DDevice->GetDeviceCaps(&caps);                         // 0xA0463C..0xA04647, vtbl+1Ch
    SR(PolyShadeMode,              D3DRS_SHADEMODE 9);        // 0xA0464A
    SR(PolyFillMode,               D3DRS_FILLMODE 8);         // 0xA04689
    SR(ColourWriteEnable,          D3DRS_COLORWRITEENABLE 0xA8);   // 0xA046C4
    SR(ZBufferWriteEnable,         D3DRS_ZWRITEENABLE 0xE);   // 0xA046FF
    SR(ZBufferFunc,                D3DRS_ZFUNC 0x17);         // 0xA0473A
    SR(ZBufferEnable,              D3DRS_ZENABLE 7);          // 0xA04775
    SR(PolyCullMode,               D3DRS_CULLMODE 0x16);      // 0xA047A7
    SR(SourceAlphaBlendMode,       D3DRS_SRCBLEND 0x13);      // 0xA047D2
    SR(DestAlphaBlendMode,         D3DRS_DESTBLEND 0x14);     // 0xA0480D
    SR(AlphaBlendEnable,           D3DRS_ALPHABLENDENABLE 0x1B); // 0xA04848
    SR(AlphaTestEnable,            D3DRS_ALPHATESTENABLE 0xF);  // 0xA04883
    SR(AlphaTestFunc,              D3DRS_ALPHAFUNC 0x19);     // 0xA048B5
    SR(AlphaRef,                   D3DRS_ALPHAREF 0x18);      // 0xA048E7
    SR(VertexBlendMode,            D3DRS_VERTEXBLEND 0x97);   // 0xA04912
    SR(FogEnable,                  D3DRS_FOGENABLE 0x1C);     // 0xA04944
    SR(DitherEnable,               D3DRS_DITHERENABLE 0x1A);  // 0xA0497F
    SR(FogStart,                   D3DRS_FOGSTART 0x24);      // 0xA049BA
    SR(FogEnd,                     D3DRS_FOGEND 0x25);        // 0xA049F5
    SR(FogDensity,                 D3DRS_FOGDENSITY 0x26);    // 0xA04A27
    SR(FogColour,                  D3DRS_FOGCOLOR 0x22);      // 0xA04A59
    SR(FogMode,                    D3DRS_FOGTABLEMODE 0x23);  // 0xA04A8B
    SR(FogVertexMode,              D3DRS_FOGVERTEXMODE 0x8C); // 0xA04AB6
    SR(TextureFactor,              D3DRS_TEXTUREFACTOR 0x3C); // 0xA04AF1
    SR(SpecularEnable,             D3DRS_SPECULARENABLE 0x1D);// 0xA04B2C
    SR(LightingEnable,             D3DRS_LIGHTING 0x89);      // 0xA04B67
    SR(NormalizeNormals,           D3DRS_NORMALIZENORMALS 0x8F);       // 0xA04BA2
    SR(AmbientMaterialColourSource,D3DRS_AMBIENTMATERIALSOURCE 0x93);  // 0xA04BD4
    SR(DiffuseMaterialColourSource,D3DRS_DIFFUSEMATERIALSOURCE 0x91);  // 0xA04C06
    SR(SpecularMaterialColourSource,D3DRS_SPECULARMATERIALSOURCE 0x92);// 0xA04C38
    SR(PointSpriteEnable,          D3DRS_POINTSPRITEENABLE 0x9C);      // 0xA04C63
    SR(PointScaleEnable,           D3DRS_POINTSCALEENABLE 0x9D);       // 0xA04C9E
    SR(PointSize,                  D3DRS_POINTSIZE 0x9A);     // 0xA04CD9
    SR(PointSizeMin,               D3DRS_POINTSIZE_MIN 0x9B); // 0xA04D14
    SR(PointSizeMax,               D3DRS_POINTSIZE_MAX 0xA6); // 0xA04D4F
    SR(PointScaleConstantFactor,   D3DRS_POINTSCALE_A 0x9E);  // 0xA04D81
    SR(PointScaleDistanceFactor,   D3DRS_POINTSCALE_B 0x9F);  // 0xA04DB3
    SR(PointScaleDistanceSqrFactor,D3DRS_POINTSCALE_C 0xA0);  // 0xA04DE5
    SR(BlendOpMode,                D3DRS_BLENDOP 0xAB);       // 0xA04E20
    SR(StencilEnable,              D3DRS_STENCILENABLE 0x34); // 0xA04E5B
    SR(StencilOpFail,              D3DRS_STENCILFAIL 0x35);   // 0xA04E96
    SR(StencilOpZFail,             D3DRS_STENCILZFAIL 0x36);  // 0xA04ED1
    SR(StencilOpPass,              D3DRS_STENCILPASS 0x37);   // 0xA04F0C
    SR(StencilFunc,                D3DRS_STENCILFUNC 0x38);   // 0xA04F3E
    SR(StencilRef,                 D3DRS_STENCILREF 0x39);    // 0xA04F70
    SR(StencilMask,                D3DRS_STENCILMASK 0x3A);   // 0xA04FAB
    SR(StencilWriteMask,           D3DRS_STENCILWRITEMASK 0x3B);       // 0xA04FE6
    // 8 explicit lines (VC7.1 does not unroll; written out) from the private static table 0x01396DA8 = {D3DRS_WRAP0..7}
    SR(TextureCoordWrapMode[0], D3DTextureCoordWrapStageTable[0]);   // 0xA05021
    ... [1] 0xA0505E, [2] 0xA0509B, [3] 0xA050E5 (lea ecx first), [4] 0xA0510C, [5] 0xA05149, [6] 0xA05186, [7] 0xA051C3
    SR(ClippingEnable,             D3DRS_CLIPPING 0x88);      // 0xA05200
    if (caps.RasterCaps & D3DPRASTERCAPS_DEPTHBIAS /*0x04000000*/)          // 0xA05232 test ecx ; je 0xA05270
        SR(DepthOffset,            D3DRS_DEPTHBIAS 0xC3);     // 0xA05245
    if (caps.RasterCaps & D3DPRASTERCAPS_SLOPESCALEDEPTHBIAS /*0x02000000*/) // 0xA05270 test [esp+38h] ; je 0xA052A5
        SR(DepthOffsetSlope,       D3DRS_SLOPESCALEDEPTHBIAS 0xAF); // 0xA0527A
    for (long stage = 0; stage < 8; ++stage)                  // 0xA052A5 xor ebx ; lea ecx,[esi+2E54h] ; jmp 0xA052B3
    {                                                         // loop top 0xA052AF (mov ebx,[esp+10h]) / 0xA052B3
        SS(TextureMipMapMode,   D3DSAMP_MIPFILTER 7,  stage); // 0xA052B3
        SS(TextureUVModeU,      D3DSAMP_ADDRESSU 1,   stage); // 0xA052DF (value edx)
        SS(TextureUVModeV,      D3DSAMP_ADDRESSV 2,   stage); // 0xA0531D
        SS(TextureUVModeW,      D3DSAMP_ADDRESSW 3,   stage); // 0xA0535F
        SS(TextureFilterModeMag,D3DSAMP_MAGFILTER 5,  stage); // 0xA053A1
        SS(TextureFilterModeMin,D3DSAMP_MINFILTER 6,  stage); // 0xA053E3
        if (caps.RasterCaps & D3DPRASTERCAPS_MIPMAPLODBIAS /*0x2000*/)   // 0xA05421 ; je 0xA05472
            SS(TextureMipmapLODBias, D3DSAMP_MIPMAPLODBIAS 8, stage);     // 0xA05434
        SS(TextureBorderColour, D3DSAMP_BORDERCOLOR 4, stage); // 0xA05472
        SS(TextureMaxAnisotropy,D3DSAMP_MAXANISOTROPY 0xA, stage); // 0xA054B4
        ST(TextureOperation,      D3DTSS_COLOROP 1,   stage); // 0xA054F6 (value edx)
        ST(TextureAlphaOperation, D3DTSS_ALPHAOP 4,   stage); // 0xA05534
        ST(TextureArgument1,      D3DTSS_COLORARG1 2, stage); // 0xA05576
        ST(TextureArgument2,      D3DTSS_COLORARG2 3, stage); // 0xA055B8
        ST(TextureAlphaArgument1, D3DTSS_ALPHAARG1 5, stage); // 0xA055FA
        ST(TextureAlphaArgument2, D3DTSS_ALPHAARG2 6, stage); // 0xA05638
        ST(TextureCoordIndex,     D3DTSS_TEXCOORDINDEX 0xB, stage); // 0xA0567A
        if (!(InitFlags & 2))                                  // 0xA056BD test byte [esi+3A38h],2 ; jne 0xA0570A
            ST(TextureTransformMode, D3DTSS_TEXTURETRANSFORMFLAGS 0x18, stage); // 0xA056CC
        ST(TextureBumpEnvMat00,   D3DTSS_BUMPENVMAT00 7, stage); // 0xA0570A
        ST(TextureBumpEnvMat10,   D3DTSS_BUMPENVMAT10 9, stage); // 0xA0574C   (NB: 10 before 01)
        ST(TextureBumpEnvMat01,   D3DTSS_BUMPENVMAT01 8, stage); // 0xA0578E
        ST(TextureBumpEnvMat11,   D3DTSS_BUMPENVMAT11 0xA, stage); // 0xA057D0
    }                                                          // 0xA0580E inc ebx ; add ecx,14h ; cmp ebx,8 ; jl 0xA052AF
}                                                              // 0xA05826 epilogue, ret
```
- No cold blocks, no jump tables, no calls other than GetDeviceCaps.
- Loop pointer: ecx = this+0x2E54 = &DeviceState.TextureMipMapMode[0] + 0x10 (DS+0x63C); offsets in the loop are relative to it.
- The InitFlags test in retail is per-iteration (re-read [esi+3A38h]); the caps test re-reads [esp+38h] per iteration.
