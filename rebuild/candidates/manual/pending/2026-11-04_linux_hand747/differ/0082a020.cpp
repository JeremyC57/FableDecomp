// CMiniMapDisplay::GetDrawPosition(const C3DVector& pos, const C2DBoxF& box) const -- map a world position to its pixel on the minimap: the position relative to the region map, scaled into the zoomed screen box and floored to whole pixels, plus the screen origin and the region's offset divided by the pixels-per-unit scale
// Grade: DIFFER(332v343) -- try a (rewritten) 332v343, frame-masked dl=31: x87 schedule of the zoomed box (retail keeps MinY on the stack and reuses height) and the final sums (fild of both ints first); frame layout (best try a; the bare-dword fistp itself is reproduced by the shared helper, see NOTES.md)
// Leaf: ?GetDrawPosition@CMiniMapDisplay@@QBE?AVC2DVector@@ABVC3DVector@@ABVC2DBoxF@@@Z
// Flags: none
// Retail-asm: 0x0082A0D8 `fistp dword ptr [ebp+10h]` and 0x0082A118 `fistp dword ptr [ebp+0Ch]` -- each pixel coordinate floored by the
//   engine's inline-assembly GFFloatToLongFloor (shared engine/GFFloatToLongFloor.h: 0.5f / 1.0f frame constants in the dead argument
//   slots, bare dword fistp, fild/fadd, integer compare, `inc` on memory). VC7.1 emits no bare dword fistp from C++ (NOTES.md "C++
//   attempts before helper"). This row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082A020, 341 bytes (padded to 352), thiscall `ret 0Ch` (hidden result), ebp frame.
// Identity: Ego_r.pdb ?GetDrawPosition@CMiniMapDisplay@@QBE?AVC2DVector@@ABVC3DVector@@ABVC2DBoxF@@@Z.
// Callees: CRegionMinimap::GetRegionMapExtents(C2DCoordI&, C2DExtentsI&) const 0x0066C6A0 (Ego_r), CPlayerGui::PeekGuiDef 0x00438570
//   (result unused), CMainGameComponent::PMainGameComponent 0x013B86A0 -> world (+0x24) -> GetWorldMap (slot 13),
//   CWorldMap::GetLoadedRegion 0x004FB150 / GetRegion(long) const 0x004FC180, CMiniMapDisplay::GetRelativeMapSize 0x00829C70 (Ego_r).
// Globals: the minimap screen origin longs 0x013B83B8 / 0x013B83BC (descriptive names, no binding); 256.0f 0x0125B240.
#pragma optimize("y", off)
#include "engine/GFFloatToLongFloor.h"
class C2DCoordI { public: long X, Y; };
class C2DExtentsI { public: long Width, Height; };
class C2DVector { public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
class C3DVector { public: float X, Y, Z; };
class C2DBoxF { public: float X0, Y0, X1, Y1; };
class CRegionMinimap { public:
    void GetRegionMapExtents(C2DCoordI& origin, C2DExtentsI& extents) const;   // 0x0066C6A0
};
class CRegion { public:
    unsigned char _pad00[0x2c];
    float Zoom;                                  // +0x2c
    float OffsetX;                               // +0x30
    float OffsetY;                               // +0x34
};
class CWorldMap { public:
    long GetLoadedRegion() const;                // 0x004FB150
    const CRegion& GetRegion(long index) const;  // 0x004FC180
};
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual CWorldMap* GetWorldMap();            // slot 13 (+0x34)
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }
    unsigned char _pad[0x24]; CWorld* World;      // +0x24
};
class CPlayerGuiDef;
class CPlayerGui { public:
    static const CPlayerGuiDef& __fastcall PeekGuiDef();     // 0x00438570
};
extern long g_MiniMapScreenOriginX;              // 0x013B83B8
extern long g_MiniMapScreenOriginY;              // 0x013B83BC
class CMiniMapDisplay {
public:
    unsigned char  _pad00[0x54];
    CRegionMinimap RegionMinimap;                // +0x54
    void GetRelativeMapSize(C2DVector& size) const;   // 0x00829C70
    C2DVector GetDrawPosition(const C3DVector& pos, const C2DBoxF& box) const;
};
C2DVector CMiniMapDisplay::GetDrawPosition(const C3DVector& pos, const C2DBoxF& box) const
{
    C2DCoordI origin;
    C2DExtentsI extents;
    RegionMinimap.GetRegionMapExtents(origin, extents);
    float rx = (pos.X - origin.X) / extents.Width;
    float ry = (pos.Y - origin.Y) / extents.Height;
    const CPlayerGuiDef& guiDef = CPlayerGui::PeekGuiDef();
    CWorldMap* map = CMainGameComponent::Get()->World->GetWorldMap();
    const CRegion& region = map->GetRegion(map->GetLoadedRegion());
    C2DVector size;
    GetRelativeMapSize(size);
    float minX = box.X0;
    float right = (box.X1 - minX) * region.Zoom + minX;
    float bottom = (box.Y1 - box.Y0) * region.Zoom + box.Y0;
    float height = bottom - box.Y0;
    long iy = GFFloatToLongFloor(bottom - height * ry * size.Y);
    float width = right - minX;
    long ix = GFFloatToLongFloor(width * rx * size.X + minX);
    return C2DVector((g_MiniMapScreenOriginX + region.OffsetX) / (256.0f / width) + ix,
                     (g_MiniMapScreenOriginY + region.OffsetY) / (256.0f / height) + iy);
}
