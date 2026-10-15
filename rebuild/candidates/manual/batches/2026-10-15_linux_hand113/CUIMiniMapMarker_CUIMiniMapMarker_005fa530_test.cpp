#include <cstdio>
// Behaviour test for CUIMiniMapMarker::CUIMiniMapMarker() (0x005FA530): everything zero, empty def name.
struct CCharString { const char* p; long n; CCharString(const char* t, long len) : p(t), n(len) { } };
struct C2DVector { float X, Y; };
struct CUIMiniMapMarker {
    long Region; CCharString UIDefName; C2DVector Position; long DefNameLength; long GraphicIndex; float Angle;
    CUIMiniMapMarker() : Region(0), UIDefName("", -1) { Position.X = 0; Position.Y = 0; DefNameLength = 0; GraphicIndex = 0; Angle = 0; }
};
int main()
{
    CUIMiniMapMarker m;
    if (m.Region != 0 || m.UIDefName.p[0] != 0 || m.Position.X != 0.0f || m.Position.Y != 0.0f || m.DefNameLength != 0 || m.GraphicIndex != 0 || m.Angle != 0.0f) { std::printf("005fa530_TEST FAIL\n"); return 1; }
    std::printf("005fa530_TEST PASS\n");
    return 0;
}
