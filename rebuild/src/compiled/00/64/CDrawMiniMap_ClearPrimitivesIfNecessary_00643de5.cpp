#pragma optimize("s",on)
// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `sub` -> `MiniMap`.
#include "engine/CDrawMiniMap.h"

struct MiniMapPrimitiveStorage { void Clear(); };
struct CDrawMiniMap_Methods : CDrawMiniMap {
    virtual void vslot0();
    virtual void ClearDeferredPrimitives();
    void ClearPrimitivesIfNecessary();
};

void CDrawMiniMap_Methods::ClearPrimitivesIfNecessary() {
    CDrawMiniMap* miniMap = reinterpret_cast<CDrawMiniMap*>(this);
    if (miniMap->f_18 == 0)
        reinterpret_cast<MiniMapPrimitiveStorage*>(&miniMap->MiniMap)->Clear();
    if (miniMap->RegionChange == 0)          // +0xeb (the Ego_r layout read it as MiniMap[0xbf])
        ClearDeferredPrimitives();
}
