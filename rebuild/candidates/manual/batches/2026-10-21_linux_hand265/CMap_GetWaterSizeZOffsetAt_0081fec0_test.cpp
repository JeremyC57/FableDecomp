#include <cstdio>
#include <cmath>
// Behaviour test for the CMap water queries (0x0081FB00 HasWaterAt, 0x0081FCC0 / 0x0081FEC0 GetWaterSizeZOffsetAt):
// a 3x3 height map, a theme palette and a definition manager that hands out ref-counted theme defs.
struct C2DCoordI {
    long X, Y;
    C2DCoordI() { }
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
struct C3DVector { float X, Y, Z; };
struct CHeightMapCell {
    float         Height;
    unsigned char EngineThemePaletteIndex[3];
    unsigned char EngineBlend[2];
};
struct CHeightMap {
    CHeightMapCell* Cells;
    long SizeX, SizeY;
    CHeightMapCell& GetCellIncludingExtraCells(long x, long y) { return Cells[(SizeY - y) * (SizeX + 1) + x]; }
};
struct CEngineThemePaletteEntry { long DefIndex; long Count; };
struct CEngineThemeDef {
    long  RefCount;
    long  Destroyed;
    float WaterHeight;
    long  WaterType;
    void DestroySelf() { Destroyed++; }
};
struct CDefinitionManager {
    CEngineThemeDef* Defs;
    bool GetDef(long index, CEngineThemeDef** out) { Defs[index].RefCount++; *out = &Defs[index]; return true; }
};
struct CMap {
    CDefinitionManager*       DefinitionManager;
    CEngineThemePaletteEntry* EngineThemePalette;
    CHeightMap*               HeightMap;
    bool HasWaterAt(const C2DCoordI& pos)
    {
        long x = pos.X, y = pos.Y;
        long index0 = EngineThemePalette[HeightMap->GetCellIncludingExtraCells(x, y).EngineThemePaletteIndex[0]].DefIndex;
        if (index0 > 0) {
            CEngineThemeDef* def0 = 0;
            DefinitionManager->GetDef(index0, &def0);
            if (def0->WaterType != 0 && def0->WaterType != 8) { if (--def0->RefCount == 0) def0->DestroySelf(); return true; }
            long index1 = EngineThemePalette[HeightMap->GetCellIncludingExtraCells(x, y).EngineThemePaletteIndex[1]].DefIndex;
            if (index1 > 0) {
                CEngineThemeDef* def1 = 0;
                DefinitionManager->GetDef(index1, &def1);
                if (def1->WaterType != 0 && def1->WaterType != 8) {
                    if (--def1->RefCount == 0) def1->DestroySelf();
                    if (--def0->RefCount == 0) def0->DestroySelf();
                    return true;
                }
                long index2 = EngineThemePalette[HeightMap->GetCellIncludingExtraCells(x, y).EngineThemePaletteIndex[2]].DefIndex;
                if (index2 > 0) {
                    CEngineThemeDef* def2 = 0;
                    DefinitionManager->GetDef(index2, &def2);
                    bool water = def2->WaterType != 0 && def2->WaterType != 8;
                    if (--def2->RefCount == 0) def2->DestroySelf();
                    if (--def1->RefCount == 0) def1->DestroySelf();
                    if (--def0->RefCount == 0) def0->DestroySelf();
                    return water;
                }
                if (--def1->RefCount == 0) def1->DestroySelf();
            }
            if (--def0->RefCount == 0) def0->DestroySelf();
        }
        return false;
    }
    float GetWaterSizeZOffsetAt(const C2DCoordI& pos)
    {
        if (!HasWaterAt(pos)) return 0.0f;
        long x = pos.X, y = pos.Y;
        float height = 0.0f;
        long index0 = EngineThemePalette[HeightMap->GetCellIncludingExtraCells(x, y).EngineThemePaletteIndex[0]].DefIndex;
        if (index0 > 0) {
            CEngineThemeDef* def0 = 0;
            DefinitionManager->GetDef(index0, &def0);
            unsigned char blend0 = HeightMap->GetCellIncludingExtraCells(x, y).EngineBlend[0];
            height = blend0 * def0->WaterHeight * (1.0f / 255.0f);
            long index1 = EngineThemePalette[HeightMap->GetCellIncludingExtraCells(x, y).EngineThemePaletteIndex[1]].DefIndex;
            if (index1 > 0) {
                CEngineThemeDef* def1 = 0;
                DefinitionManager->GetDef(index1, &def1);
                unsigned char blend1 = HeightMap->GetCellIncludingExtraCells(x, y).EngineBlend[1];
                height += blend1 * def1->WaterHeight * (1.0f / 255.0f);
                long index2 = EngineThemePalette[HeightMap->GetCellIncludingExtraCells(x, y).EngineThemePaletteIndex[2]].DefIndex;
                if (index2 > 0) {
                    CEngineThemeDef* def2 = 0;
                    DefinitionManager->GetDef(index2, &def2);
                    unsigned char blend2 = (unsigned char)(255 - blend1 - blend0);
                    height += blend2 * def2->WaterHeight * (1.0f / 255.0f);
                    if (--def2->RefCount == 0) def2->DestroySelf();
                }
                if (--def1->RefCount == 0) def1->DestroySelf();
            }
            if (--def0->RefCount == 0) def0->DestroySelf();
        }
        return height;
    }
    float GetWaterSizeZOffsetAt(const C3DVector& pos)
    {
        long x = (long)pos.X, y = (long)pos.Y;
        float total = 0.0f;
        long count = 0;
        float h = GetWaterSizeZOffsetAt(C2DCoordI(x, y));
        if (h > 0.05f) { total += h + HeightMap->GetCellIncludingExtraCells(x, y).Height; count++; }
        long x1 = x + 1;
        h = GetWaterSizeZOffsetAt(C2DCoordI(x1, y));
        if (h > 0.05f) { total += h + HeightMap->GetCellIncludingExtraCells(x1, y).Height; count++; }
        long y1 = y + 1;
        h = GetWaterSizeZOffsetAt(C2DCoordI(x, y1));
        if (h > 0.05f) { total += h + HeightMap->GetCellIncludingExtraCells(x, y1).Height; count++; }
        h = GetWaterSizeZOffsetAt(C2DCoordI(x1, y1));
        if (h > 0.05f) { total += h + HeightMap->GetCellIncludingExtraCells(x1, y1).Height; count++; }
        if (count > 0) return total / count;
        return 0.0f;
    }
};
static bool Near(float a, float b) { return std::fabs(a - b) < 0.001f; }
static int Fail(const char* addr) { std::printf("%s_TEST FAIL\n", addr); return 1; }
int main()
{
    // defs: 0 unused, 1 land, 2 water (height 2), 3 water type 8 (=none), 4 water (height 4)
    CEngineThemeDef defs[5];
    int i;
    for (i = 0; i < 5; i++) { defs[i].RefCount = 1; defs[i].Destroyed = 0; defs[i].WaterHeight = 0.0f; defs[i].WaterType = 0; }
    defs[2].WaterType = 3; defs[2].WaterHeight = 2.0f;
    defs[3].WaterType = 8; defs[3].WaterHeight = 9.0f;
    defs[4].WaterType = 1; defs[4].WaterHeight = 4.0f;
    CEngineThemePaletteEntry palette[5];
    for (i = 0; i < 5; i++) { palette[i].DefIndex = i; palette[i].Count = 0; }
    CHeightMapCell cells[9];
    for (i = 0; i < 9; i++) {
        cells[i].Height = 10.0f; cells[i].EngineThemePaletteIndex[0] = 1; cells[i].EngineThemePaletteIndex[1] = 0;
        cells[i].EngineThemePaletteIndex[2] = 0; cells[i].EngineBlend[0] = 255; cells[i].EngineBlend[1] = 0;
    }
    CHeightMap hm; hm.Cells = cells; hm.SizeX = 2; hm.SizeY = 2;
    CDefinitionManager dm; dm.Defs = defs;
    CMap map; map.DefinitionManager = &dm; map.EngineThemePalette = palette; map.HeightMap = &hm;
    // cell (0,0) -> index (2-0)*3+0 = 6: land, then water on layer 1 with half blend
    CHeightMapCell& c = hm.GetCellIncludingExtraCells(0, 0);
    c.EngineThemePaletteIndex[0] = 1; c.EngineThemePaletteIndex[1] = 2; c.EngineBlend[0] = 128; c.EngineBlend[1] = 127;
    // cell (1,0): water type 8 only -> no water
    hm.GetCellIncludingExtraCells(1, 0).EngineThemePaletteIndex[0] = 3;
    // cell (0,1): three layers land/8/water4 with blends 100/100/55
    CHeightMapCell& d = hm.GetCellIncludingExtraCells(0, 1);
    d.EngineThemePaletteIndex[0] = 1; d.EngineThemePaletteIndex[1] = 3; d.EngineThemePaletteIndex[2] = 4;
    d.EngineBlend[0] = 100; d.EngineBlend[1] = 100; d.Height = 20.0f;
    const char* A = "0081fec0";
    C3DVector v; v.X = 0.4f; v.Y = 0.7f; v.Z = 0.0f;
    float expect = ((127.0f * 2.0f / 255.0f + 10.0f) + ((100.0f * 9.0f + 55.0f * 4.0f) / 255.0f + 20.0f)) / 2.0f;
    if (!Near(map.GetWaterSizeZOffsetAt(v), expect)) return Fail(A);
    v.X = 1.0f; v.Y = 1.0f;   // probes (1,1) (2,1) (1,2) (2,2): all land
    if (map.GetWaterSizeZOffsetAt(v) != 0.0f) return Fail(A);
    for (i = 0; i < 5; i++) if (defs[i].RefCount != 1 || defs[i].Destroyed != 0) return Fail(A);
    std::printf("0081fec0_TEST PASS\n");
    return 0;
}
