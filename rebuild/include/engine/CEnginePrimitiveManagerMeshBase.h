#ifndef FABLE_ENGINE_CENGINEPRIMITIVEMANAGERMESHBASE_H
#define FABLE_ENGINE_CENGINEPRIMITIVEMANAGERMESHBASE_H
#ifdef FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#include "engine/CEnginePrimitiveManagerOcclusionBase.h"
class CTexture;class CGraphicDataBank;class C3DMeshMaterial;class CCompositeSurfaceInfo;
#if defined(FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_GETAPPROPRIATEMIPMAP) || defined(FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_ANIMATEDMESH)
class C3DVector;class CMatrix3x4;class C3DPrimitive2;class CEngineTextureDecal;template<class T> class CArray;
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_GETAPPROPRIATEMIPMAP
class C3DVector;class CMatrix3x4;class C3DPrimitive2;
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_APPROPRIATELOD
class C3DMeshInfo;
#endif
template<class K,class V> class CKeyPairCompareLess;
template<class K,class V,class C> class CVectorMap;
class CEnginePrimitiveManagerMeshBase : public CEnginePrimitiveManagerOcclusionBase {
public:
    virtual ~CEnginePrimitiveManagerMeshBase();
#ifdef FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
// Original Ego_r/FableWin TPI and module916 call sites; private evidence in00ba115c.
 static ERenderMode FABLE_FASTCALL GetAppropriateRenderMode(const C3DMeshMaterial*,unsigned long,unsigned long);
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_RENDERMODE2D
    // Retail opt-in (agentNF77, row 0x00B9E950): FableWin.pdb (0x02E85190; no Ego_r.pdb copy)
    // ?GetAppropriateRenderMode2D@CEnginePrimitiveManagerMeshBase@@SI?AW4ERenderMode@@PBVC3DMeshMaterial@@KK@Z (public static fastcall; no
    // retail out-of-line copy: Render2DPrimitive tests the material's map flags +0x20 in place, 0x00B9EBDA). Declaration only (the row gives
    // the inline body, as 0x00BA1140 does for GetAppropriateRenderMode). Needs C3DMeshMaterial / ERenderMode declared.
    static ERenderMode FABLE_FASTCALL GetAppropriateRenderMode2D(const C3DMeshMaterial* material, unsigned long modifier_flags, unsigned long subprim_render_flags);
#endif

public:
    CEnginePrimitiveManagerMeshBase(const CEnginePrimitiveManagerMeshBase&);
    CEnginePrimitiveManagerMeshBase(CArray<CEngineScene *>&);
    static const CTexture* FABLE_FASTCALL GetDiffuseTexture(const C3DMeshMaterial*,CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >*,unsigned long,bool&,const CGraphicDataBank*,bool);
    static const CTexture* FABLE_FASTCALL GetBumpTexture(const C3DMeshMaterial*,CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >*,unsigned long,bool&,const CGraphicDataBank*,bool);
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_ANIMATEDMESH
    // Opt-in (chunk 4010, row 0x00BD3070): FableWin.pdb static fastcall texture getters, out of line in retail:
    // ?GetReflectionTexture@CEnginePrimitiveManagerMeshBase@@SIPBVCTexture@@PBVC3DMeshMaterial@@KAA_NPBVCGraphicDataBank@@_N@Z 0x00BA9830,
    // ?Get2DAlphaMaskTexture@CEnginePrimitiveManagerMeshBase@@SIPBVCTexture@@KKAA_NPBVCGraphicDataBank@@_N@Z 0x00BA9A70,
    // ?GetOverlaidTextures@CEnginePrimitiveManagerMeshBase@@KI_NABVCEngineTextureDecal@@AAV?$CArray@PBVCTexture@@@@KPBVCGraphicDataBank@@@Z 0x00BEC950 (protected).
    static const CTexture* FABLE_FASTCALL GetReflectionTexture(const C3DMeshMaterial* material,unsigned long level,bool& failed,const CGraphicDataBank* bank,bool load);
    static const CTexture* FABLE_FASTCALL Get2DAlphaMaskTexture(unsigned long index,unsigned long level,bool& failed,const CGraphicDataBank* bank,bool load);
protected:
    static bool FABLE_FASTCALL GetOverlaidTextures(const CEngineTextureDecal& decal,CArray<const CTexture*>& textures,unsigned long level,const CGraphicDataBank* bank);
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_2DALPHAMASK
    // Opt-in (agentC6, row 0x00BB30A0 Render2DClothPrimitive): Ego_r.pdb 0x00A60640 / FableWin.pdb
    // ?Get2DAlphaMaskTexture@CEnginePrimitiveManagerMeshBase@@SIPBVCTexture@@KKAA_NPBVCGraphicDataBank@@_N@Z, out of line at 0x00BA9A70
    // (call 0x00BB3C25: bank index in ecx, mipmap in edx, then &preloading, the bank, the force flag). Declaration only.
    static const CTexture* FABLE_FASTCALL Get2DAlphaMaskTexture(unsigned long bank_index, unsigned long mipmap, bool& preloading,
        const CGraphicDataBank* bank, bool force);
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_REFLECTIONTEXTURE
    // Opt-in (bwf2 / agentC16, row 0x00BA2350): Ego_r.pdb / FableWin.pdb
    // ?GetReflectionTexture@CEnginePrimitiveManagerMeshBase@@SIPBVCTexture@@PBVC3DMeshMaterial@@KAA_NPBVCGraphicDataBank@@_N@Z, out of line
    // at 0x00BA9830. Declaration only (Get2DAlphaMaskTexture is under _2DALPHAMASK).
    static const CTexture* FABLE_FASTCALL GetReflectionTexture(const C3DMeshMaterial* material, unsigned long mipmap_level, bool& used_blank,
        const CGraphicDataBank* bank, bool block_on_load);
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_GETAPPROPRIATEMIPMAP
    // Opt-in (chunk 2060, row 0x00BA5260; agentC2 row 0x00BB3E80): FableWin.pdb
    // ?GetAppropriateMipmap@CEnginePrimitiveManagerMeshBase@@SIKABVC3DVector@@ABVCMatrix3x4@@ABVC3DPrimitive2@@MPBVC3DMeshMaterial@@PBVCGraphicDataBank@@@Z
    // (public static __fastcall; no Ego_r.pdb or retail out-of-line copy: expanded in place). The including row defines the body.
#ifdef FABLE_RETAIL_MESH_PRELOAD_CALLS_PASS30
    // FableWin and Ego_r agree on these public static overloads.
    static void FABLE_FASTCALL PreloadDiffuseTexture(const C3DMeshMaterial*, class CEngineInternalPrimitiveMorphInfo*, unsigned long, bool&, const CGraphicDataBank*);
    static void FABLE_FASTCALL PreloadBumpTexture(const C3DMeshMaterial*, class CEngineInternalPrimitiveMorphInfo*, unsigned long, bool&, const CGraphicDataBank*);
    static void FABLE_FASTCALL PreloadReflectionTexture(const C3DMeshMaterial*, unsigned long, bool&, const CGraphicDataBank*);
#endif
    static unsigned long FABLE_FASTCALL GetAppropriateMipmap(const C3DVector& camera_position, const CMatrix3x4& transform,
        const C3DPrimitive2& mesh_primitive, float scale, const C3DMeshMaterial* material, const CGraphicDataBank* graphic_bank);
#endif
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_APPROPRIATELOD
    // Opt-in (chunk 2032, row 0x00BA6FF0): FableWin.pdb ?GetAppropriateLOD@CEnginePrimitiveManagerMeshBase@@SIKPBVC3DMeshInfo@@MMM@Z
    // (static fastcall; no Ego_r.pdb or Fable.exe copy: expanded in place). The including row defines the body.
    static unsigned long FABLE_FASTCALL GetAppropriateLOD(const C3DMeshInfo* mesh_info,float distance,float radius,float tolerance);   // parameter names descriptive
#endif
private:
    static const CTexture* FABLE_FASTCALL GetComposedTexture(long&,CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >&,unsigned long,bool,const CGraphicDataBank*);
};
FABLE_STATIC_ASSERT_AT(27, sizeof(CEnginePrimitiveManagerMeshBase)==0x20);

#else
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEnginePrimitiveManagerMeshBase = 0x20.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_STATICBLOCKS
class CTexture;
class CGraphicDataBank;
class C3DMeshMaterial;
class CCompositeSurfaceInfo;
template<class K,class V> class CKeyPairCompareLess;
template<class K,class V,class C> class CVectorMap;
#endif

#pragma pack(push, 1)
struct CEnginePrimitiveManagerMeshBase {
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _base_0x04[0x1c];                          // +0x04 flattened base-class subobject(s)
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEMANAGERMESHBASE_STATICBLOCKS
    // Retail opt-in (Codex 2026-11-09): exact Ego_r/FableWin static signatures; no layout changes.
public:
    static const CTexture* FABLE_FASTCALL GetDiffuseTexture(const C3DMeshMaterial* material,
        CVectorMap<long, CCompositeSurfaceInfo, CKeyPairCompareLess<long, CCompositeSurfaceInfo> >* surfaces, unsigned long level,
        bool& failed, const CGraphicDataBank* bank, bool load);                      // 0x00BA9360
    static const CTexture* FABLE_FASTCALL GetBumpTexture(const C3DMeshMaterial* material,
        CVectorMap<long, CCompositeSurfaceInfo, CKeyPairCompareLess<long, CCompositeSurfaceInfo> >* surfaces, unsigned long level,
        bool& failed, const CGraphicDataBank* bank, bool load);                      // 0x00BA95D0
#endif
#ifdef FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
private:
    static const CTexture* FABLE_FASTCALL GetComposedTexture(long&,CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> >&,unsigned long,bool,const CGraphicDataBank*);
#endif
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(68, sizeof(CEnginePrimitiveManagerMeshBase) == 0x20);
FABLE_STATIC_ASSERT_AT(69, offsetof(CEnginePrimitiveManagerMeshBase, __vftable) == 0x0);

#endif
#endif // FABLE_ENGINE_CENGINEPRIMITIVEMANAGERMESHBASE_H
