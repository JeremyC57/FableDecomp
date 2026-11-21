#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVEMORPHINFO_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVEMORPHINFO_H
#ifdef FABLE_RETAIL_PASS22_STATIC_MESH_RENDER
// Both original PDB class/field/method records; native parent00BA1140 uses these original owners.
#include "engine/CVectorMap.h"
#include "engine/CEngineTextureDecal.h"
#include "engine/CCompositeSurfaceInfo.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
class C3DMesh2;
class CEngineInternalPrimitiveMorphInfo {
public:
 const unsigned char* GetHiddenMaterials(const C3DMesh2&);
 // Both original PDB signatures and native00BA1490 call00BEC8D0; existing binding agrees.
 void InitialiseHiddenMaterials(const C3DMesh2&);
 CVectorMap<std::pair<long,long>,CEngineTextureDecal> TextureDecals;
 CVectorMap<long,CCompositeSurfaceInfo> CompositeSurfaces;
 std::set<CCharString> HiddenMaterialNames;
 CArray<unsigned char> HiddenMaterials;
};
#else
// CEngineInternalPrimitiveMorphInfo (Ego_r.pdb / FableWin.pdb), moved out of agentC22's engine/CEngineInternalPrimitiveMorphedStaticMesh.h
// unchanged (wfA6b) so the morphed animated-mesh headers can share it without pulling CEngineInternalPrimitiveStaticMesh.h (its
// CEngineRenderInfoStaticMesh.h redefines CEngineRenderInfoMeshBase, which clashes with CEngineRenderInfoMeshBase.h in one TU).
// Layout notes: see that header (retail sizeof 0x38: TextureDecals +0x00, CompositeSurfaces +0x10, names +0x20, array +0x2C).
#include "rebuild_abi.h"
#include "engine/CVectorMap.h"
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEMORPHINFO_GETTEXTUREDECAL
#include "engine/CEngineTextureDecal.h"
#include "fable_compiler.h"
#endif
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEMORPHINFO_HIDDENMATERIALS
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CArray.h"
#include "fable_compiler.h"
class C3DMesh2;
#endif
class CEngineTextureDecal;
class CCompositeSurfaceInfo;
class CEngineInternalPrimitiveMorphInfo {
public:
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEMORPHINFO_GETTEXTUREDECAL
    // Opt-in (wfE7 chunk 4010 / p25S, row 0x00BD3070): FableWin.pdb ?GetTextureDecal@CEngineInternalPrimitiveMorphInfo@@QAEPAVCEngineTextureDecal@@JJ@Z
    // (60i/204b; retail keeps an uncalled copy 0x00BD8800, landed with this body): the decal registered for (original texture index, decal ID),
    // or null. _FORCEINLINE: the expanded form.
    typedef std::pair<long,long> TTextureDecalKey;
    typedef CVectorMap<TTextureDecalKey,CEngineTextureDecal,CKeyPairCompareLess<TTextureDecalKey,CEngineTextureDecal> > TTextureDecalMap;
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEMORPHINFO_GETTEXTUREDECAL_FORCEINLINE
    FABLE_FORCEINLINE
#endif
    CEngineTextureDecal* GetTextureDecal(long original_texture_index, long decal_id)
    {
        TTextureDecalMap::iterator it = TextureDecals.Find(TTextureDecalKey(original_texture_index, decal_id));
        return it != TextureDecals.end() ? &it->second : 0;
    }
#endif
    CVectorMap<std::pair<long,long>,CEngineTextureDecal,CKeyPairCompareLess<std::pair<long,long>,CEngineTextureDecal> > TextureDecals;   // +0x00
    CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> > CompositeSurfaces;                       // +0x10
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEMORPHINFO_HIDDENMATERIALS
    // Retail opt-in (wfA6b, row 0x00BA6240 CEnginePrimitiveManagerStaticMeshesBase::RenderPrimitiveToShadowBuffer): the two members
    // typed (Ego_r.pdb names: set<CCharString> HiddenMaterialNames, CArray<unsigned char> HiddenMaterials; retail offsets +0x20 / +0x2C)
    // and FableWin.pdb ?GetHiddenMaterials@CEngineInternalPrimitiveMorphInfo@@QAEPBEABVC3DMesh2@@@Z (0x02E82240, 28i/76b; no retail
    // out-of-line copy, so FABLE_FORCEINLINE (plain inline is called in that caller); expanded at 0x00BA6518..0x00BA654C: no names -> 0, an empty array is built by
    // ?InitialiseHiddenMaterials@CEngineInternalPrimitiveMorphInfo@@QAEXABVC3DMesh2@@@Z 0x00BEC8D0, then the array's GetAsCArray).
    std::set<CCharString> HiddenMaterialNames;                           // +0x20
    CArray<unsigned char> HiddenMaterials;                               // +0x2c
    void InitialiseHiddenMaterials(const C3DMesh2& mesh);                // 0x00BEC8D0
    FABLE_FORCEINLINE const unsigned char* GetHiddenMaterials(const C3DMesh2& mesh)
    {
        if (HiddenMaterialNames.empty())
            return 0;
        if (HiddenMaterials.empty())
            InitialiseHiddenMaterials(mesh);
        return HiddenMaterials.GetAsCArray();
    }
#else
    unsigned char _opaque_0x20[0x18];                                    // +0x20 HiddenMaterialNames (set), HiddenMaterials (CArray)
#endif
};
#endif
#endif
