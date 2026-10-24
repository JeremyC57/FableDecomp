// Behaviour test for 0052e530: a copy of the source's model and logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_fail = 0;
static void Check(bool ok, int line)
{
    if (!ok) {
        g_fail = 1;
        std::printf("check failed at line %d\n", line);
    }
}
#define CHECK(c) Check((c) ? true : false, __LINE__)


class C2DVector {
public:
    float X;
    float Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
};
class CUIScreenUnitConverter {
public:
    static bool  IsUsingRelativeCoords;
    static float BackBufferWidth;
    static float BackBufferHeight;
    static float ScreenWidth;
    static float ScreenHeight;
    static C2DVector UnConvertVector(C2DVector v)
    {
        C2DVector result = v;
        if (IsUsingRelativeCoords) {
            result.X = v.X / BackBufferWidth;
            result.X *= ScreenWidth;
            result.Y = v.Y / BackBufferHeight;
            result.Y *= ScreenHeight;
        }
        return result;
    }
};
bool  CUIScreenUnitConverter::IsUsingRelativeCoords = false;
float CUIScreenUnitConverter::BackBufferWidth = 1280.0f;
float CUIScreenUnitConverter::BackBufferHeight = 960.0f;
float CUIScreenUnitConverter::ScreenWidth = 640.0f;
float CUIScreenUnitConverter::ScreenHeight = 480.0f;
static void RunTest()
{
    C2DVector v(640.0f, 240.0f);
    C2DVector r = CUIScreenUnitConverter::UnConvertVector(v);
    CHECK(r.X == v.X && r.Y == v.Y);                    // absolute coordinates: unchanged
    CUIScreenUnitConverter::IsUsingRelativeCoords = true;
    r = CUIScreenUnitConverter::UnConvertVector(v);
    CHECK(r.X == 320.0f && r.Y == 120.0f);
    CUIScreenUnitConverter::BackBufferWidth = 800.0f;   // 800x600 back buffer
    CUIScreenUnitConverter::BackBufferHeight = 600.0f;
    r = CUIScreenUnitConverter::UnConvertVector(C2DVector(80.0f, 60.0f));
    CHECK(r.X == 64.0f && r.Y == 48.0f);
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("0052e530_TEST FAIL\n");
        return 1;
    }
    std::printf("0052e530_TEST PASS\n");
    return 0;
}