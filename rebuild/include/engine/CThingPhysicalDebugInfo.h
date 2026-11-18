#ifndef FABLE_ENGINE_CTHINGPHYSICALDEBUGINFO_H
#define FABLE_ENGINE_CTHINGPHYSICALDEBUGINFO_H

#ifdef FABLE_RETAIL_CTHINGPHYSICAL_DEBUGDRAW_PASS24
#include "rebuild_abi.h"
class CThingPhysicalDebugInfo {
public:
    bool OnMesh;
    bool OnGround;
    long Type;
    bool Moving;
    bool Collidable;
    bool Interactable;
    bool Immoveable;
};
FABLE_STATIC_ASSERT(sizeof(CThingPhysicalDebugInfo) == 0xC);
#endif
#endif
