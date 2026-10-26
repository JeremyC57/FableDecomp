// CTCMapwho::MoveInMapwho(const C3DVector& pos) -- re-place the thing in the mapwho: drop its nodes when it is in (DestroyMapwhoNode), then, if that left it out, place it at the new position (PlaceThing)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071A750, length 54 bytes.
// Callees: the CTCBase world-map accessor @ 0x00686D30 (`[g_pMainGameComponent]+0x28`, `this` unused),
// CThingMapManager::DestroyMapwhoNode(CTCMapwho*) @ 0x00637A10, CThingMapManager::PlaceThing(CTCMapwho*, const C3DVector&)
// @ 0x00637FC0 (the manager at world map +0x10). The second half is the inline PlaceInMapwho (landed 0x0071A710).
// Layout: engine/CTCMapwho.h (InMapwho = bit 0 of the byte at +0x50); methods on a layout-only extension.
// Shape: the InMapwho bit is re-tested after the removal (the call may clear it); both accessor results are named locals
// so the call runs BEFORE the argument pushes (a chained `GetWorldMap()->m->F(args)` pushes first); the manager load
// lands between the two pushes.
#include "engine/CTCMapwho.h"

class C3DVector { public: float x, y, z; };

struct CThingMapManager {
    void DestroyMapwhoNode(CTCMapwho* mapwho);                          // 0x00637A10
    void PlaceThing(CTCMapwho* mapwho, const C3DVector& pos);           // 0x00637FC0
};

class CWorldMap { public:
    unsigned char     _pad_0x00[0x10];
    CThingMapManager* m_pThingMapManager;                               // +0x10
};

struct CTCMapwhoMethods : CTCMapwho {
    CWorldMap* GetWorldMap_00686d30();                                           // 0x00686D30 (CTCBase accessor)
    bool IsInMapwho() const { return (InMapwho[0] & 1) != 0; }
    void PlaceInMapwho(const C3DVector& pos)
    {
        if (!IsInMapwho()) {
            CWorldMap* map = GetWorldMap_00686d30();
            map->m_pThingMapManager->PlaceThing(this, pos);
        }
    }
    void MoveInMapwho(const C3DVector& pos);
};

void CTCMapwhoMethods::MoveInMapwho(const C3DVector& pos)
{
    if (IsInMapwho()) {
        CWorldMap* map = GetWorldMap_00686d30();
        map->m_pThingMapManager->DestroyMapwhoNode(this);
    }
    PlaceInMapwho(pos);
}