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

struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
struct C2DBoxI {
    C2DCoordI TopLeft, BottomRight;
    C2DBoxI(long x0, long y0, long x1, long y1) : TopLeft(x0, y0), BottomRight(x1, y1) { }
};
struct CRGBColour {                                              // B,G,R,A bytes
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
struct CMapInfo { unsigned char _body[0x48]; };
struct CMapInfoVector {
    CMapInfo* _M_start;
    CMapInfo* _M_finish;
    CMapInfo* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
};
struct CMap {
    unsigned char _pad[0x1d0c];
    long WorldSizeX;                                             // +0x1D0C
    long WorldSizeY;                                             // +0x1D10
    long GetWorldMapNumber() const;                              // 0x0081E690
    bool IsPassableAt(const C2DCoordI& pos) const;               // 0x0081F3D0
};
struct CIEngine {
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
struct CDisplayEngine {
    unsigned char _pad[0x2c];
    CIEngine* Engine3D;                                          // +0x2C
};
struct CMainGameComponent {
    unsigned char _pad[0x28];
    CDisplayEngine* PDisplayEngine;                              // +0x28
};
extern CMainGameComponent* g_pMainGameComponent;                 // 0x013B86A0

struct CWorldMap {
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
