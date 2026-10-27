// 00643970 behaviour test for C2DBoxF::GetTRPos() const
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#ifndef __fastcall
#define __fastcall
#endif
#ifndef __cdecl
#define __cdecl
#endif
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed at line %d\n", __LINE__); g_fail = 1; } } while (0)
class C2DVector {
public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class C2DBoxF {
public:
    float TLX, TLY, BRX, BRY;
    C2DVector GetTRPos() const;
};
C2DVector C2DBoxF::GetTRPos() const
{
    return C2DVector(BRX, TLY);
}
static void run()
{
    C2DBoxF box = { 10.0f, 20.0f, 110.0f, 70.0f };
    C2DVector tr = box.GetTRPos();
    CHECK(tr.X == 110.0f && tr.Y == 20.0f);
}

int main()
{
    run();
    if (g_fail) {
        std::printf("00643970_TEST FAIL\n");
        return 1;
    }
    std::printf("00643970_TEST PASS\n");
    return 0;
}