// Behaviour test for retail 0x004ff570: candidate source embedded verbatim, then a two-map world with a 3x2 passability
// pattern, a recording 3D engine, and a main that checks the overlay calls and the colour grid.
#include <stdio.h>
#include <stdlib.h>
// CWorldMap::ShowPassabilityOverlay(bool show) -- for every loaded map (from 1), switch the 3D engine's overlay for that map on or off; when on, build a width x height colour grid (green-ish where CMap::IsPassableAt, red elsewhere) and hand it to the engine
// Leaf: ShowPassabilityOverlay
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FF570, 475 bytes (+ int3 padding), `ret 4`, 8-aligned frame (`and esp,-8`).
//   Manifest signature CWorldMap::ShowPassabilityOverlay(bool).
// Callees: CWorldMap virtual slots 10 IsMapLoaded(long) const (+0x28, 0x004FC050) and 18 GetMap(long) (+0x48,
//   0x0051CE20); CMap 0x0081E690 (manifest GetWorldMapNumber: [+0x20]->slot 13->slot 17(&WorldPos), returns the engine's
//   map handle), CMap::IsPassableAt(const C2DCoordI&) const 0x0081F3D0, vector<CRGBColour>(size_type) 0x005179C0
//   (manifest "std_vector_ulong_Initialize"), CRT free 0x00BFEA14; the 3D engine (g_pMainGameComponent 0x013B86A0
//   -> PDisplayEngine +0x28 -> Engine3D +0x2C) slots 64 (+0x100, overlay data) and 65 (+0x104, overlay enable).
// Shape: the blocked colour is declared first (its bytes are stored first, passable sits below it); the overlay box
//   uses a four-long ctor built before the colour vector (all four stores share one `xor eax,eax`); GetData is
//   `size() > 0 ? start : (T*)this` (sar + ja).
// Layout: local models at retail offsets: CWorldMap Maps (vector<CMapInfo>, 0x48-byte elements) +0x20; CMap WorldSizeX
//   +0x1D0C / WorldSizeY +0x1D10 (as the landed CMap::CreateScriptedMapBrush 0x00820F80); CRGBColour B,G,R,A bytes
//   (as the landed CPSCLight 0x00B1A640). engine/ headers carry the PDB's 16-byte containers.
extern "C" void free(void* block);                               // 0x00BFEA14

class C2DCoordI { public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
class C2DBoxI { public:
    C2DCoordI TopLeft, BottomRight;
    C2DBoxI(long x0, long y0, long x1, long y1) : TopLeft(x0, y0), BottomRight(x1, y1) { }
};
class CRGBColour { public:                                              // B,G,R,A bytes
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CColourVector {                                           // vector<CRGBColour> (STLport, 12 bytes)
    CRGBColour* _M_start;
    CRGBColour* _M_finish;
    CRGBColour* _M_end_of_storage;
    explicit CColourVector(unsigned long count);                 // 0x005179C0
    ~CColourVector() { if (_M_start != 0) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    CRGBColour& operator[](unsigned long n) { return *(_M_start + n); }
    CRGBColour* GetData() { return size() > 0 ? _M_start : (CRGBColour*)this; }
};
class CMapInfo { public: unsigned char _body[0x48]; };
struct CMapInfoVector {
    CMapInfo* _M_start;
    CMapInfo* _M_finish;
    CMapInfo* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
};
class CMap { public:
    unsigned char _pad[0x1d0c];
    long WorldSizeX;                                             // +0x1D0C
    long WorldSizeY;                                             // +0x1D10
    long GetWorldMapNumber() const;                              // 0x0081E690
    bool IsPassableAt(const C2DCoordI& pos) const;               // 0x0081F3D0
};
class CIEngine { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void SetMapOverlay(long map, const CRGBColour* colours, const C2DBoxI& area);   // slot 64 (+0x100)
    virtual void EnableMapOverlay(long map, bool enable, bool b);                          // slot 65 (+0x104)
};
class CDisplayEngine { public:
    unsigned char _pad[0x2c];
    CIEngine* Engine3D;                                          // +0x2C
};
class CMainGameComponent { public:
    unsigned char _pad[0x28];
    CDisplayEngine* PDisplayEngine;                              // +0x28
};
extern CMainGameComponent* g_pMainGameComponent;                 // 0x013B86A0

class CWorldMap { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual bool IsMapLoaded(long map) const;                    // slot 10 (+0x28) 0x004FC050
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void v17();
    virtual CMap& GetMap(long map);                              // slot 18 (+0x48) 0x0051CE20
    unsigned char _pad04[0x20 - 4];
    CMapInfoVector Maps;                                         // +0x20
    void ShowPassabilityOverlay(bool show);
};

void CWorldMap::ShowPassabilityOverlay(bool show)
{
    CRGBColour blocked(0xff, 0, 0, 0x40);
    CRGBColour passable(0x35, 0x78, 0x18, 0x40);
    for (long i = 1; i < (long)Maps.size(); ++i) {
        if (!IsMapLoaded(i))
            continue;
        CMap& map = GetMap(i);
        CIEngine* engine = g_pMainGameComponent->PDisplayEngine->Engine3D;
        engine->EnableMapOverlay(map.GetWorldMapNumber(), show, true);
        if (!show)
            continue;
        long width = map.WorldSizeX;
        long height = map.WorldSizeY;
        C2DBoxI area(0, 0, width, height);
        CColourVector colours(width * height);
        for (long y = 0; y < height; ++y)
            for (long x = 0; x < width; ++x) {
                if (map.IsPassableAt(C2DCoordI(x, y)))
                    colours[y * width + x] = passable;
                else
                    colours[y * width + x] = blocked;
            }
        engine->SetMapOverlay(map.GetWorldMapNumber(), colours.GetData(), area);
    }
}

// ---- stubs ----
void CWorldMap::v00() { }
void CWorldMap::v01() { }
void CWorldMap::v02() { }
void CWorldMap::v03() { }
void CWorldMap::v04() { }
void CWorldMap::v05() { }
void CWorldMap::v06() { }
void CWorldMap::v07() { }
void CWorldMap::v08() { }
void CWorldMap::v09() { }
void CWorldMap::v11() { }
void CWorldMap::v12() { }
void CWorldMap::v13() { }
void CWorldMap::v14() { }
void CWorldMap::v15() { }
void CWorldMap::v16() { }
void CWorldMap::v17() { }
void CIEngine::v00() { }
void CIEngine::v01() { }
void CIEngine::v02() { }
void CIEngine::v03() { }
void CIEngine::v04() { }
void CIEngine::v05() { }
void CIEngine::v06() { }
void CIEngine::v07() { }
void CIEngine::v08() { }
void CIEngine::v09() { }
void CIEngine::v10() { }
void CIEngine::v11() { }
void CIEngine::v12() { }
void CIEngine::v13() { }
void CIEngine::v14() { }
void CIEngine::v15() { }
void CIEngine::v16() { }
void CIEngine::v17() { }
void CIEngine::v18() { }
void CIEngine::v19() { }
void CIEngine::v20() { }
void CIEngine::v21() { }
void CIEngine::v22() { }
void CIEngine::v23() { }
void CIEngine::v24() { }
void CIEngine::v25() { }
void CIEngine::v26() { }
void CIEngine::v27() { }
void CIEngine::v28() { }
void CIEngine::v29() { }
void CIEngine::v30() { }
void CIEngine::v31() { }
void CIEngine::v32() { }
void CIEngine::v33() { }
void CIEngine::v34() { }
void CIEngine::v35() { }
void CIEngine::v36() { }
void CIEngine::v37() { }
void CIEngine::v38() { }
void CIEngine::v39() { }
void CIEngine::v40() { }
void CIEngine::v41() { }
void CIEngine::v42() { }
void CIEngine::v43() { }
void CIEngine::v44() { }
void CIEngine::v45() { }
void CIEngine::v46() { }
void CIEngine::v47() { }
void CIEngine::v48() { }
void CIEngine::v49() { }
void CIEngine::v50() { }
void CIEngine::v51() { }
void CIEngine::v52() { }
void CIEngine::v53() { }
void CIEngine::v54() { }
void CIEngine::v55() { }
void CIEngine::v56() { }
void CIEngine::v57() { }
void CIEngine::v58() { }
void CIEngine::v59() { }
void CIEngine::v60() { }
void CIEngine::v61() { }
void CIEngine::v62() { }
void CIEngine::v63() { }

static CMap g_maps[3];
static bool g_loaded[3] = { true, false, true };
static int g_enableCalls = 0, g_dataCalls = 0;
static long g_lastEnableMap = -1; static bool g_lastEnable = false, g_lastFlag = false;
static long g_lastDataMap = -1; static long g_box[4];
static unsigned char g_grid[64][4]; static unsigned long g_gridCount = 0;

CColourVector::CColourVector(unsigned long count)
{
    _M_start = (CRGBColour*)malloc((count ? count : 1) * sizeof(CRGBColour));
    for (unsigned long i = 0; i < count; ++i) { _M_start[i].R = 1; _M_start[i].G = 2; _M_start[i].B = 3; _M_start[i].A = 4; }
    _M_finish = _M_end_of_storage = _M_start + count;
}
long CMap::GetWorldMapNumber() const { return 100 + (long)(this - g_maps); }
bool CMap::IsPassableAt(const C2DCoordI& pos) const { return ((pos.X + pos.Y) & 1) == 0; }
bool CWorldMap::IsMapLoaded(long map) const { return g_loaded[map]; }
CMap& CWorldMap::GetMap(long map) { return g_maps[map]; }
void CIEngine::EnableMapOverlay(long map, bool enable, bool b)
{
    ++g_enableCalls; g_lastEnableMap = map; g_lastEnable = enable; g_lastFlag = b;
}
void CIEngine::SetMapOverlay(long map, const CRGBColour* colours, const C2DBoxI& area)
{
    ++g_dataCalls; g_lastDataMap = map;
    g_box[0] = area.TopLeft.X; g_box[1] = area.TopLeft.Y; g_box[2] = area.BottomRight.X; g_box[3] = area.BottomRight.Y;
    g_gridCount = (unsigned long)(area.BottomRight.X * area.BottomRight.Y);
    for (unsigned long i = 0; i < g_gridCount && i < 64; ++i) {
        g_grid[i][0] = colours[i].R; g_grid[i][1] = colours[i].G; g_grid[i][2] = colours[i].B; g_grid[i][3] = colours[i].A;
    }
}
struct CTestEngine : CIEngine { };
static CTestEngine g_engine;
static CDisplayEngine g_display;
static CMainGameComponent g_component;
CMainGameComponent* g_pMainGameComponent = &g_component;

struct CTestWorldMap : CWorldMap { };

int main()
{
    int ok = 1;
    g_display.Engine3D = &g_engine;
    g_component.PDisplayEngine = &g_display;
    for (int m = 0; m < 3; ++m) { g_maps[m].WorldSizeX = 3; g_maps[m].WorldSizeY = 2; }
    CTestWorldMap world;
    CMapInfo infos[3];
    world.Maps._M_start = infos; world.Maps._M_finish = infos + 3; world.Maps._M_end_of_storage = infos + 3;

    world.ShowPassabilityOverlay(false);          // map 0 skipped, map 1 unloaded, map 2 switched off, no data
    if (g_enableCalls != 1 || g_lastEnableMap != 102 || g_lastEnable || !g_lastFlag || g_dataCalls != 0) ok = 0;

    g_enableCalls = 0;
    world.ShowPassabilityOverlay(true);           // map 2 switched on and given a 3x2 grid
    if (g_enableCalls != 1 || g_lastEnableMap != 102 || !g_lastEnable || !g_lastFlag) ok = 0;
    if (g_dataCalls != 1 || g_lastDataMap != 102) ok = 0;
    if (g_box[0] != 0 || g_box[1] != 0 || g_box[2] != 3 || g_box[3] != 2 || g_gridCount != 6) ok = 0;
    for (long y = 0; y < 2 && ok; ++y)
        for (long x = 0; x < 3; ++x) {
            const unsigned char* c = g_grid[y * 3 + x];
            bool passable = ((x + y) & 1) == 0;
            if (passable && !(c[0] == 0x35 && c[1] == 0x78 && c[2] == 0x18 && c[3] == 0x40)) ok = 0;
            if (!passable && !(c[0] == 0xff && c[1] == 0 && c[2] == 0 && c[3] == 0x40)) ok = 0;
        }

    g_loaded[2] = false; g_enableCalls = g_dataCalls = 0;
    world.ShowPassabilityOverlay(true);           // nothing loaded beyond map 0
    if (g_enableCalls != 0 || g_dataCalls != 0) ok = 0;

    CMapInfo one[1];
    world.Maps._M_start = one; world.Maps._M_finish = one + 1;
    world.ShowPassabilityOverlay(true);           // only map 0: the loop does not run
    if (g_enableCalls != 0) ok = 0;

    printf(ok ? "004ff570_TEST PASS\n" : "004ff570_TEST FAIL\n");
    return ok ? 0 : 1;
}