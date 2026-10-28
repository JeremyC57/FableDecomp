// CEngineMap::PeekLandscapeHeight(long x, long y) const -- the landscape height of a cell on this map quantised to 1/128 (the game map's block height at scale 1), following the cell into the neighbouring map that holds it, else the clamped cell's, 0.0f without a game map
// Grade: DIFFER(345v356) -- the PeekThemeId residue (retail's extra `push ecx` frame slot for mapY, x/y kept in edi/ebp; ours homes mapY in y's argument slot, so every frame offset shifts by 4); the quantise tail (`fmul 128.0 ; fstp [f] ; fld [f] ; fistp [r] ; fild [r] ; fmul 1/128`) matches retail instruction for instruction | 4 grades: earlier F 345v356 and explicit-loop forms (worse); now, with the asm helper, two early returns 378v356 / 395v356 and this one-call-site form 345v356 (113 lines) | next: fix the mapY slot as for 0x00B43720; then this lands as retail_asm
// Leaf: ?PeekLandscapeHeight@CEngineMap@@QBEMJJ@Z
// Retail-asm: 0x00B43AF8..0x00B43B04 `fstp dword ptr [esp+1Ch] ; fld dword ptr [esp+1Ch] ; fistp dword ptr [esp+18h] ; fild dword ptr [esp+18h]` -- GFFloatToLongNear's inline-assembly `fld value ; fistp result` (a bare `fistp dword` rounding with the FPU mode; VC7.1 C++ `(long)f` calls __ftol2), its parameter and result homed in the dead x/y argument slots.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B439C0, length 356 bytes, `ret 8`, float in st(0).
// Name: FableWin.pdb `?PeekLandscapeHeight@CEngineMap@@QBEMJJ@Z` (the catalog spelling is right for this one; it was propagated to
//   0x00B43720 / 0x00B43870). Inlines (FableWin.pdb): CEngineMap::QuantiseLandscapeHeight(float) const (`?QuantiseLandscapeHeight@
//   CEngineMap@@ABEMM@Z`, private) = (float)GFFloatToLongNear(h * 128.0f) * 0.0078125f; GFFloatToLongNear (`?GFFloatToLongNear@@YIJM@Z`,
//   __fastcall); CEngineWorldMap::GetMap. Constants 128.0f @ 0x012571B0, 0.0078125f @ 0x012A1F34, 0.0f @ 0x0122DEDC.
// Callee: the game map's CIDrawMap::DrawBlockGetSizeZAt(unsigned long, unsigned long, float) const (virtual slot 5, `call [eax+14h]`,
//   scale 1.0f); the world map 0x01436E8C (g_pEngineWorldMap) as in 0x00B43720.
// Layout: Ego_r.pdb CEngineMap GameMap +0x08, MapWidth/MapHeight +0x5C/+0x5E, WorldPosX/WorldPosY +0x60/+0x62.
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
__forceinline long __fastcall GFFloatToLongNear(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

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
    float PeekLandscapeHeight(long x, long y) const;
private:
    __forceinline float QuantiseLandscapeHeight(float height) const { return (float)GFFloatToLongNear(height * 128.0f) * 0.0078125f; }
public:
};

float CEngineMap::PeekLandscapeHeight(long x, long y) const
{
    const CEngineWorldMap* worldMap = g_pEngineWorldMap;
    if (!(x < MapWidth && y < MapHeight && x >= 0 && y >= 0 && GameMap)) {
        long worldX = WorldPosX - worldMap->MapOffsetX + x;
        long worldY = WorldPosY - worldMap->MapOffsetY + y;
        const CEngineMap* map = worldMap->GetMap(worldX, worldY);
        if (map) {
            long mapY = WorldPosY - map->WorldPosY + y;
            long mapX = WorldPosX - map->WorldPosX + x;
            if (mapX < map->MapWidth && mapY < map->MapHeight && mapX >= 0 && mapY >= 0 && map->GameMap)
                return map->PeekLandscapeHeight(mapX, mapY);
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
            return 0.0f;
    }
    return QuantiseLandscapeHeight(GameMap->DrawBlockGetSizeZAt(x, y, 1.0f));
}
