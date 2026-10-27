// CMiniMapDisplay::GetDrawPosition(const C3DVector& pos, const C2DBoxF& box) const -- map a world position to its pixel on the minimap: relative position inside the region map, scaled into the screen box, snapped to whole (rounded, then nudged off exact .5) pixels, offset by the minimap's screen origin
// Leaf: GetDrawPosition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082A020, length 341 bytes (padded to 352), `ret 0Ch`.
// Callees: CRegionMinimap::GetRegionMapExtents @ 0x0066C6A0, PeekGuiDef @ 0x00438570, CWorld::GetWorldMap (vslot 13) / CWorldMap
//   GetLoadedRegion @ 0x004FB150 / GetRegion(long) @ 0x004FC180, CMiniMapDisplay::GetRelativeMapSize @ 0x00829C70.
#pragma optimize("y", off)
// Retail-asm: 0x0082A0D2..0x0082A0EC (`fld [ebp-0Ch] ; fsub [ebp-8] ; fistp dword ptr [ebp+10h] ; fild [ebp+10h] ; fadd [ebp+0Ch] ; fstp [ebp-10h] ; mov eax,[ebp-0Ch] ; cmp eax,[ebp-10h] ; jne ; inc dword ptr [ebp+10h]`, 1.0f and the result homed in the dead `pos` / `box` argument slots) and 0x0082A112..0x0082A12C -- the engine's inline-assembly floor helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; inc dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
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

// the engine's inline-assembly FPU floor helper (0x0048D940 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
// The whole 10-instruction block is asm (the bit compare and the memory increment included: rasm1 family (b)); the result
// is read next by C++ after the block.
__forceinline long FloorToLong(float value)
{
    long  result;
    float one;
    float half;
    float next;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fsub  half
        fistp result
        fild  result
        fadd  one
        fstp  next
        mov   eax, value
        cmp   eax, next
        jne   done
        inc   result
    done:
    }
    return result;
}

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
    float minX = box.Min.X;
    float minY = box.Min.Y;
    float w = (box.Max.X - minX) * region.Zoom + minX;
    float h = (box.Max.Y - minY) * region.Zoom + minY;
    float hm = h - minY;
    long iy = FloorToLong((hm + minY) - hm * ry * size.Y);
    float wm = w - minX;
    long ix = FloorToLong(wm * rx * size.X + minX);
    return C2DVector(ix + (g_ScreenOriginX + region.OffsetX) / (g_MiniMapScale / wm),
                     iy + (g_ScreenOriginY + region.OffsetY) / (g_MiniMapScale / hm));
}
