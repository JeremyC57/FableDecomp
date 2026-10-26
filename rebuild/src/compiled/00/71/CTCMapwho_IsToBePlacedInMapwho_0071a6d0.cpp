// CTCMapwho::IsToBePlacedInMapwho(const C3DVector& pos) -- a thing belongs in the mapwho when its position is inside the world map, or when the thing-map manager says its footprint overlaps map cells
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071A6D0, length 61 bytes.
// Callees: the CTCBase world-map accessor @ 0x00686D20 (`[g_pMainGameComponent]+0x28`, `this` unused, called twice),
// CWorldMap::IsPosInMap(const C3DVector&) @ 0x004FC600, CThingMapManager::IsOverlappingCells(CTCMapwho*, const C3DVector&)
// @ 0x00637F40 (the manager at world map +0x10).
// Layout: engine/CTCMapwho.h (PDB, 0x54 bytes); the methods sit on a layout-only extension because the generated header
// declares no member functions. Shape: the position argument is pushed before the accessor call (arguments first, then the
// object expression); the manager is taken into a local BEFORE the second call's argument pushes (a chained call pushes
// first); `if (!f()) return false; return true;` gives retail's `test al,al ; setne al`.
#include "engine/CTCMapwho.h"

class C3DVector { public: float x, y, z; };

struct CThingMapManager {
    bool IsOverlappingCells(CTCMapwho* mapwho, const C3DVector& pos);   // 0x00637F40
};

class CWorldMap { public:
    unsigned char     _pad_0x00[0x10];
    CThingMapManager* m_pThingMapManager;                               // +0x10
    bool IsPosInMap(const C3DVector& pos);                              // 0x004FC600
};

struct CTCMapwhoMethods : CTCMapwho {
    CWorldMap* GetWorldMap();                                           // 0x00686D20 (CTCBase accessor)
    bool IsToBePlacedInMapwho(const C3DVector& pos);
};

bool CTCMapwhoMethods::IsToBePlacedInMapwho(const C3DVector& pos)
{
    if (GetWorldMap()->IsPosInMap(pos))
        return true;
    CThingMapManager* manager = GetWorldMap()->m_pThingMapManager;
    if (!manager->IsOverlappingCells(this, pos))
        return false;
    return true;
}