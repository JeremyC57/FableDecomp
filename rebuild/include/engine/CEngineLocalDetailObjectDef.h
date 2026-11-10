#ifndef FABLE_ENGINE_CENGINELOCALDETAILOBJECTDEF_H
#define FABLE_ENGINE_CENGINELOCALDETAILOBJECTDEF_H
// Retail model (chunk 1018, row 0x00477CD2): RTTI .?AVCEngineLocalDetailObjectDef@@ : CParentDefClassBase : CDefClassBase (ghidra_out/rtti_bases.tsv), vtable
// 0x0123634C (rebuild/manifest/rtti-vtables.tsv: slot 0 scalar deleting dtor 0x00477943, 18 Transfer 0x0047769e, 19 Copy 0x0047795f, 20 GetSizeofClass 0x00477698).
// Members: Ego_r.pdb CEngineLocalDetailObjectDef (140 bytes), retail offsets = Ego_r - 4 after the 0x39-byte retail CParentDefClassBase;
//   sizeof 0x88 = the element stride of TransferVectorOfSubComponents<CEngineLocalDetailObjectDef> 0x00477CD2 (`idiv` by 0x88).
// Implicit constructor and destructor: retail builds the default element inline (CParentDefClassBase() 0x00430370, then the
//   0x0123634C vtable store; 0x00477D8D..0x00477DA6) and destroys it with ~CParentDefClassBase 0x00430300 (0x00477DBB).
// Virtual overrides: Ego_r.pdb / FableWin.pdb ?Transfer@CEngineLocalDetailObjectDef@@UAEXAAVCPersistContext@@@Z, ?Copy@CEngineLocalDetailObjectDef@@UAEXPBVCDefClassBase@@@Z,
//   ?GetSizeofClass@CEngineLocalDetailObjectDef@@UAEKXZ (declarations only). Needs FABLE_RETAIL_DEF_CLASS_BASE (the retail def chain).
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#error engine/CEngineLocalDetailObjectDef.h is the retail def model: define FABLE_RETAIL_DEF_CLASS_BASE before including it
#else
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"
class CPersistContext;
class CEngineLocalDetailObjectDef : public CParentDefClassBase {
public:
    virtual void Transfer(CPersistContext&);                 // slot 18
    virtual void Copy(const CDefClassBase*);                 // slot 19
    virtual unsigned long GetSizeofClass();                  // slot 20
    long  Mesh;                                              // +0x3C
    long  ShadowMesh;                                        // +0x40
    float Probability;                                       // +0x44
    float ThemeBlendThreshold;                               // +0x48
    float Scale;                                             // +0x4C
    float ScaleRandomElement;                                // +0x50
    float FadeStart;                                         // +0x54
    float FadeEnd;                                           // +0x58
    long  ZSpriteMesh;                                       // +0x5C
    float ZSpriteFadeStart;                                  // +0x60
    float ZSpriteFadeEnd;                                    // +0x64
    float SlopeFadeStart;                                    // +0x68
    float SlopeFadeEnd;                                      // +0x6C
    long  AlphaRef;                                          // +0x70
    float AlphaMipBias;                                      // +0x74
    float WindSkewConstantFactor;                            // +0x78
    float WindSkewRandomFactor;                              // +0x7C
    float WindSkewSpeedFactor;                               // +0x80
    bool  CastShadows : 1;                                   // +0x84 bit 0
    bool  ReceiveShadows : 1;                                // +0x84 bit 1
    bool  IsRepeatedMesh : 1;                                // +0x84 bit 2
    bool  IsZSprite : 1;                                     // +0x84 bit 3
    bool  AlphaIsBoolean : 1;                                // +0x84 bit 4
    bool  HasLandscapeNormalLighting : 1;                    // +0x84 bit 5
    bool  TiltToSlope : 1;                                   // +0x84 bit 6
    bool  HasWindSkew : 1;                                   // +0x84 bit 7
};
FABLE_STATIC_ASSERT(sizeof(CEngineLocalDetailObjectDef) == 0x88);
#endif // FABLE_RETAIL_DEF_CLASS_BASE
#endif // FABLE_ENGINE_CENGINELOCALDETAILOBJECTDEF_H
