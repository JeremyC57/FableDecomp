// Behaviour test for CUIMiniMapMarker::CUIMiniMapMarker (0x00613510): all fields zero, name built from "" with length -1.
#include <cstdio>

static const char* g_lastText = 0;
static long g_lastLength = 0;
class CCharString { public:
    const char* m_text;
    CCharString(const char* text, long length) { m_text = text; g_lastText = text; g_lastLength = length; }
};
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct CUIMiniMapMarker {
    long Region;
    CCharString UIDefName;
    C2DVector Position;
    long DefNameLength;
    long GraphicIndex;
    float Angle;
    CUIMiniMapMarker();
};
CUIMiniMapMarker::CUIMiniMapMarker()
    : Region(0), UIDefName("", -1), Position(0.0f, 0.0f), DefNameLength(0), GraphicIndex(0), Angle(0.0f)
{
}

int main()
{
    CUIMiniMapMarker m;
    if (m.Region != 0 || m.Position.X != 0.0f || m.Position.Y != 0.0f || m.DefNameLength != 0 || m.GraphicIndex != 0
        || m.Angle != 0.0f || g_lastLength != -1 || g_lastText == 0 || g_lastText[0] != 0 || m.UIDefName.m_text != g_lastText) {
        std::printf("00613510_TEST FAIL\n");
        return 1;
    }
    std::printf("00613510_TEST PASS\n");
    return 0;
}