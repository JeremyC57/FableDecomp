#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVE2DPARTICLEGROUP_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVE2DPARTICLEGROUP_H
// CEngineInternalPrimitive2DParticleGroup (new header, wfA4 row 0x00BE0EB0). Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) size 284 with the
// members below at Ego_r +0xC0..+0x118; the retail base CEngineInternalPrimitiveParticleGroup is 0xAC (Ego_r 0xC0), so every member sits 0x14
// lower in retail: Layer +0xAC, InverseTransform +0xB0, Pos +0xE0 (0x00BE0EB0 reads +0xB0..+0xDC and +0xE0..+0xE8), AlphaMaskBankIndex +0xEC.
#include "rebuild_abi.h"
#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DPARTICLEGROUP
#ifndef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP
#endif
#include "engine/CEngineInternalPrimitiveParticleGroup.h"
#include "engine/C3DVector.h"
#include "engine/C4DVector.h"
#include "engine/CMatrix3x4.h"
enum EEngine2DLayer;
class CGraphicDataBank;
class CEngineInternalPrimitive2DParticleGroup : public CEngineInternalPrimitiveParticleGroup {
public:
    EEngine2DLayer                    Layer;                             // +0xac
    CMatrix3x4                        InverseTransform;                  // +0xb0
    C3DVector                         Pos;                               // +0xe0
    unsigned long                     AlphaMaskBankIndex;                // +0xec
    CGraphicDataBank*                 AlphaMaskGraphicBank;              // +0xf0
    C4DVector                         AlphaMaskPositionFactors;          // +0xf4
    bool                              MaskObjectsOutsideAlphaMask;       // +0x104
};
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitive2DParticleGroup) == 0x108);
#endif
#endif // FABLE_ENGINE_CENGINEINTERNALPRIMITIVE2DPARTICLEGROUP_H
