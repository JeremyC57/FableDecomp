#include <cstdio>
// Behaviour test for CUIMiniMapMarker::CUIMiniMapMarker(const CUIMiniMapMarker&) (0x00612980).
static int g_copies = 0;
struct CCharString { const char* p; CCharString(const char* t) : p(t) { } CCharString(const CCharString& o) : p(o.p) { ++g_copies; } };
struct C2DVector { float X, Y; };
struct CUIMiniMapMarker {
    long Region; CCharString UIDefName; C2DVector Position; long DefNameLength; long GraphicIndex; float Angle;
    CUIMiniMapMarker(const char* n) : UIDefName(n) { }
    CUIMiniMapMarker(const CUIMiniMapMarker& o)
        : Region(o.Region), UIDefName(o.UIDefName), Position(o.Position), DefNameLength(o.DefNameLength), GraphicIndex(o.GraphicIndex), Angle(o.Angle) { }
};
int main()
{
    CUIMiniMapMarker a("MARKER");
    a.Region = 4; a.Position.X = 1.5f; a.Position.Y = -2.0f; a.DefNameLength = 6; a.GraphicIndex = 9; a.Angle = 0.25f;
    CUIMiniMapMarker b(a);
    if (b.Region != 4 || b.UIDefName.p != a.UIDefName.p || g_copies != 1 || b.Position.X != 1.5f || b.Position.Y != -2.0f ||
        b.DefNameLength != 6 || b.GraphicIndex != 9 || b.Angle != 0.25f) { std::printf("00612980_TEST FAIL\n"); return 1; }
    std::printf("00612980_TEST PASS\n");
    return 0;
}