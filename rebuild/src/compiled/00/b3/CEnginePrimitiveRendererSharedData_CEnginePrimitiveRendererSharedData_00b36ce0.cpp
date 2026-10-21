// CEnginePrimitiveRendererSharedData::CEnginePrimitiveRendererSharedData() -- construct the shared renderer data: the CBase root, this class's vtable, the current interpolation info and the forty CPixelShader members
// Leaf: ??0CEnginePrimitiveRendererSharedData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B36CE0, length 480 bytes (body 0x1db + int3 padding), plain `ret`.
// Callees: CBase::CBase @ 0x0099A2F0, CEngineInterpolationInfo::CEngineInterpolationInfo @ 0x0042FA30, CPixelShader::CPixelShader
// @ 0x00A5EC00 (catalogued CBaseIntelligentPointer, propagated: called on each 8-byte pixel-shader member). The only caller is
// 0x00B33B65.
// Layout: Ego_r.pdb CEnginePrimitiveRendererSharedData (616 bytes; vtable 0x012A1300): BlockOnLoad +4, CurrentInterpolationInfo +8,
// the scalar settings +0x78..+0xd8 (none of them set here: they have no initialisers), the 40 CPixelShader members +0xdc..+0x214,
// the shader pointers and debug flags +0x21c..+0x264 (also left alone).
// Shape: base ctor, vtable store, then each member sub-object's out-of-line default ctor in declaration order with `lea ecx`.
struct CBase {
    virtual void v00();
    CBase();                                    // 0x0099A2F0
};

struct CEngineInterpolationInfo {
    CEngineInterpolationInfo();                 // 0x0042FA30
    long _body[0x70 / 4];
};

struct CPixelShader {
    CPixelShader();                             // 0x00A5EC00
    void* _vptr;
    void* PShader;
};

struct CEnginePrimitiveRendererSharedData : CBase {
    virtual void v00();
    CEnginePrimitiveRendererSharedData();
    bool  BlockOnLoad;                          // +0x04
    CEngineInterpolationInfo CurrentInterpolationInfo;   // +0x08
    long  CurrentOcclusionLayer;                // +0x78
    float DrawDistanceFactor;                   // +0x7c
    float DrawDistanceMinimum;                  // +0x80
    float DrawDistanceMaximum;                  // +0x84
    float RepeatedMeshDrawDistanceFactor;       // +0x88
    float ZSpriteDrawDistanceFactor;            // +0x8c
    float MinimumZSpriteAsMeshDistance;         // +0x90
    float MaximumZSpriteAsMeshDistance;         // +0x94
    bool  DrawDistanceInvalid;                  // +0x98
    float LODErrorTolerance;                    // +0x9c
    float CharacterLODErrorTolerance;           // +0xa0
    float LODVirtualScreenSize;                 // +0xa4
    float LODVirtualScreenSizeFOVAdjusted;      // +0xa8
    float MipmapQuality;                        // +0xac
    float MipmapSizeMultiplier;                 // +0xb0
    long  PreloadMipmapTolerance;               // +0xb4
    long  PreloadMipmapMaxSize;                 // +0xb8
    bool  BumpMappingEnable;                    // +0xbc
    bool  OcclusionEnable;                      // +0xbd
    bool  OcclusionHierarchyEnable;             // +0xbe
    bool  OcclusionHierarchyEnableConsole;      // +0xbf
    unsigned long GlobalPrimitiveInitFlags;     // +0xc0
    unsigned long DynamicVBUsage;               // +0xc4
    long  DynamicVBPool;                        // +0xc8
    unsigned long DynamicVBLockFlags;           // +0xcc
    unsigned long DynamicIBUsage;               // +0xd0
    long  DynamicIBPool;                        // +0xd4
    unsigned long DynamicIBLockFlags;           // +0xd8
    CPixelShader PixelShaderDiffuseOnly;                                    // +0xdc
    CPixelShader PixelShaderDiffuseColourTextureDiffuseAlpha;              // +0xe4
    CPixelShader PixelShaderTextureOnly;                                    // +0xec
    CPixelShader PixelShaderDiffuse;                                        // +0xf4
    CPixelShader PixelShaderDiffuseFog;                                     // +0xfc
    CPixelShader PixelShaderDiffuseEnvFog;                                  // +0x104
    CPixelShader PixelShaderDiffuseEnvBumpFog;                              // +0x10c
    CPixelShader PixelShaderDiffuseDP3BumpFog;                              // +0x114
    CPixelShader PixelShaderDiffuseFogDepthBufferShadow;                    // +0x11c
    CPixelShader PixelShaderDiffuseFogDepthBufferShadowOutputFactor;        // +0x124
    CPixelShader PixelShaderDiffuseEnvFogDepthBufferShadow;                 // +0x12c
    CPixelShader PixelShaderDiffuseEnvBumpFogDepthBufferShadow;             // +0x134
    CPixelShader PixelShaderDiffuseDP3BumpFogDepthBufferShadow;             // +0x13c
    CPixelShader PixelShaderDiffuseDP3BumpFogDepthBufferShadowOutputFactor; // +0x144
    CPixelShader PixelShaderDepthShadowBufferRender;                        // +0x14c
    CPixelShader PixelShaderDepthShadowBufferRenderAlpha;                   // +0x154
    CPixelShader PixelShaderColourShadowBufferRender;                       // +0x15c
    CPixelShader PixelShaderColourShadowBufferRenderAlpha;                  // +0x164
    CPixelShader PixelShaderDiffuseFogColourBufferShadow;                   // +0x16c
    CPixelShader PixelShaderDiffuseFogColourBufferShadowOutputFactor;       // +0x174
    CPixelShader PixelShaderDiffuseEnvFogColourBufferShadow;                // +0x17c
    CPixelShader PixelShaderDiffuseEnvBumpFogColourBufferShadow;            // +0x184
    CPixelShader PixelShaderDiffuseDP3BumpFogColourBufferShadow;            // +0x18c
    CPixelShader PixelShaderDiffuseDP3BumpFogColourBufferShadowOutputFactor; // +0x194
    CPixelShader PixelShader2DDiffuseOnly;                                  // +0x19c
    CPixelShader PixelShader2DTextureOnly;                                  // +0x1a4
    CPixelShader PixelShader2DDiffuse;                                      // +0x1ac
    CPixelShader PixelShader2DDiffuseEnv;                                   // +0x1b4
    CPixelShader PixelShader2DDiffuseEnvBump;                               // +0x1bc
    CPixelShader PixelShader2DDiffuseDP3Bump;                               // +0x1c4
    CPixelShader PixelShader2DDiffuseOnlyAdditiveAlpha;                     // +0x1cc
    CPixelShader PixelShader2DTextureOnlyAdditiveAlpha;                     // +0x1d4
    CPixelShader PixelShader2DDiffuseAdditiveAlpha;                         // +0x1dc
    CPixelShader PixelShader2DDiffuseEnvAdditiveAlpha;                      // +0x1e4
    CPixelShader PixelShader2DDiffuseEnvBumpAdditiveAlpha;                  // +0x1ec
    CPixelShader PixelShader2DDiffuseDP3BumpAdditiveAlpha;                  // +0x1f4
    CPixelShader PixelShaderOutlineSmoothFill;                              // +0x1fc
    CPixelShader PixelShaderOutlineSmoothFillAlpha;                         // +0x204
    CPixelShader PixelShaderOutlineSmoothFillDP3;                           // +0x20c
    CPixelShader PixelShaderOutlineSmoothFillDP3Alpha;                      // +0x214
    CPixelShader* PixelShaderShadowBufferRender;                            // +0x21c
    unsigned char _rest[0x268 - 0x220];         // +0x220 the remaining shader pointers and debug flags
};

CEnginePrimitiveRendererSharedData::CEnginePrimitiveRendererSharedData()
{
}