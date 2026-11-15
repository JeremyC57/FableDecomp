#ifndef FABLE_ENGINE_CENGINEPRIMITIVEPARTICLEDECAL_H
#define FABLE_ENGINE_CENGINEPRIMITIVEPARTICLEDECAL_H
#include "engine/CEnginePrimitive.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CRightHandedSet.h"
// New shared header (wfD6, row 0x00B1F200): Ego_r.pdb CEnginePrimitiveParticleDecal (64 bytes): WorldPos +0x0c, Size +0x18,
// Colour +0x20, DecalFrame +0x24, RHSet +0x28. Constructor Ego_r/FableWin
// ??0CEnginePrimitiveParticleDecal@@QAE@JABVC3DVector@@ABVC2DVector@@ABVCRGBColour@@ABVCRightHandedSet@@@Z, out of line at 0x00B1FE80
// (landed; type 0x19).
class CEnginePrimitiveParticleDecal : public CEnginePrimitive {
public:
    C3DVector WorldPos;                                 // +0x0c
    C2DVector Size;                                     // +0x18
    CRGBColour Colour;                                  // +0x20
    long DecalFrame;                                    // +0x24
    CRightHandedSet RHSet;                              // +0x28
    CEnginePrimitiveParticleDecal(long decal_frame, const C3DVector& world_pos, const C2DVector& size, const CRGBColour& colour,
                                  const CRightHandedSet& rh_set);
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveParticleDecal)==64);
#endif
