// C3DMeshMaterial::C3DMeshMaterial() -- CBaseClass base, this class's vtable, MaterialIndex -1, default Name, every id/index/flag zero, four false flags, null Filenames
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABF7B0, length 80 bytes.
// Callees: 0x0099A2D0 CBaseClass::CBaseClass, 0x0099E4B0 CCharString::CCharString()
// Layout from the PDB (sizeof 0x30, no engine header generated): the intrusive Filenames pointer at +0x2c is
// value-initialised like the plain members.
//
#include "rebuild_abi.h"

struct CCharString {
    CCharString();                                      // 0x0099E4B0 default
    ~CCharString();                                     // 0x0099EAE0
    void* m_pStringData;                                // CCharStringData*
};

struct CBaseClass {
    virtual void v0();
    CBaseClass();                                  // 0x0099A2D0 (vtable store only)
};
struct CIVCountedPointeeBase;
struct C3DMeshMaterial : CBaseClass {
    long                   MaterialIndex;            // +0x04
    CCharString            Name;                     // +0x08
    long                   DecalID;                  // +0x0c
    unsigned long          DiffuseMapBankIndex;      // +0x10
    unsigned long          BumpMapBankIndex;         // +0x14
    unsigned long          ReflectionMapBankIndex;   // +0x18
    unsigned long          IlluminationMapBankIndex; // +0x1c
    unsigned long          MapFlags;                 // +0x20
    unsigned long          SelfIllumination;         // +0x24
    bool                   TwoSided;                 // +0x28
    bool                   Transparent;              // +0x29
    bool                   BooleanAlpha;             // +0x2a
    bool                   DegenerateTriangles;      // +0x2b
    CIVCountedPointeeBase* Filenames;                // +0x2c CIVCountedPointer<C3DMeshMaterial::CFilenames>   sizeof == 0x30
    C3DMeshMaterial();
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshMaterial) == 0x30);
C3DMeshMaterial::C3DMeshMaterial()
    : MaterialIndex(-1), DecalID(0), DiffuseMapBankIndex(0), BumpMapBankIndex(0), ReflectionMapBankIndex(0),
      IlluminationMapBankIndex(0), MapFlags(0), SelfIllumination(0),
      TwoSided(false), Transparent(false), BooleanAlpha(false), DegenerateTriangles(false), Filenames(0)
{
}