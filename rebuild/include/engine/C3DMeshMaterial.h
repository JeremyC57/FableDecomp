#ifndef FABLE_ENGINE_C3DMESHMATERIAL_H
#define FABLE_ENGINE_C3DMESHMATERIAL_H
#include "fable_string.h"
// PDB complete48-byte object. The unnamed first word and IV-counted filenames handle retain their storage.
class C3DMeshMaterial {
public:
#ifdef FABLE_RETAIL_C3DMESH2_LOADBINARY
 // Retail opt-in (Codex pass22): both-PDB exact public LoadBinary signature and native caller.
 void LoadBinary(class CMemoryDataInputStream&,const class CGraphicDataBank*);
#endif
#if defined(FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS) || defined(FABLE_RETAIL_C3DMESHMATERIAL_MAPBANKINDICES)
#ifdef FABLE_RETAIL_MESH_PRELOAD_CALLS_PASS30
    bool IsDegenerateTriangles() const { return DegenerateTriangles; }
#endif
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
#ifdef FABLE_RETAIL_C3DMESHMATERIAL_PARTICLEMESH
    // Retail opt-in (chunk 2059, row 0x00BC25F0): FableWin.pdb ?HasValidDiffuseMap@C3DMeshMaterial@@QBE_NXZ (listed right after
    // CEnginePrimitiveManagerParticleMeshGroup::RenderSubPrimitive: first user) and ?GetSelfIllumination@C3DMeshMaterial@@QBEKXZ (17b/9i),
    // inline in retail (0x00BC2CF4 DiffuseMapBankIndex +0x10 tested, 0x00BC2EC4 SelfIllumination +0x24 read in place).
    bool HasValidDiffuseMap() const { return DiffuseMapBankIndex != 0; }
    unsigned long GetSelfIllumination() const { return SelfIllumination; }
#endif
#ifdef FABLE_RETAIL_C3DMESHMATERIAL_FLAGS
    // Opt-in (bwf1 / agentC2, row 0x00BB3E80): FableWin.pdb inline ?GetSelfIllumination@C3DMeshMaterial@@QBEKXZ, ?IsTwoSided@C3DMeshMaterial@@QBE_NXZ,
    // ?IsTransparent@C3DMeshMaterial@@QBE_NXZ (no out-of-line copy; +0x24 / +0x28 / +0x29 read in place). Members shared with the
    // _PARTICLEMESH / _FLAG_ACCESSORS opt-ins are declared here only without them.
#ifndef FABLE_RETAIL_C3DMESHMATERIAL_PARTICLEMESH
    unsigned long GetSelfIllumination() const{return SelfIllumination;}
#endif
    bool IsTwoSided() const{return TwoSided;}
#ifndef FABLE_RETAIL_C3DMESHMATERIAL_FLAG_ACCESSORS
    bool IsTransparent() const{return Transparent;}
#endif
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
FABLE_STATIC_ASSERT_AT(38, sizeof(C3DMeshMaterial)==0x30);
#endif
