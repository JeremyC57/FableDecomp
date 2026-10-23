// Grade: DIFFER(522v518) -- x87 register variables: retail keeps the row's x (origin.X) on the FPU stack across the inner loop (fst into the box, spilled to a frame word only around Draw2DBox) and its frame is 0x4c with separate single-cell and grid boxes; ours homes x/y in frame words (0x40 frame) and orders the single-cell box stores differently
// NThingMap::CThingMap::DebugDraw(const C2DVector& origin, long level) -- draws one mapwho level's cell grid as screen
// boxes (occupied cells in the level's first colour, empty ones in its second); a level flagged as a single cell is one
// full-size box coloured by its only cell
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085CBD0, `ret 8`.
// Callees: GFXToY 0x009E23A0 (__fastcall, float on the stack), CRenderManager::Draw2DBox 0x009DE3F0.
// Globals: g_ProgressScreenWidth 0x013961E8 (double), the debug scale 0x01248028 (double), the level count 0x013BA848,
//   the render manager 0x013B8384, the per-level colour pairs 0x0137DAE0 (16 bytes per level).
// Layout: CThingMap MapwhoLevelInfo +0x04 (vector of 20-byte infos: single-cell flag +0, width +4, height +8),
//   Cells +0x14 (vector of vector<CThingMapCell>, 4-byte cells).
struct C2DVector { float X, Y; };
struct C2DBoxF {
    float X0, Y0, X1, Y1;
    C2DBoxF(float x0, float y0, float x1, float y1) : X0(x0), Y0(y0), X1(x1), Y1(y1) {}
};
struct CRGBColour { unsigned char R, G, B, A; };
struct SLevelColours { CRGBColour Occupied; CRGBColour Empty; long _pad[2]; };
struct CRenderManager {
    bool Draw2DBox(const C2DBoxF& box, const CRGBColour& colour, long flags);   // 0x009DE3F0
};
float __fastcall GFXToY(float x);                          // 0x009E23A0
extern double         g_ProgressScreenWidth;               // 0x013961E8
extern const double   g_ThingMapDebugScale;                // 0x01248028
extern long           g_NumMapwhoLevels;                   // 0x013BA848
extern CRenderManager* g_pRenderManager;                   // 0x013B8384
extern SLevelColours  g_MapwhoLevelColours[];              // 0x0137DAE0
namespace NThingMap {
struct CMapwhoLevelInfo {
    long SingleCell;                                       // +0x00 (-1 = one cell)
    long Width;                                            // +0x04
    long Height;                                           // +0x08
    long _pad[2];
};
struct CThingMapCell { void* First; };
struct CCellVector { CThingMapCell* _M_start; CThingMapCell* _M_finish; CThingMapCell* _M_end_of_storage; };
struct CThingMap {
    void*             _vfptr;
    CMapwhoLevelInfo* LevelInfo;                           // +0x04 (vector begin)
    CMapwhoLevelInfo* LevelInfoEnd;
    CMapwhoLevelInfo* LevelInfoCap;
    void*             _pad10;
    CCellVector*      Cells;                               // +0x14 (vector begin)
    CCellVector*      CellsEnd;
    CCellVector*      CellsCap;
    const CThingMapCell& GetCell(unsigned char level, long x, long y) const
    {
        return Cells[level]._M_start[y * LevelInfo[level].Width + x];
    }
    void DebugDraw(const C2DVector& origin, long level);
};
}
void NThingMap::CThingMap::DebugDraw(const C2DVector& origin, long level)
{
    float size = (float)(g_ProgressScreenWidth * g_ThingMapDebugScale);
    if (level < 0 || level >= g_NumMapwhoLevels)
        return;
    float cellWidth = size / (float)LevelInfo[level].Width;
    float cellHeight = GFXToY(cellWidth);
    float x = origin.X;
    float y = origin.Y;
    if (LevelInfo[level].SingleCell == -1) {
        C2DBoxF box(0.0f, 0.0f, size, size);
        if (GetCell((unsigned char)level, 0, 0).First)
            g_pRenderManager->Draw2DBox(box, g_MapwhoLevelColours[level].Occupied, 0);
        else
            g_pRenderManager->Draw2DBox(box, g_MapwhoLevelColours[level].Empty, 0);
        return;
    }
    for (long row = 0; row < LevelInfo[level].Height; ++row) {
        for (long x0 = 0; x0 < LevelInfo[level].Width; ++x0) {
            if (GetCell((unsigned char)level, x0, row).First) {
                C2DBoxF box(x, y, x + cellWidth, y + cellHeight);
                x = x + cellWidth;
                g_pRenderManager->Draw2DBox(box, g_MapwhoLevelColours[level].Occupied, 0);
            } else {
                C2DBoxF box(x, y, x + cellWidth, y + cellHeight);
                x = x + cellWidth;
                g_pRenderManager->Draw2DBox(box, g_MapwhoLevelColours[level].Empty, 0);
            }
        }
        y += cellHeight;
        x = origin.X;
    }
}
