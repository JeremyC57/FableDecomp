// Behaviour test for 0x008203A0 CMap::ApplyScriptedMapBrushUpdate (RST2): engine stubbed, the body verbatim from final.cpp.
// Checks: amount == 1 copies brush heights (clamped to [0, 2048 - 1e-4]), theme palette indices (0 for theme 0) and
// blends into the area shrunk by one on the top/left, leaves other cells alone and calls UpdateBrushHeights,
// UpdateBrushThemes, UpdateBrushBlends in that order; amount != 1 blends heights with the radial falloff weight;
// both paths report GetWorldBox(region index, game area) to the update sink.
#include <stdio.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define _snprintf snprintf
#endif
extern "C" double pow(double x, double y);
extern "C" double sqrt(double x);
extern "C" double fabs(double x);
inline float fabs(float x) { return (float)fabs((double)x); }
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static const float g_zero_0122dedc = 0.0f;
static const float g_maxSize_012724c0 = 2048.0f;
static const float g_epsilon_0129ba3c = 1e-4f;
static float Abs(float v) { return v < 0 ? -v : v; }

struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) {}
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
    float GetLength() const { return (float)sqrt(X * X + Y * Y); }
};
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    C2DBoxI() {}
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) {}
};
static int g_areaCalls = 0;
struct CScriptedMapBrush {
    C2DBoxI Area;
    const C2DBoxI& GetGameArea() const { ++g_areaCalls; return Area; }
    float GetGameHeight(long x, long y) const
    {
        if (x == 1 && y == 1) return -5.0f;
        if (x == 3 && y == 2) return 5000.0f;
        return (float)(x * 10 + y);
    }
    unsigned long GetTheme(long x, long y, unsigned long layer) const { return (unsigned long)((x + y + layer) % 3); }
    unsigned char GetThemeBlend(long x, long y, unsigned long layer) const { return (unsigned char)(x * 16 + y * 4 + layer); }
};
struct CHeightMapCell {
    float         Height;
    unsigned char EngineThemePaletteIndex[3];
    unsigned char EngineBlend[2];
    unsigned char Flags;
    unsigned char _pad0a[2];
};
template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned long n) { return *(_M_start + n); }
};
struct CHeightMap {
    CRetailVector<CHeightMapCell> Cells;
    long SizeX;
    long SizeY;
    __forceinline CHeightMapCell& GetCellIncludingExtraCells(long x, long y) { return Cells[(SizeY - y) * (SizeX + 1) + x]; }
    void SetSizeZAt(const C2DCoordI& cell, float size)
    {
        float value;
        if (size < g_zero_0122dedc) {
            value = g_zero_0122dedc;
        } else {
            float limit = g_maxSize_012724c0 - g_epsilon_0129ba3c;
            value = size > limit ? limit : size;
        }
        GetCellIncludingExtraCells(cell.X, cell.Y).Height = value;
    }
};
template <class T> struct scoped_ptr {
    T* ptr;
    T* operator->() const { return ptr; }
};
static const void* g_key = 0;
struct CMapRegionTable {
    virtual long GetRegionIndex(const void* key) { g_key = key; return 7; }
    C2DBoxI GetWorldBox(long index, const C2DBoxI& box) { return C2DBoxI(box.TLX + index * 100, box.TLY + index * 100, box.BRX + index * 100, box.BRY + index * 100); }
};
static int g_updates = 0;
static C2DBoxI g_updated;
struct CMapUpdateSink {
    virtual void AddUpdateArea(const C2DBoxI& box) { ++g_updates; g_updated = box; }
};
struct CMapOwner {
    CMapRegionTable Table;
    CMapUpdateSink Sink;
    virtual CMapUpdateSink* GetUpdateSink() { return &Sink; }
    virtual CMapRegionTable* GetRegionTable() { return &Table; }
};
static char g_order[8];
static int g_orderCount = 0;
struct CMap {
    CMapOwner*             POwner;
    scoped_ptr<CHeightMap> HeightMap;
    unsigned char          RegionKey[4];
    CScriptedMapBrush*     Expected;
    virtual void UpdateBrushHeights(CScriptedMapBrush* brush) { CHECK(brush == Expected); g_order[g_orderCount++] = 'H'; }
    virtual void UpdateBrushBlends(CScriptedMapBrush* brush) { CHECK(brush == Expected); g_order[g_orderCount++] = 'B'; }
    virtual void UpdateBrushThemes(CScriptedMapBrush* brush) { CHECK(brush == Expected); g_order[g_orderCount++] = 'T'; }
    unsigned char AddThemeDefIndexToPalette(unsigned long themeDefIndex) { return (unsigned char)(themeDefIndex + 40); }
    __forceinline void SetEngineThemeAtIncludingExtraCells(long x, long y, unsigned long themeDefIndex, long layer)
    {
        if (themeDefIndex > 0) {
            long paletteIndex = AddThemeDefIndexToPalette(themeDefIndex);
            HeightMap->GetCellIncludingExtraCells(x, y).EngineThemePaletteIndex[layer] = (unsigned char)paletteIndex;
        } else
            HeightMap->GetCellIncludingExtraCells(x, y).EngineThemePaletteIndex[layer] = 0;
    }
    void SetEngineBlendAtIncludingExtraCells(long x, long y, unsigned char blend, long layer)
    {
        HeightMap->GetCellIncludingExtraCells(x, y).EngineBlend[layer] = blend;
    }
    void ApplyScriptedMapBrushUpdate(float amount, CScriptedMapBrush* brush);
};

void CMap::ApplyScriptedMapBrushUpdate(float amount, CScriptedMapBrush* brush)
{
    const C2DBoxI& area = brush->GetGameArea();
    C2DBoxI box = area;
    ++box.TLX;
    ++box.TLY;
    long centreX = box.BRX - (box.BRX - box.TLX) / 2;
    long centreY = box.BRY - (box.BRY - box.TLY) / 2;
    float invRadius = 1.0f / C2DVector((float)(centreX - box.BRX), (float)(centreY - box.BRY)).GetLength();
    if (fabs(amount - 1.0f) > 1e-4f) {
        for (long y = box.TLY; y < box.BRY; ++y) {
            for (long x = box.TLX; x < box.BRX; ++x) {
                float distance = C2DVector((float)(x - centreX), (float)(y - centreY)).GetLength() * invRadius;
                float falloff = 1.0f - distance;
                falloff = falloff * falloff;
                float weight = falloff * amount;
                double p3 = pow(amount, 3.0);
                double p4 = pow(amount, 4.0);
                weight = weight * (float)p3 + (float)p4;
                if (weight > 1.0f)
                    weight = 1.0f;
                float oldHeight = HeightMap->GetCellIncludingExtraCells(x, y).Height;
                float height = brush->GetGameHeight(x, y) * weight + (1.0f - weight) * oldHeight;
                HeightMap->SetSizeZAt(C2DCoordI(x, y), height);
            }
        }
    } else {
        for (long y = box.TLY; y < box.BRY; ++y) {
            for (long x = box.TLX; x < box.BRX; ++x) {
                float height = brush->GetGameHeight(x, y);
                HeightMap->SetSizeZAt(C2DCoordI(x, y), height);
                for (long layer = 0; layer < 3; ++layer)
                    SetEngineThemeAtIncludingExtraCells(x, y, brush->GetTheme(x, y, layer), layer);
                for (long blend = 0; blend < 2; ++blend)
                    SetEngineBlendAtIncludingExtraCells(x, y, brush->GetThemeBlend(x, y, blend), blend);
            }
        }
        UpdateBrushHeights(brush);
        UpdateBrushThemes(brush);
        UpdateBrushBlends(brush);
    }
    long region = POwner->GetRegionTable()->GetRegionIndex(RegionKey);
    C2DBoxI worldBox = POwner->GetRegionTable()->GetWorldBox(region, brush->GetGameArea());
    POwner->GetUpdateSink()->AddUpdateArea(worldBox);
}

static CHeightMapCell g_cells[30];
static void Reset(CHeightMap& hm)
{
    for (int i = 0; i < 30; ++i) {
        g_cells[i].Height = 100.0f;
        g_cells[i].EngineThemePaletteIndex[0] = g_cells[i].EngineThemePaletteIndex[1] = g_cells[i].EngineThemePaletteIndex[2] = 0xEE;
        g_cells[i].EngineBlend[0] = g_cells[i].EngineBlend[1] = 0xDD;
    }
    hm.Cells._M_start = g_cells; hm.Cells._M_finish = g_cells + 30; hm.Cells._M_end_of_storage = g_cells + 30;
    hm.SizeX = 5; hm.SizeY = 4;
    g_orderCount = 0; g_updates = 0; g_key = 0; g_areaCalls = 0;
}
static bool InArea(long x, long y) { return x >= 1 && x < 4 && y >= 1 && y < 3; }
int main()
{
    CMapOwner owner;
    CHeightMap hm;
    CScriptedMapBrush brush;
    brush.Area = C2DBoxI(0, 0, 4, 3);
    CMap map;
    map.POwner = &owner;
    map.HeightMap.ptr = &hm;
    map.Expected = &brush;
    float limit = 2048.0f - 1e-4f;

    // amount == 1: copy path
    Reset(hm);
    map.ApplyScriptedMapBrushUpdate(1.0f, &brush);
    for (long y = 0; y <= 4; ++y)
        for (long x = 0; x <= 5; ++x) {
            CHeightMapCell& c = hm.GetCellIncludingExtraCells(x, y);
            if (InArea(x, y)) {
                float want = brush.GetGameHeight(x, y);
                if (want < 0) want = 0;
                if (want > limit) want = limit;
                CHECK(c.Height == want);
                for (unsigned long l = 0; l < 3; ++l) {
                    unsigned long t = brush.GetTheme(x, y, l);
                    CHECK(c.EngineThemePaletteIndex[l] == (t ? (unsigned char)(t + 40) : 0));
                }
                CHECK(c.EngineBlend[0] == brush.GetThemeBlend(x, y, 0));
                CHECK(c.EngineBlend[1] == brush.GetThemeBlend(x, y, 1));
            } else {
                CHECK(c.Height == 100.0f);
                CHECK(c.EngineThemePaletteIndex[0] == 0xEE && c.EngineBlend[1] == 0xDD);
            }
        }
    CHECK(g_orderCount == 3 && g_order[0] == 'H' && g_order[1] == 'T' && g_order[2] == 'B');
    CHECK(g_updates == 1 && g_key == map.RegionKey);
    CHECK(g_updated.TLX == 700 && g_updated.TLY == 700 && g_updated.BRX == 704 && g_updated.BRY == 703);

    // amount != 1: radial blend path
    Reset(hm);
    float amount = 0.5f;
    map.ApplyScriptedMapBrushUpdate(amount, &brush);
    long cx = 4 - (4 - 1) / 2, cy = 3 - (3 - 1) / 2;
    double inv = 1.0 / sqrt((double)((cx - 4) * (cx - 4) + (cy - 3) * (cy - 3)));
    for (long y = 0; y <= 4; ++y)
        for (long x = 0; x <= 5; ++x) {
            CHeightMapCell& c = hm.GetCellIncludingExtraCells(x, y);
            if (InArea(x, y)) {
                double d = sqrt((double)((x - cx) * (x - cx) + (y - cy) * (y - cy))) * inv;
                double f = (1.0 - d) * (1.0 - d);
                double w = f * amount * pow(amount, 3.0) + pow(amount, 4.0);
                if (w > 1.0) w = 1.0;
                double h = brush.GetGameHeight(x, y) * w + (1.0 - w) * 100.0;
                if (h < 0) h = 0;
                if (h > limit) h = limit;
                CHECK(Abs((float)(c.Height - h)) < 1e-2f);
                CHECK(c.EngineThemePaletteIndex[0] == 0xEE && c.EngineBlend[0] == 0xDD);
            } else
                CHECK(c.Height == 100.0f);
        }
    CHECK(g_orderCount == 0);
    CHECK(g_updates == 1 && g_updated.TLX == 700 && g_updated.BRY == 703);
    CHECK(g_areaCalls == 2);

    char line[64];
    _snprintf(line, sizeof(line), "008203a0_TEST %s", g_fail ? "FAIL" : "PASS");
    printf("%s\n", line);
    return g_fail ? 1 : 0;
}