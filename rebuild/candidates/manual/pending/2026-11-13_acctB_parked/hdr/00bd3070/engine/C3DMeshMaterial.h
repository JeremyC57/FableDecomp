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
#ifdef FABLE_RETAIL_C3DMESHMATERIAL_GETDECALID
    // Scout 4010 (row 0x00BD3070): FableWin.pdb ?GetDecalID@C3DMeshMaterial@@QBEJXZ (17b/9i, FableWin 0x02E8E830); +0x0C.
    long GetDecalID() const{return DecalID;}
#endif
#ifdef FABLE_RETAIL_C3DMESHMATERIAL_SELFILLUM_TRANSPARENT
    // Scout 4010: FableWin.pdb ?GetSelfIllumination@C3DMeshMaterial@@QBEKXZ / ?IsTransparent@C3DMeshMaterial@@QBE_NXZ (17b/9i each).
    unsigned long GetSelfIllumination() const{return SelfIllumination;}
    bool IsTransparent() const{return Transparent;}
#endif
    unsigned char UnmodelledBase[4];long MaterialIndex;CCharString Name;long DecalID;
    unsigned long DiffuseMapBankIndex,BumpMapBankIndex,ReflectionMapBankIndex,IlluminationMapBankIndex,MapFlags,SelfIllumination;
    bool TwoSided,Transparent,BooleanAlpha,DegenerateTriangles;
    unsigned char Filenames[4]; // CIVCountedPointer<CFilenames>, one word.
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshMaterial)==0x30);
#endif
