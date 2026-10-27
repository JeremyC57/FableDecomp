// CThingMapManager::DebugDraw(const C2DCoordI& pos, long layer) -- debug-draw the thing map under the position and, on alternate half-cycles of the debug frame counter, a white marker box at the position
// Grade: DIFFER(288v288) -- the map-local coordinate subtractions are scheduled X first (retail Y first) and the marker colour bytes / rect pushes are interleaved differently; frame and calls match
// Leaf: DebugDraw
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00637C80, length 288 bytes, `ret 8`.
// Callees: CThingMapManager::GetThingMapAt(const C2DCoordI&) @ 0x00637A80, NThingMap::CThingMap::DebugDraw(const C2DVector&,
// long) @ 0x0085CBD0, GFAXToVX(float) @ 0x009E1DA0 / GFAYToVY(float) @ 0x009E1DD0 (`__fastcall`, float on the stack), the
// debug box draw @ 0x009DE3F0 on the display object @ 0x013B8384 (box, colour, 0).
// Globals: the two debug-origin doubles @ 0x013961E8 / 0x013961F0 (scaled by the 0.05 double @ 0x012388A8), the debug
// frame counter @ 0x013BA850.
// Layout: NThingMap::CThingMap Area (C2DBoxI) +0x20 (the landed Clear 0x0085CEC0 model).
// Shape: the screen origin is a C2DVector of the two scaled doubles; the marker is a C2DBoxF of +-3 virtual pixels
// around the converted map-local position, coloured by a CRGBColour local (Set order R, G, B, A).
struct C2DCoordI { long X, Y; };
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    C2DBoxF(float tlx, float tly, float brx, float bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { Set(r, g, b, a); }
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
namespace NThingMap {
struct C2DBoxI { long X0, Y0, X1, Y1; };
struct CThingMap {
    unsigned char _pad00[0x20];
    C2DBoxI       Area;                                        // +0x20
    void DebugDraw(const C2DVector& origin, long layer);       // 0x0085CBD0
};
}
class CDebugDisplay {
public:
    void DrawBox(const C2DBoxF& box, const CRGBColour& colour, long z);   // 0x009DE3F0
};
extern CDebugDisplay* g_pDebugDisplay;                         // 0x013B8384
extern double g_DebugMapOriginX;                               // 0x013961E8
extern double g_DebugMapOriginY;                               // 0x013961F0
extern const double g_DebugMapOriginScale;                     // 0x012388A8 (0.05)
extern long g_DebugDrawFrame;                                  // 0x013BA850
float __fastcall GFAXToVX(float x);                            // 0x009E1DA0
float __fastcall GFAYToVY(float y);                            // 0x009E1DD0

class CThingMapManager {
public:
    NThingMap::CThingMap* GetThingMapAt(const C2DCoordI& pos);   // 0x00637A80
    void DebugDraw(const C2DCoordI& pos, long layer);
};

void CThingMapManager::DebugDraw(const C2DCoordI& pos, long layer)
{
    NThingMap::CThingMap* map = GetThingMapAt(pos);
    C2DVector origin;
    origin.X = (float)(g_DebugMapOriginX * g_DebugMapOriginScale);
    origin.Y = (float)(g_DebugMapOriginY * g_DebugMapOriginScale);
    if (map) {
        map->DebugDraw(origin, layer);
        if (g_DebugDrawFrame % 16 > 8) {
            float halfWidth = GFAXToVX(3.0f);
            float halfHeight = GFAYToVY(3.0f);
            C2DCoordI local;
            local.X = pos.X - map->Area.X0;
            local.Y = pos.Y - map->Area.Y0;
            float y = GFAYToVY((float)local.Y) + origin.Y;
            float x = GFAXToVX((float)local.X) + origin.X;
            CRGBColour white(0xff, 0xff, 0xff, 0xff);
            g_pDebugDisplay->DrawBox(C2DBoxF(x - halfWidth, y - halfHeight, x + halfWidth, y + halfHeight), white, 0);
        }
        ++g_DebugDrawFrame;
    }
}
