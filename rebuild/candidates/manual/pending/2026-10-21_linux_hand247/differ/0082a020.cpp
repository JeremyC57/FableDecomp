// Grade: DIFFER -- lane gap: retail rounds both coordinates with a bare dword fistp through dead argument slots (the inline-asm rounding helper of GOTCHAS chunk 145); this draft's arithmetic is approximate and was graded once
// CMiniMapDisplay::GetDrawPosition(const C3DVector& pos, const C2DBoxF& box) const -- map a world position to its pixel on the minimap: relative position inside the region map, scaled into the screen box, snapped to whole (rounded, then nudged off exact .5) pixels, offset by the minimap's screen origin
// Leaf: GetDrawPosition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082A020, length 341 bytes (padded to 352), `ret 0Ch`.
// Callees: CRegionMinimap::GetRegionMapExtents @ 0x0066C6A0, PeekGuiDef @ 0x00438570, CWorld::GetWorldMap (vslot 13) / CWorldMap
//   GetLoadedRegion @ 0x004FB150 / GetRegion(long) @ 0x004FC180, CMiniMapDisplay::GetRelativeMapSize @ 0x00829C70.
struct C2DCoordI { long X, Y; };
struct C2DExtentsI { long Width, Height; };
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C3DVector { float X, Y, Z; };
struct C2DBoxF { C2DVector Min, Max; };
struct CRegionMinimap {
    void GetRegionMapExtents(C2DCoordI& origin, C2DExtentsI& extents) const;   // 0x0066C6A0
};
struct CRegion {
    unsigned char _pad00[0x2c];
    float Zoom;                                  // +0x2c
    float OffsetX;                               // +0x30
    float OffsetY;                               // +0x34
};
struct CWorldMap {
    long GetLoadedRegion() const;                // 0x004FB150
    CRegion& GetRegion(long index) const;        // 0x004FC180
};
struct CWorld {
    unsigned char _pad00[0x24];
    struct CWorldHolder { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
        virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8(); virtual void v9(); virtual void v10();
        virtual void v11(); virtual void v12(); virtual CWorldMap& GetWorldMap(); }* Holder;   // +0x24
};
extern CWorld* g_pWorld;                         // 0x013B86A0
struct CPlayerGuiDef;
CPlayerGuiDef* __fastcall PeekGuiDef();          // 0x00438570
extern long g_ScreenOriginX;                     // 0x013B83B8
extern long g_ScreenOriginY;                     // 0x013B83BC
extern float g_MiniMapScale;                     // 0x0125B240 (256.0f)

class CMiniMapDisplay {
public:
    unsigned char  _pad00[0x54];
    CRegionMinimap RegionMinimap;                // +0x54
    void GetRelativeMapSize(C2DVector& size) const;   // 0x00829C70
    C2DVector GetDrawPosition(const C3DVector& pos, const C2DBoxF& box) const;
};

static long RoundToLong(float f) { return (long)f; }

C2DVector CMiniMapDisplay::GetDrawPosition(const C3DVector& pos, const C2DBoxF& box) const
{
    C2DCoordI origin;
    C2DExtentsI extents;
    RegionMinimap.GetRegionMapExtents(origin, extents);
    float rx = (pos.X - origin.X) / extents.Width;
    float ry = (pos.Y - origin.Y) / extents.Height;
    PeekGuiDef();
    CWorldMap& map = g_pWorld->Holder->GetWorldMap();
    CRegion& region = map.GetRegion(map.GetLoadedRegion());
    C2DVector size;
    GetRelativeMapSize(size);
    float w = (box.Max.X - box.Min.X) * region.Zoom + box.Min.X;
    float h = (box.Max.Y - box.Min.Y) * region.Zoom + box.Min.Y;
    float py = h - (h - box.Min.Y) * ry * size.Y;
    long iy = RoundToLong(py - 0.5f);
    if (py == iy + 1.0f) ++iy;
    float px = (w - box.Min.X) * rx * size.X + box.Min.X;
    long ix = RoundToLong(px - 0.5f);
    if (px == ix + 1.0f) ++ix;
    return C2DVector(g_ScreenOriginX + region.OffsetX + g_MiniMapScale / w / ix, g_ScreenOriginY + region.OffsetY + g_MiniMapScale / h / iy);
}
