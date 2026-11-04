// C3DMeshMaterial::~C3DMeshMaterial() -- mesh material dtor (empty user body): release the shared texture-filename block, ~Name, then ~CBaseClass
// Leaf: ??1C3DMeshMaterial@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ABF800.
// Identity: Ego_r.pdb ??1C3DMeshMaterial@@UAE@XZ; nested CFilenames at +0x2c (CIVCountedPointer), Name CCharString.
// Shape: empty user body; member destructors in reverse order, then ~CBaseClass.
#include "rebuild_abi.h"
#include "engine/CBaseClass.h"
#include "fable_string.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CIVCountedPointer.h"

class C3DMeshMaterial : public CBaseClass {                  // vtable 0x0129EDC0
public:
    class CFilenames : public CIVCountedPointeeBase { };     // RTTI C3DMeshMaterial::CFilenames : CIVCountedPointeeBase
    C3DMeshMaterial();
    virtual ~C3DMeshMaterial();
    long          MaterialIndex;                             // +0x04
    CCharString   Name;                                      // +0x08
    long          DecalID;                                   // +0x0c
    unsigned long DiffuseMapBankIndex;                       // +0x10
    unsigned long BumpMapBankIndex;                          // +0x14
    unsigned long ReflectionMapBankIndex;                    // +0x18
    unsigned long IlluminationMapBankIndex;                  // +0x1c
    unsigned long MapFlags;                                  // +0x20
    unsigned long SelfIllumination;                          // +0x24
    bool          TwoSided;                                  // +0x28
    bool          Transparent;                               // +0x29
    bool          BooleanAlpha;                              // +0x2a
    bool          DegenerateTriangles;                       // +0x2b
    CIVCountedPointer<CFilenames> Filenames;                 // +0x2c
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshMaterial) == 0x30);

C3DMeshMaterial::~C3DMeshMaterial()
{
}