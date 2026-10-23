// CEngineMap::PeekHasWaterFast(long x, long y) -- true when one of the first three game-map layers at (x, y) holds a ground theme whose def entry is a water type: each layer value comes from the inlined PeekLayerValue (this map's draw map, else the neighbouring map that covers the cell, else this map clamped to its edges); a value <= 0 ends the search with false
// Grade: DIFFER(1034v1037) -- family gap: inlined PeekLayerValue register/argument-slot allocation (retail x->edi, y->ebx, width spilled, x copy in the x argument slot)
// Leaf: PeekHasWaterFast
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B40260, length 1040 bytes, `ret 8`.
// Callees: CEngineMap::PeekLayerValue(long x, long y, long layer) @ 0x00B43720 (the out-of-line copy of the same inline,
//   reached for the neighbouring map; manifest label "PeekLandscapeHeight" is propagated -- the body is this lookup and
//   ends in the draw map's virtual slot 6), CIDrawMap slot 6 (+0x18) GetLayerValue(x, y, layer),
//   CDefinitionManager::GetDefClassIndexFromGlobalIndex @ 0x009ACCA0 (global 0x01436E28).
// Globals: the engine world map 0x01436E8C (engine/CEngineWorldMap.h: Map +0xc, MapArrayWidth +0x10, MapArrayHeight +0x14,
//   MapOffsetX +0x18, MapOffsetY +0x1c, MapList data +0x20), the ground-type table owner 0x01436EA8 (0x38-byte entries
//   at +0x4b8, the water flag at +0x30).
// Layout: Ego_r.pdb CEngineMap with the retail offsets 4 bytes lower from MapWidth on: GameMap +8, MapWidth +0x5c,
//   MapHeight +0x5e, WorldPosX +0x60, WorldPosY +0x62 (PDB +0x60..+0x66; the out-of-line 0x00B43720 reads the same
//   retail offsets).
#include "engine/CEngineWorldMap.h"

struct CIDrawMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual long GetLayerValue(long x, long y, long layer);   // slot 6 (+0x18)
};

class CEngineMap;
extern CEngineWorldMap* g_pEngineWorldMap;         // 0x01436E8C

class CEngineMap {
public:
    void*          LandscapeMap;                   // +0x00
    void*          LocalDetailCacheMap;            // +0x04
    CIDrawMap*     GameMap;                        // +0x08
    unsigned char  _pad0c[0x5c - 0xc];
    unsigned short MapWidth;                       // +0x5c
    unsigned short MapHeight;                      // +0x5e
    unsigned short WorldPosX;                      // +0x60
    unsigned short WorldPosY;                      // +0x62

    long PeekLayerValueOutOfLine(long x, long y, long layer);   // 0x00B43720
    __forceinline long PeekLayerValue(long x, long y, long layer)
    {
        if (!(x < MapWidth && y < MapHeight && x >= 0 && y >= 0 && GameMap)) {
            CEngineWorldMap* world = g_pEngineWorldMap;
            long worldX = WorldPosX - world->MapOffsetX + x;
            long worldY = WorldPosY - world->MapOffsetY + y;
            if (worldX >= 0 && worldY >= 0 && worldX < world->MapArrayWidth * 32 && worldY < world->MapArrayHeight * 32) {
                long index = ((short*)world->Map)[(worldY >> 5) * world->MapArrayWidth + (worldX >> 5)];
                if (index > 0) {
                    CEngineMap* other = (*(CEngineMap***)world->MapList)[index];
                    if (other) {
                        long otherX = WorldPosX - other->WorldPosX + x;
                        long otherY = WorldPosY - other->WorldPosY + y;
                        if (otherX < other->MapWidth && otherY < other->MapHeight && otherX >= 0 && otherY >= 0
                            && other->GameMap)
                            return other->PeekLayerValueOutOfLine(otherX, otherY, layer);
                    }
                }
            }
            if (x < 0)
                x = 0;
            else if (x > MapWidth - 1)
                x = MapWidth - 1;
            if (y < 0)
                y = 0;
            else if (y > MapHeight - 1)
                y = MapHeight - 1;
            if (!GameMap)
                return 0;
        }
        return GameMap->GetLayerValue(x, y, layer);
    }
    bool PeekHasWaterFast(long x, long y);
};

class CDefinitionManager {
public:
    long GetDefClassIndexFromGlobalIndex(long globalIndex);   // 0x009ACCA0
};
extern CDefinitionManager* g_pDefinitionManager;   // 0x01436E28

struct SGroundTypeEntry {                          // 0x38 bytes
    unsigned char _pad00[0x30];
    long          Water;                           // +0x30
    unsigned char _pad34[4];
};
struct CGroundTypeTableOwner {                     // the object @ 0x01436EA8
    unsigned char     _pad000[0x4b8];
    SGroundTypeEntry* GroundTypes;                 // +0x4b8
};
extern CGroundTypeTableOwner* g_pGroundTypeTableOwner;   // 0x01436EA8

bool CEngineMap::PeekHasWaterFast(long x, long y)
{
    long value = PeekLayerValue(x, y, 0);
    if (value <= 0)
        return false;
    if (g_pGroundTypeTableOwner->GroundTypes[g_pDefinitionManager->GetDefClassIndexFromGlobalIndex(value)].Water)
        return true;
    value = PeekLayerValue(x, y, 1);
    if (value <= 0)
        return false;
    if (g_pGroundTypeTableOwner->GroundTypes[g_pDefinitionManager->GetDefClassIndexFromGlobalIndex(value)].Water)
        return true;
    value = PeekLayerValue(x, y, 2);
    if (value <= 0)
        return false;
    if (g_pGroundTypeTableOwner->GroundTypes[g_pDefinitionManager->GetDefClassIndexFromGlobalIndex(value)].Water)
        return true;
    return false;
}
