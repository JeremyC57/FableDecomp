// CEngineMap::PeekHasWater(long x, long y) -- true when any game-map layer value at (x, y) (inlined PeekLayerValue: own draw map, else the neighbouring map covering the cell, else clamped to this map) names a ground theme whose def is a water type
// Grade: DIFFER(1070v1081) -- family gap: the inlined PeekLayerValue allocates x->edi, y->ebx and spills width with the x copy in the x argument slot in retail; ours picks other registers and drops one spill
// Leaf: PeekHasWater
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
        long width = MapWidth;
        if (!(x < width && y < MapHeight && x >= 0 && y >= 0 && GameMap)) {
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
            else if (x > width - 1)
                x = width - 1;
            if (y < 0)
                y = 0;
            else if (y > MapHeight - 1)
                y = MapHeight - 1;
            if (!GameMap)
                return 0;
        }
        return GameMap->GetLayerValue(x, y, layer);
    }
    float PeekWaterDepth(long x, long y);          // 0x00B40BD0
    bool PeekHasWater(long x, long y);
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

bool CEngineMap::PeekHasWater(long x, long y)
{
    long value = PeekLayerValue(x, y, 0);
    if (value <= 0)
        return false;
    bool hasWater = g_pGroundTypeTableOwner->GroundTypes[g_pDefinitionManager->GetDefClassIndexFromGlobalIndex(value)].Water != 0;
    value = PeekLayerValue(x, y, 1);
    if (value > 0) {
        hasWater |= g_pGroundTypeTableOwner->GroundTypes[g_pDefinitionManager->GetDefClassIndexFromGlobalIndex(value)].Water != 0;
        value = PeekLayerValue(x, y, 2);
        if (value > 0)
            hasWater |= g_pGroundTypeTableOwner->GroundTypes[g_pDefinitionManager->GetDefClassIndexFromGlobalIndex(value)].Water != 0;
    }
    if (hasWater && PeekWaterDepth(x, y) > 0.001f)
        return true;
    return false;
}
