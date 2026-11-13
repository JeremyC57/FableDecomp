#ifndef FABLE_ENGINE_C3DMESHMATERIAL_H
#define FABLE_ENGINE_C3DMESHMATERIAL_H
#include "fable_string.h"
// PDB complete48-byte object. The unnamed first word and IV-counted filenames handle retain their storage.
class C3DMeshMaterial {
public:
#if defined(FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS) || defined(FABLE_RETAIL_C3DMESHMATERIAL_MAPBANKINDICES)
    unsigned long GetDiffuseMapBankIndex() const{return DiffuseMapBankIndex;}
    unsigned long GetBumpMapBankIndex() const{return BumpMapBankIndex;}
#endif
#ifdef FABLE_RETAIL_C3DMESHMATERIAL_MAPBANKINDICES
    // Retail opt-in (chunk 1021, row 0x00BEC680 CEngineInternalPrimitiveMeshBase::CollectStats): the FableWin.pdb inline accessors
    // ?GetReflectionMapBankIndex@C3DMeshMaterial@@QBEKXZ / ?GetIlluminationMapBankIndex@C3DMeshMaterial@@QBEKXZ (no out-of-line copy
    // in Ego_r.pdb or Fable.exe; +0x18 / +0x1C read in place). The diffuse / bump pair above is shared with the static-block opt-in.
    unsigned long GetReflectionMapBankIndex() const{return ReflectionMapBankIndex;}
    unsigned long GetIlluminationMapBankIndex() const{return IlluminationMapBankIndex;}
#endif
#ifdef FABLE_RETAIL_C3DMESHMATERIAL_FLAG_ACCESSORS
    // Retail opt-in (chunk 3013, row 0x00B9A4A0 CEnginePrimitiveManagerRepeatedStaticMeshes::RenderPrimitive): the FableWin.pdb inline
    // accessors ?IsTransparent@C3DMeshMaterial@@QBE_NXZ / ?IsBooleanAlpha@C3DMeshMaterial@@QBE_NXZ / ?GetMapFlags@C3DMeshMaterial@@QBEKXZ
    // (no out-of-line copy in Fable.exe; +0x29 / +0x2A / +0x20 read in place).
    bool IsTransparent() const{return Transparent;}
    bool IsBooleanAlpha() const{return BooleanAlpha;}
    unsigned long GetMapFlags() const{return MapFlags;}
#endif
    unsigned char UnmodelledBase[4];long MaterialIndex;CCharString Name;long DecalID;
    unsigned long DiffuseMapBankIndex,BumpMapBankIndex,ReflectionMapBankIndex,IlluminationMapBankIndex,MapFlags,SelfIllumination;
    bool TwoSided,Transparent,BooleanAlpha,DegenerateTriangles;
    unsigned char Filenames[4]; // CIVCountedPointer<CFilenames>, one word.
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshMaterial)==0x30);
#endif
