#ifndef FABLE_ENGINE_CENGINEINTERNALPRIMITIVEPARTICLEDECAL_H
#define FABLE_ENGINE_CENGINEINTERNALPRIMITIVEPARTICLEDECAL_H
// New header (chunk 2038): Ego_r.pdb CEngineInternalPrimitiveParticleDecal (sizeof 0x7C; no container member, so retail = Ego_r):
// RTTI : CEngineInternalPrimitiveBase (0x48 bytes), own members WorldPos +0x48, Size +0x54, Colour +0x5C, DecalFrame +0x60, RHSet +0x64.
// Retail evidence: RenderLandscapeDecalGroup 0x00BB8A66..0x00BB8C85 reads WorldPos.X/Y/Z +0x48/+0x4C/+0x50, Size.X/Y +0x54/+0x58, the
// colour bytes +0x5C..+0x5F and RHSet.Up +0x64..+0x6C; the landed Update 0x00BB6CE0 / GetWorldPosition 0x00BB6D50.
// Functions: Ego_r.pdb / FableWin.pdb ??0 / ??1 (virtual) / Update / GetWorldPosition (declarations). Data public (the particle decal
// group renderer reads them in place; access not recorded by the layout dump).
#include "rebuild_abi.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CRightHandedSet.h"
class CEnginePrimitive;
class CEngineInternalPrimitiveParticleDecal : public CEngineInternalPrimitiveBase {
public:
    C3DVector       WorldPos;                   // +0x48
    C2DVector       Size;                       // +0x54
    CRGBColour      Colour;                     // +0x5c
    long            DecalFrame;                 // +0x60
    CRightHandedSet RHSet;                      // +0x64
};
FABLE_STATIC_ASSERT(sizeof(CEngineInternalPrimitiveParticleDecal) == 0x7c);
#endif // FABLE_ENGINE_CENGINEINTERNALPRIMITIVEPARTICLEDECAL_H
