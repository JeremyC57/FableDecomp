// CEngineMap::PeekWaterHeight(long x, long y) const -- the water surface height at map cell (x, y): 0 where the water depth is <= 0.001; otherwise the land height under the water plus the depth -- taken from the neighbouring map that owns the cell when (x, y) lies outside this map (or this map has no game map), else from this map's game map at the clamped cell, quantised to 1/128 units
// Grade: DIFFER(427v424) -- try p dl=4: Ego_r const signatures, inline CEngineWorldMap::GetMap + QuantiseLandscapeHeight, world local, ly before lx; residue: retail spills the zero-extended MapWidth (movzx) for the x clamp -- a long width local is rematerialised from this, an unsigned short / short local spills but loads `mov ax ; movzx eax,ax`; plus one ebp reload placement (best try p; the bare-dword fistp itself is reproduced by the shared helper, see NOTES.md)
// Leaf: ?PeekWaterHeight@CEngineMap@@QBEMJJ@Z
// Flags: none
// Retail-asm: 0x00B41BEA `fistp dword ptr [esp+1Ch]` -- engine/GFFloatToLongNear.h (GFFloatToLongNear inside the inline Ego_r
//   CEngineMap::QuantiseLandscapeHeight(float) const: the height times 128 stored to the dead argument slot, reloaded, bare dword
//   fistp, fild, times 1/128). VC7.1 `(long)x` calls __ftol2, `/QIfist` stores `fistp qword`, `_mm_cvtss_si32` emits cvtss2si.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B41A80, 424 bytes (+int3), `ret 8`.
// Name: Ego_r.pdb ?PeekWaterHeight@CEngineMap@@QBEMJJ@Z.
// Callees: CEngineMap::PeekWaterDepth(long, long) const 0x00B40BD0, CEngineMap::PeekLandscapeHeight(long, long) const 0x00B439C0
//   (the neighbour; Ego_r names), CIDrawMap slot 5 (+0x14) GetHeight(x, y, 1.0f). Inlined: CEngineWorldMap::GetMap(long, long)
//   const, CEngineMap::QuantiseLandscapeHeight(float) const (Ego_r / FableWin names). World map 0x01436E8C.
// Layout: Ego_r.pdb CEngineMap less 4 from +0x5c (MapWidth +0x5c, MapHeight +0x5e, WorldPosX +0x60, WorldPosY +0x62; GameMap
//   +0x08); CEngineWorldMap: Map +0x0c, MapArrayWidth/Height +0x10/+0x14, MapOffsetX/Y +0x18/+0x1c, MapList +0x20.
// Constants: 0.001f @0x012A1ED8, 128.0f @0x012571B0, 1/128 @0x012A1F34, 0.0f @0x0122DEDC.
#include "engine/GFFloatToLongNear.h"
class CIDrawMap { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual float GetHeight(long x, long y, float scale) const;   // slot 5 (+0x14)
};
class CEngineMap;
class CEngineWorldMap { public:
    unsigned char _pad00[0x0c];
    short*        Map;                     // +0x0c
    long          MapArrayWidth;           // +0x10
    long          MapArrayHeight;          // +0x14
    long          MapOffsetX;              // +0x18
    long          MapOffsetY;              // +0x1c
    CEngineMap**  MapList;                 // +0x20
    __forceinline CEngineMap* GetMap(long x, long y) const
    {
        if (x >= 0 && y >= 0 && x < MapArrayWidth * 32 && y < MapArrayHeight * 32) {
            long index = Map[(y >> 5) * MapArrayWidth + (x >> 5)];
            if (index > 0)
                return MapList[index];
        }
        return 0;
    }
};
extern CEngineWorldMap* g_pEngineWorldMap;     // 0x01436E8C

class CEngineMap { public:
    void*          LandscapeMap;           // +0x00
    void*          LocalDetailCacheMap;    // +0x04
    CIDrawMap*     GameMap;                // +0x08
    unsigned char  _pad0c[0x5c - 0x0c];
    unsigned short MapWidth;               // +0x5c
    unsigned short MapHeight;              // +0x5e
    unsigned short WorldPosX;              // +0x60
    unsigned short WorldPosY;              // +0x62
    float PeekWaterDepth(long x, long y) const;        // 0x00B40BD0
    float PeekLandscapeHeight(long x, long y) const;   // 0x00B439C0
    float PeekWaterHeight(long x, long y) const;
private:
    float QuantiseLandscapeHeight(float height) const { return GFFloatToLongNear(height * 128.0f) * (1.0f / 128.0f); }
};

float CEngineMap::PeekWaterHeight(long x, long y) const
{
    float depth = PeekWaterDepth(x, y);
    if (depth > 0.001f) {
        unsigned short width = MapWidth;
        long cy = y;
        long cx = x;
        if (!(x < width && y < MapHeight && x >= 0 && y >= 0) || !GameMap) {
            const CEngineWorldMap* world = g_pEngineWorldMap;
            long wx = WorldPosX - world->MapOffsetX + x;
            long wy = WorldPosY - world->MapOffsetY + y;
            const CEngineMap* other = world->GetMap(wx, wy);
            if (other) {
                long ly = WorldPosY - other->WorldPosY + y;
                long lx = WorldPosX - other->WorldPosX + x;
                if (lx < other->MapWidth && ly < other->MapHeight && lx >= 0 && ly >= 0 && other->GameMap)
                    return other->PeekLandscapeHeight(lx, ly) + depth;
            }
            cx = x < 0 ? 0 : (x > width - 1 ? width - 1 : cx);
            cy = y < 0 ? 0 : (y > MapHeight - 1 ? MapHeight - 1 : cy);
        }
        float height = GameMap ? QuantiseLandscapeHeight(GameMap->GetHeight(cx, cy, 1.0f)) : 0.0f;
        return height + depth;
    }
    return 0.0f;
}
