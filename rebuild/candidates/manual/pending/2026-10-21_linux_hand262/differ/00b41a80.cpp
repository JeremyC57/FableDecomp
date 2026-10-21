// Grade: DIFFER(329v424) -- lane gap: retail quantises the game-map height with an inline-asm rounding helper (`fstp [arg] ; fld [arg] ; fistp dword ; fild`, GOTCHAS wave 25 fistp family); this body (one structural attempt) also still differs in frame size (the clamped coordinates / width live in a 0x10 frame), the neighbour-map lookup (not inlined the same way) and the early-out layout
// CEngineMap::PeekWaterHeight(long x, long y) -- the water surface height at map cell (x, y): 0 where the water depth is
//   <= 0.001; otherwise the land height under the water plus the depth -- taken from the neighbouring map that owns the cell
//   when (x, y) lies outside this map (or this map has no game map), else from this map's game map at the clamped cell,
//   rounded to 1/128 units.
// Leaf: PeekWaterHeight
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B41A80, 424 bytes (+int3), `ret 8`.
// Callees: CEngineMap::PeekWaterDepth 0x00B40BD0, CEngineMap::PeekLandscapeHeight 0x00B439C0 (the neighbour), CIDrawMap slot 5
//   (+0x14) GetHeight(x, y, 1.0f); world map 0x01436E8C (Ego_r.pdb CEngineWorldMap: Map +0x0c, MapArrayWidth/Height +0x10/+0x14,
//   MapOffsetX/Y +0x18/+0x1c, MapList +0x20). Constants: 0.001f @0x012A1ED8, 128.0f @0x012571B0, 1/128 @0x012A1F34, 0.0f @0x0122DEDC.
// Layout: Ego_r.pdb CEngineMap less 4 from +0x5c (MapWidth +0x5c, MapHeight +0x5e, WorldPosX +0x60, WorldPosY +0x62; GameMap +0x08);
//   no engine/ header exists.
struct CIDrawMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual float GetHeight(long x, long y, float scale);   // slot 5 (+0x14)
};
struct CEngineWorldMap;
inline long RoundToLong(float value) { return (long)value; }   // retail: an inline-asm fistp helper (lane gap)

struct CEngineMap {
    void*          LandscapeMap;           // +0x00
    void*          LocalDetailCacheMap;    // +0x04
    CIDrawMap*     GameMap;                // +0x08
    unsigned char  _pad0c[0x5c - 0x0c];
    unsigned short MapWidth;               // +0x5c
    unsigned short MapHeight;              // +0x5e
    unsigned short WorldPosX;              // +0x60
    unsigned short WorldPosY;              // +0x62
    float PeekWaterDepth(long x, long y);       // 0x00B40BD0
    float PeekLandscapeHeight(long x, long y);  // 0x00B439C0
    float PeekLandscapeHeightInline(long x, long y)
    {
        if (GameMap)
            return RoundToLong(GameMap->GetHeight(x, y, 1.0f) * 128.0f) * 0.0078125f;
        return 0.0f;
    }
    bool IsInside(long x, long y) const { return x < MapWidth && y < MapHeight && x >= 0 && y >= 0; }
    float PeekWaterHeight(long x, long y);
};
struct CEngineWorldMap {
    unsigned char _pad00[0x0c];
    short*        Map;                     // +0x0c
    long          MapArrayWidth;           // +0x10
    long          MapArrayHeight;          // +0x14
    long          MapOffsetX;              // +0x18
    long          MapOffsetY;              // +0x1c
    CEngineMap**  MapList;                 // +0x20
    CEngineMap* PeekMapAt(long x, long y) const
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

float CEngineMap::PeekWaterHeight(long x, long y)
{
    float depth = PeekWaterDepth(x, y);
    if (depth <= 0.001f)
        return 0.0f;
    long width = MapWidth;
    long cx = x, cy = y;
    if (!IsInside(x, y) || !GameMap) {
        long wx = WorldPosX - g_pEngineWorldMap->MapOffsetX + x;
        long wy = WorldPosY - g_pEngineWorldMap->MapOffsetY + y;
        CEngineMap* other = g_pEngineWorldMap->PeekMapAt(wx, wy);
        if (other) {
            long lx = WorldPosX - other->WorldPosX + x;
            long ly = WorldPosY - other->WorldPosY + y;
            if (other->IsInside(lx, ly) && other->GameMap)
                return other->PeekLandscapeHeight(lx, ly) + depth;
        }
        cx = x < 0 ? 0 : (x > width - 1 ? width - 1 : x);
        cy = y < 0 ? 0 : (y > MapHeight - 1 ? MapHeight - 1 : y);
    }
    return PeekLandscapeHeightInline(cx, cy) + depth;
}
