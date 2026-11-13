#ifndef FABLE_ENGINE_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_H
#define FABLE_ENGINE_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_H
#ifdef FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#include "engine/CEnginePrimitiveManagerMeshBase.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
class C3DPrimitive2;class C3DMesh2;class C3DTextureStages;class CGraphicDataBank;
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_SILHOUETTE
class CEngineInternalPrimitiveStaticMesh;
enum ERenderMode;                                                        // MS forward enum (Ego_r.pdb; no enumerator name recovered)
#endif
class CEnginePrimitiveManagerStaticMeshesBase : public CEnginePrimitiveManagerMeshBase {
public:
    virtual CEngineInternalPrimitiveBase* CreateInternalPrimitive(EEnginePrimitiveType);
    virtual void GetSupportedTypes(CEnginePrimitiveTypeList&) const;
    virtual void RenderSubPrimitive(CEngineSubPrimitiveBase&,ERenderPassType);
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase&,EEngineRenderLayer,CEngineSubPrimitiveChain&);
    virtual ERendered RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase&);
    virtual bool PreloadPrimitive(CEngineInternalPrimitiveBase&,const C3DVector&,unsigned long);
    virtual void FrameStart();
private:
    virtual void AddStaticShadowData(CEngineInternalPrimitiveBase&,NEngineShadowRenderer::CShadowSceneContext&);
public:
    virtual ~CEnginePrimitiveManagerStaticMeshesBase();
public:
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH
    CEnginePrimitiveHandle AddStaticMesh(const C3DMeshInfo*,const C3DMeshInfo*,const CMatrix3x4&,float,const CEngineFadeDistance&,long,const CRGBColour&,long,float,bool,unsigned long);
#endif
    CEnginePrimitiveManagerStaticMeshesBase(const CEnginePrimitiveManagerStaticMeshesBase&);
    CEnginePrimitiveManagerStaticMeshesBase(CArray<CEngineScene *>&);
    static bool FABLE_FASTCALL SetupStaticBlocksNoMorph(C3DPrimitive2&,const C3DMesh2&,C3DTextureStages&,long,const CGraphicDataBank*,bool);
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_SILHOUETTE
    // Opt-in (chunk 2032, row 0x00BA6FF0): Ego_r.pdb / FableWin.pdb private members
    // ?DrawImmediately@CEnginePrimitiveManagerStaticMeshesBase@@AAEXAAVCEngineInternalPrimitiveStaticMesh@@W4ERenderMode@@W4ERenderPassType@@@Z
    // (landed 0x00BA6DC0) and ?DrawSilhouette@CEnginePrimitiveManagerStaticMeshesBase@@AAEXAAVCEngineInternalPrimitiveStaticMesh@@@Z
    // (0x00BA6FF0, the next function in the TU). Declarations only.
private:
    void DrawImmediately(CEngineInternalPrimitiveStaticMesh& primitive, ERenderMode mode, ERenderPassType pass);   // 0x00BA6DC0
    void DrawSilhouette(CEngineInternalPrimitiveStaticMesh& primitive);                                           // 0x00BA6FF0
#endif
private:
    unsigned long PointLightShaderIndex[6];
    CVertexShader VertexShadersStaticSpotlight[4];
    CVertexShader VertexShadersStaticStippleAlpha[4];
    CVertexShader VertexShadersStatic[4];
    CVertexShader VertexShadersStaticEnv[4];
    CVertexShader VertexShadersStaticEnvBump[4];
    CVertexShader VertexShadersStaticDepthBufferShadow[4];
    CVertexShader VertexShadersStaticEnvDepthBufferShadow[4];
    CVertexShader VertexShadersStaticEnvBumpDepthBufferShadow[4];
    CVertexShader VertexShadersStaticDP3BumpDepthBufferShadow[4];
    CVertexShader VertexShadersStaticSpotDepthBufferShadow[4];
    CVertexShader VertexShadersStaticEnvSpotDepthBufferShadow[4];
    CVertexShader VertexShadersStaticEnvBumpSpotDepthBufferShadow[4];
    CVertexShader VertexShadersStaticDP3BumpSpotDepthBufferShadow[4];
    CVertexShader VertexShaders2DStatic;
    CVertexShader VertexShaders2DStaticEnv;
    CVertexShader VertexShaders2DStaticEnvBump;
    CVertexShader VertexShaders2DStaticDP3Bump;
    CVertexShader VertexShaderDepthShadowBufferRender;
    CVertexShader VertexShaderColourShadowBufferRender;
    CVertexShader VertexShadersStaticColourBufferShadow[4];
    CVertexShader VertexShadersStaticEnvColourBufferShadow[4];
    CVertexShader VertexShadersStaticEnvBumpColourBufferShadow[4];
    CVertexShader VertexShadersStaticDP3BumpColourBufferShadow[4];
    CVertexShader VertexShadersStaticSpotColourBufferShadow[4];
    CVertexShader VertexShadersStaticEnvSpotColourBufferShadow[4];
    CPixelShader PixelShaderDiffuseFogStippleAlpha;
    CVertexShader VertexShaderOutline;
    CVertexShader VertexShaderDP3BumpOutline;
    CVertexShader VertexShaderOutlineSmoothFill;
    CVertexShader VertexShaderDP3BumpOutlineSmoothFill;
    CVertexShader* VertexShadersStaticShadow;
    CVertexShader* VertexShadersStaticEnvShadow;
    CVertexShader* VertexShadersStaticEnvBumpShadow;
    CVertexShader* VertexShadersStaticDP3BumpShadow;
    CVertexShader* VertexShadersStaticSpotShadow;
    CVertexShader* VertexShadersStaticEnvSpotShadow;
    CVertexShader* VertexShaderShadowBufferRender;
    long OverridePrimitive;
    long RenderOnlyNMeshes;
    long StippleAlphaSolidFactor;
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveManagerStaticMeshesBase)==0x318);

#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEnginePrimitiveManagerStaticMeshesBase = 0x318.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CVertexShader;
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_DISPLAYMIRRORED
class CTexture;
class C3DPrimitive2;
class CEngineSubPrimitiveStaticMesh;
enum ERenderPassType;                                                    // MS forward enum (engine/CEnginePrimitiveManager.h)
#endif

#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_STATICBLOCKS
class C3DPrimitive2;
class C3DMesh2;
class C3DTextureStages;
class CGraphicDataBank;
#endif

#pragma pack(push, 1)
struct CEnginePrimitiveManagerStaticMeshesBase {
    void*          __vftable;                                // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char  _pad_0x04[0x4];                           // +0x04
    unsigned char  _pad_0x08[0x4];                           // +0x08
    unsigned char  _pad_0x0c[0x4];                           // +0x0c
    unsigned char  _pad_0x10[0x4];                           // +0x10
    unsigned char  _pad_0x14[0x4];                           // +0x14
    unsigned char  _pad_0x18[0x4];                           // +0x18
    unsigned char  _pad_0x1c[0x4];                           // +0x1c
    unsigned long  PointLightShaderIndex[6];                 // +0x20
    unsigned char  VertexShadersStaticSpotlight[0x20];       // +0x38 CVertexShader[4]
    unsigned char  VertexShadersStaticStippleAlpha[0x20];    // +0x58 CVertexShader[4]
    unsigned char  VertexShadersStatic[0x20];                // +0x78 CVertexShader[4]
    unsigned char  VertexShadersStaticEnv[0x20];             // +0x98 CVertexShader[4]
    unsigned char  VertexShadersStaticEnvBump[0x20];         // +0xb8 CVertexShader[4]
    unsigned char  VertexShadersStaticDepthBufferShadow[0x20]; // +0xd8 CVertexShader[4]
    unsigned char  VertexShadersStaticEnvDepthBufferShadow[0x20]; // +0xf8 CVertexShader[4]
    unsigned char  VertexShadersStaticEnvBumpDepthBufferShadow[0x20]; // +0x118 CVertexShader[4]
    unsigned char  VertexShadersStaticDP3BumpDepthBufferShadow[0x20]; // +0x138 CVertexShader[4]
    unsigned char  VertexShadersStaticSpotDepthBufferShadow[0x20]; // +0x158 CVertexShader[4]
    unsigned char  VertexShadersStaticEnvSpotDepthBufferShadow[0x20]; // +0x178 CVertexShader[4]
    unsigned char  VertexShadersStaticEnvBumpSpotDepthBufferShadow[0x20]; // +0x198 CVertexShader[4]
    unsigned char  VertexShadersStaticDP3BumpSpotDepthBufferShadow[0x20]; // +0x1b8 CVertexShader[4]
    unsigned char  VertexShaders2DStatic[0x8];               // +0x1d8 CVertexShader
    unsigned char  VertexShaders2DStaticEnv[0x8];            // +0x1e0 CVertexShader
    unsigned char  VertexShaders2DStaticEnvBump[0x8];        // +0x1e8 CVertexShader
    unsigned char  VertexShaders2DStaticDP3Bump[0x8];        // +0x1f0 CVertexShader
    unsigned char  VertexShaderDepthShadowBufferRender[0x8]; // +0x1f8 CVertexShader
    unsigned char  VertexShaderColourShadowBufferRender[0x8]; // +0x200 CVertexShader
    unsigned char  VertexShadersStaticColourBufferShadow[0x20]; // +0x208 CVertexShader[4]
    unsigned char  VertexShadersStaticEnvColourBufferShadow[0x20]; // +0x228 CVertexShader[4]
    unsigned char  VertexShadersStaticEnvBumpColourBufferShadow[0x20]; // +0x248 CVertexShader[4]
    unsigned char  VertexShadersStaticDP3BumpColourBufferShadow[0x20]; // +0x268 CVertexShader[4]
    unsigned char  VertexShadersStaticSpotColourBufferShadow[0x20]; // +0x288 CVertexShader[4]
    unsigned char  VertexShadersStaticEnvSpotColourBufferShadow[0x20]; // +0x2a8 CVertexShader[4]
    unsigned char  PixelShaderDiffuseFogStippleAlpha[0x8];   // +0x2c8 CPixelShader
    unsigned char  VertexShaderOutline[0x8];                 // +0x2d0 CVertexShader
    unsigned char  VertexShaderDP3BumpOutline[0x8];          // +0x2d8 CVertexShader
    unsigned char  VertexShaderOutlineSmoothFill[0x8];       // +0x2e0 CVertexShader
    unsigned char  VertexShaderDP3BumpOutlineSmoothFill[0x8]; // +0x2e8 CVertexShader
    CVertexShader* VertexShadersStaticShadow;                // +0x2f0
    CVertexShader* VertexShadersStaticEnvShadow;             // +0x2f4
    CVertexShader* VertexShadersStaticEnvBumpShadow;         // +0x2f8
    CVertexShader* VertexShadersStaticDP3BumpShadow;         // +0x2fc
    CVertexShader* VertexShadersStaticSpotShadow;            // +0x300
    CVertexShader* VertexShadersStaticEnvSpotShadow;         // +0x304
    CVertexShader* VertexShaderShadowBufferRender;           // +0x308
    long           OverridePrimitive;                        // +0x30c
    long           RenderOnlyNMeshes;                        // +0x310
    long           StippleAlphaSolidFactor;                  // +0x314
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_DISPLAYMIRRORED
    // Opt-in (chunk 851, row 0x00BA1B00): Ego_r.pdb / FableWin.pdb
    // ?DisplayMirroredTextures@CEnginePrimitiveManagerStaticMeshesBase@@AAEXPAPBVCTexture@@KPAVC3DPrimitive2@@W4ERenderPassType@@AAVCEngineSubPrimitiveStaticMesh@@K@Z
    // (private, `ret 18h`). Declaration only; the flattened layout above is unchanged.
private:
    void DisplayMirroredTextures(const CTexture** textures, unsigned long textureCount, C3DPrimitive2* primitive, ERenderPassType pass,
                                 CEngineSubPrimitiveStaticMesh& subPrimitive, unsigned long renderFlags);   // 0x00BA1B00
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_STATICBLOCKS
    // Retail opt-in (Codex 2026-11-09): exact Ego_r/FableWin static signatures; no layout changes.
public:
    static bool FABLE_FASTCALL SetupStaticBlocksNoMorph(C3DPrimitive2& primitive, const C3DMesh2& mesh, C3DTextureStages& stages,
        long level, const CGraphicDataBank* bank, bool load);
#endif
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveManagerStaticMeshesBase) == 0x318);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, PointLightShaderIndex) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticSpotlight) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticStippleAlpha) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStatic) == 0x78);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticEnv) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticEnvBump) == 0xb8);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticDepthBufferShadow) == 0xd8);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticEnvDepthBufferShadow) == 0xf8);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticEnvBumpDepthBufferShadow) == 0x118);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticDP3BumpDepthBufferShadow) == 0x138);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticSpotDepthBufferShadow) == 0x158);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticEnvSpotDepthBufferShadow) == 0x178);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticEnvBumpSpotDepthBufferShadow) == 0x198);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticDP3BumpSpotDepthBufferShadow) == 0x1b8);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShaders2DStatic) == 0x1d8);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShaders2DStaticEnv) == 0x1e0);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShaders2DStaticEnvBump) == 0x1e8);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShaders2DStaticDP3Bump) == 0x1f0);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShaderDepthShadowBufferRender) == 0x1f8);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShaderColourShadowBufferRender) == 0x200);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticColourBufferShadow) == 0x208);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticEnvColourBufferShadow) == 0x228);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticEnvBumpColourBufferShadow) == 0x248);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticDP3BumpColourBufferShadow) == 0x268);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticSpotColourBufferShadow) == 0x288);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticEnvSpotColourBufferShadow) == 0x2a8);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, PixelShaderDiffuseFogStippleAlpha) == 0x2c8);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShaderOutline) == 0x2d0);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShaderDP3BumpOutline) == 0x2d8);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShaderOutlineSmoothFill) == 0x2e0);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShaderDP3BumpOutlineSmoothFill) == 0x2e8);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticShadow) == 0x2f0);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticEnvShadow) == 0x2f4);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticEnvBumpShadow) == 0x2f8);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticDP3BumpShadow) == 0x2fc);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticSpotShadow) == 0x300);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShadersStaticEnvSpotShadow) == 0x304);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, VertexShaderShadowBufferRender) == 0x308);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, OverridePrimitive) == 0x30c);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, RenderOnlyNMeshes) == 0x310);
FABLE_STATIC_ASSERT(offsetof(CEnginePrimitiveManagerStaticMeshesBase, StippleAlphaSolidFactor) == 0x314);

#endif
#endif // FABLE_ENGINE_CENGINEPRIMITIVEMANAGERSTATICMESHESBASE_H
