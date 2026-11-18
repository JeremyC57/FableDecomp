#ifndef FABLE_ENGINE_CTHINGPHYSICSMESHINFO_H
#define FABLE_ENGINE_CTHINGPHYSICSMESHINFO_H
#ifdef FABLE_RETAIL_CTHINGPHYSICSMESHINFO
#include "engine/C3DVector.h"
class CThingPhysicsMeshInfo {                                // Ego_r.pdb, 28 bytes, no STL member
public:
    unsigned short BankIndex;                                // +0x00
    float          RenderSizeX;                              // +0x04
    C3DVector      ToCentre;                                 // +0x08
    float          BoundingRadius;                           // +0x14
    float          BoundingCircleRadius;                     // +0x18
};
FABLE_STATIC_ASSERT(sizeof(CThingPhysicsMeshInfo) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CThingPhysicsMeshInfo, RenderSizeX) == 4);
#endif
#endif
