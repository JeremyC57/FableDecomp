#ifndef FABLE_ENGINE_C3DMESHMATERIAL_H
#define FABLE_ENGINE_C3DMESHMATERIAL_H
#include "fable_string.h"
// PDB complete48-byte object. The unnamed first word and IV-counted filenames handle retain their storage.
class C3DMeshMaterial {
public:
#ifdef FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
    unsigned long GetDiffuseMapBankIndex() const{return DiffuseMapBankIndex;}
    unsigned long GetBumpMapBankIndex() const{return BumpMapBankIndex;}
#endif
    unsigned char UnmodelledBase[4];long MaterialIndex;CCharString Name;long DecalID;
    unsigned long DiffuseMapBankIndex,BumpMapBankIndex,ReflectionMapBankIndex,IlluminationMapBankIndex,MapFlags,SelfIllumination;
    bool TwoSided,Transparent,BooleanAlpha,DegenerateTriangles;
    unsigned char Filenames[4]; // CIVCountedPointer<CFilenames>, one word.
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshMaterial)==0x30);
#endif
