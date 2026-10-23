// CEngineMap::PeekWaterType(long x, long y) -- return the water type of the first game-map layer at (x, y) whose ground-theme def is water (inlined PeekLayerValue per layer), else 0
// Grade: DIFFER(1072v1125) -- family gap: same PeekLayerValue inline register/argument-slot allocation as 00b412a0, plus a second inlined lookup retail keeps in a different block order
// Leaf: PeekWaterType
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
    long PeekWaterType(long x, long y);
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

long CEngineMap::PeekWaterType(long x, long y)
{
    long waterType = 0;
    long value0 = PeekLayerValue(x, y, 0);
    long value1 = PeekLayerValue(x, y, 1);
    long value2 = PeekLayerValue(x, y, 2);
    long class0 = g_pDefinitionManager->GetDefClassIndexFromGlobalIndex(value0);
    long class1 = g_pDefinitionManager->GetDefClassIndexFromGlobalIndex(value1);
    long class2 = g_pDefinitionManager->GetDefClassIndexFromGlobalIndex(value2);
    SGroundTypeEntry* types = g_pGroundTypeTableOwner->GroundTypes;
    if (types[class0].Water)
        waterType = types[class0].Water;
    if (class1 && types[class1].Water) {
        if (waterType == 0 || waterType == 7)
            waterType = types[class1].Water;
        if (class2 && types[class2].Water && (waterType == 0 || waterType == 7))
            return types[class2].Water;
    }
    return waterType;
}
