// NThingMap::CThingMapManager::DebugDraw(const C2DCoordI& pos, long layer) const -- debug-draw the thing map under the position and, on alternate half-cycles of the debug frame counter, a white marker box at the position
// Grade: DIFFER(288v288) -- wave 25 (was 38 lines, now 8): `pos - map->Area.GetTLPos()` through C2DCoordI's by-value operator- fixed the Y-first subtraction and the colour-byte/push interleave; left: retail loads Area.TLY then pos.Y then TLX then pos.X (eax/esi = TLY/TLX), ours loads TLX and TLY first (eax/esi = TLX/TLY). Tried: explicit temp C2DCoordI(TLX, TLY), GetTLPos via GetTLX/GetTLY, body-assignment ctor, free operator-, PDB names (NThingMap namespace, const method): all 8 lines. Lane gap: two-load scheduling of the inlined temporary.
// Leaf: ?DebugDraw@CThingMapManager@NThingMap@@QBEXABVC2DCoordI@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00637C80, 288 bytes, `ret 8`. Ego_r.pdb
//   ?DebugDraw@CThingMapManager@NThingMap@@QBEXABVC2DCoordI@@J@Z (const; the catalog's "CThingMapManager::DebugDraw" lacked
//   the namespace and the const).
// Callees: NThingMap::CThingMapManager::GetThingMapAt(const C2DCoordI&) const @ 0x00637A80, NThingMap::CThingMap::DebugDraw(
//   const C2DVector&, long) @ 0x0085CBD0, GFAXToVX(float) @ 0x009E1DA0 / GFAYToVY(float) @ 0x009E1DD0 (__fastcall, float on the
//   stack), CRenderManager2D::Draw2DBox(const C2DBoxF&, const CRGBColour&, unsigned long) @ 0x009DE3F0 on g_pRenderManager2D
//   @ 0x013B8384.
// Globals: the virtual screen size doubles g_VirtualWidth @ 0x013961E8 / g_VirtualHeight @ 0x013961F0 (landed spelling),
//   scaled by the 0.05 double literal @ 0x012388A8; the debug frame counter @ 0x013BA850 (descriptive).
// Layout: engine/CThingMapManager.h for `this` (no member is read here); NThingMap::CThingMap (retail STLport layout: vptr,
//   MapwhoLevelInfo vector +0x04, Manager +0x10, Cells vector +0x14) Area (C2DBoxI) +0x20, as the landed 0x00637B10 / 0x0085CEC0.
// Shape: the map-local position is `pos - map->Area.GetTLPos()` through C2DCoordI's by-value operator- (its result ctor
//   evaluates Y first, which also lets the colour bytes and the box pushes interleave as retail); the screen origin is a
//   C2DVector of the two scaled doubles; the marker is a C2DBoxF of +-3 virtual pixels around the converted position,
//   coloured by a CRGBColour local (Set order R, G, B, A).
#define CThingMapManager CThingMapManagerLayout
#include "engine/CThingMapManager.h"
#undef CThingMapManager

class C2DCoordI {
public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
    C2DCoordI operator-(const C2DCoordI& o) const { return C2DCoordI(X - o.X, Y - o.Y); }
};
class C2DVector {
public:
    float X, Y;
    C2DVector() { }
};
class C2DBoxF {
public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF(float tlx, float tly, float brx, float bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};
class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { Set(r, g, b, a); }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;
    C2DCoordI GetTLPos() const { return C2DCoordI(TLX, TLY); }
};
class CRenderManager2D {
public:
    bool Draw2DBox(const C2DBoxF& box, const CRGBColour& colour, unsigned long z);   // 0x009DE3F0
};
extern CRenderManager2D* g_pRenderManager2D;                   // 0x013B8384
extern double g_VirtualWidth;                                  // 0x013961E8
extern double g_VirtualHeight;                                 // 0x013961F0
extern long g_ThingMapDebugDrawFrame;                          // 0x013BA850 (descriptive)
float __fastcall GFAXToVX(float x);                            // 0x009E1DA0
float __fastcall GFAYToVY(float y);                            // 0x009E1DD0

namespace NThingMap {
class CThingMap {
public:
    virtual ~CThingMap();
    unsigned char _pad04[0x20 - 0x04];                         // MapwhoLevelInfo, Manager, Cells
    C2DBoxI       Area;                                        // +0x20
    void DebugDraw(const C2DVector& origin, long layer);       // 0x0085CBD0
};
class CThingMapManager : public CThingMapManagerLayout {
public:
    CThingMap* GetThingMapAt(const C2DCoordI& pos) const;      // 0x00637A80
    void DebugDraw(const C2DCoordI& pos, long layer) const;
};
}

void NThingMap::CThingMapManager::DebugDraw(const C2DCoordI& pos, long layer) const
{
    CThingMap* map = GetThingMapAt(pos);
    C2DVector origin;
    origin.X = (float)(g_VirtualWidth * 0.05);
    origin.Y = (float)(g_VirtualHeight * 0.05);
    if (map) {
        map->DebugDraw(origin, layer);
        if (g_ThingMapDebugDrawFrame % 16 > 8) {
            float halfWidth = GFAXToVX(3.0f);
            float halfHeight = GFAYToVY(3.0f);
            C2DCoordI local = pos - map->Area.GetTLPos();
            float y = GFAYToVY((float)local.Y) + origin.Y;
            float x = GFAXToVX((float)local.X) + origin.X;
            CRGBColour white(0xff, 0xff, 0xff, 0xff);
            g_pRenderManager2D->Draw2DBox(C2DBoxF(x - halfWidth, y - halfHeight, x + halfWidth, y + halfHeight), white, 0);
        }
        ++g_ThingMapDebugDrawFrame;
    }
}
