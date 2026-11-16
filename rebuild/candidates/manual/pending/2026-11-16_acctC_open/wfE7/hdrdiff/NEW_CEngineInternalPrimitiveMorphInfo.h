#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVEMORPHINFO_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVEMORPHINFO_H
// New header (chunk 4010, row 0x00BD3070 CEnginePrimitiveManagerAnimatedMeshesBase::RenderMeshPrimitive; the same class model as chunk
// 2060's engine/CEngineInternalPrimitiveMorphedStaticMesh.h). Ego_r.pdb CEngineInternalPrimitiveMorphInfo: TextureDecals
// (CVectorMap<pair<long,long>,CEngineTextureDecal>) +0x00, CompositeSurfaces (CVectorMap<long,CCompositeSurfaceInfo>) +0x10 in retail
// (Ego_r 20-byte CVectorMap -> retail 16: 0x00BD31CC `lea ecx,[esi+10h]`), then HiddenMaterialNames (STLport set) +0x20 and
// HiddenMaterials (CArray<unsigned char>) +0x2C (opaque here; retail sizeof 0x38). FableWin.pdb
// ?GetTextureDecal@CEngineInternalPrimitiveMorphInfo@@QAEPAVCEngineTextureDecal@@JJ@Z (60i/204b; retail keeps an uncalled copy
// 0x00BD8800, landed with this body): the decal registered for (original texture index, decal ID), or null.
#include "engine/CVectorMap.h"
#include "engine/CEngineTextureDecal.h"
class CCompositeSurfaceInfo;
class CEngineInternalPrimitiveMorphInfo {
public:
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
    TTextureDecalMap TextureDecals;                                                                                                   // +0x00
    CVectorMap<long,CCompositeSurfaceInfo,CKeyPairCompareLess<long,CCompositeSurfaceInfo> > CompositeSurfaces;                       // +0x10
    unsigned char _opaque_0x20[0x18];                                    // +0x20 HiddenMaterialNames (set), HiddenMaterials (CArray)
};
#endif
