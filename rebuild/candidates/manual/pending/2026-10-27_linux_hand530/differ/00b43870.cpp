// CEngineMap::PeekThemeBlend(long x, long y, long layer) const -- the engine theme blend of a landscape cell on this map, following the cell into the neighbouring map that holds it, else the clamped cell's, 0 without a game map
// Grade: DIFFER(327v324) -- the PeekThemeId residue: retail reserves an extra frame slot (`push ecx`) for mapY and keeps x/y in edi/ebp across the neighbour test, writing the argument slots only on the loop back-edge; this build homes mapY in y's argument slot, which shifts every frame offset (4 grades) | next: as 0x00B43720
// Leaf: ?PeekThemeBlend@CEngineMap@@QBEEJJJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B43870, length 324 bytes, `ret 0Ch`.
// Name: FableWin.pdb `?PeekThemeBlend@CEngineMap@@QBEEJJJ@Z` (catalog label "PeekLandscapeHeight" is propagated over all three Peek bodies; this one calls CIDrawMap
//   virtual slot 7 DrawGetEngineBlendAt(long, long, long) const -- CMap's RTTI vtable and the PDB name set agree on the slot order).
// Callee: the game map's CIDrawMap::DrawGetEngineBlendAt (virtual, `call [eax+1Ch]`); the world map 0x01436E8C (g_pEngineWorldMap; MapArrayWidth +0x10,
//   MapArrayHeight +0x14, MapOffsetX/Y +0x18/+0x1C, the 32x32 cell index Map +0x0C, MapList +0x20) through the inline
//   CEngineWorldMap::GetMap(x, y) const.
// Layout: Ego_r.pdb CEngineMap GameMap +0x08, MapWidth/MapHeight +0x5C/+0x5E, WorldPosX/WorldPosY +0x60/+0x62 (equal in retail).
// Shape: the recursion into the neighbouring map is a tail call VC7.1 turns into a loop (`mov ecx,eax ; jmp` back to the top);
//   both GameMap calls are tail-merged into one block.
class C2DExtentsI;
class C3DVector;

class CIDrawMap {
public:
    virtual ~CIDrawMap();
    virtual long DrawGetHeightMapSizeX() const = 0;
    virtual long DrawGetHeightMapSizeY() const = 0;
    virtual void DrawGetHeightMapExtents() const = 0;
    virtual float DrawGetSizeZAt(const C3DVector* pos, float scale) const = 0;
    virtual float DrawBlockGetSizeZAt(unsigned long x, unsigned long y, float scale) const = 0;
    virtual long DrawGetEngineThemeAt(long x, long y, long layer) const;
    virtual unsigned char DrawGetEngineBlendAt(long x, long y, long layer) const;
};

class CEngineMap;

class CEngineWorldMap {
public:
    unsigned char _pad00[0xC];
    short*        Map;              // +0x0C
    long          MapArrayWidth;    // +0x10
    long          MapArrayHeight;   // +0x14
    long          MapOffsetX;       // +0x18
    long          MapOffsetY;       // +0x1C
    CEngineMap**  MapList;          // +0x20 CArray<CEngineMap*> data
    __forceinline CEngineMap* GetMap(long x, long y) const
    {
        if (x < 0 || y < 0 || x >= MapArrayWidth * 32 || y >= MapArrayHeight * 32)
            return 0;
        long index = Map[(y >> 5) * MapArrayWidth + (x >> 5)];
        if (index <= 0)
            return 0;
        return MapList[index];
    }
};
extern CEngineWorldMap* g_pEngineWorldMap;   // 0x01436E8C

class CEngineMap {
public:
    unsigned char    _pad00[0x8];
    const CIDrawMap* GameMap;       // +0x08
    unsigned char    _pad0c[0x50];
    unsigned short   MapWidth;      // +0x5C
    unsigned short   MapHeight;     // +0x5E
    unsigned short   WorldPosX;     // +0x60
    unsigned short   WorldPosY;     // +0x62
    unsigned char PeekThemeBlend(long x, long y, long layer) const;
};

unsigned char CEngineMap::PeekThemeBlend(long x, long y, long layer) const
{
    const CEngineWorldMap* worldMap = g_pEngineWorldMap;
    if (x < MapWidth && y < MapHeight && x >= 0 && y >= 0 && GameMap)
        return GameMap->DrawGetEngineBlendAt(x, y, layer);
    long worldX = WorldPosX - worldMap->MapOffsetX + x;
    long worldY = WorldPosY - worldMap->MapOffsetY + y;
    const CEngineMap* map = worldMap->GetMap(worldX, worldY);
    if (map) {
        long mapY = WorldPosY - map->WorldPosY + y;
        long mapX = WorldPosX - map->WorldPosX + x;
        if (mapX < map->MapWidth && mapY < map->MapHeight && mapX >= 0 && mapY >= 0 && map->GameMap)
            return map->PeekThemeBlend(mapX, mapY, layer);
    }
    if (x < 0)
        x = 0;
    else if (x > MapWidth - 1)
        x = MapWidth - 1;
    if (y < 0)
        y = 0;
    else if (y > MapHeight - 1)
        y = MapHeight - 1;
    if (GameMap)
        return GameMap->DrawGetEngineBlendAt(x, y, layer);
    return 0;
}
