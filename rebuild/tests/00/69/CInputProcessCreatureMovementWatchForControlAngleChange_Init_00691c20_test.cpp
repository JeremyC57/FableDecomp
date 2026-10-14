#include <cstdio>
static const bool g_is32 = sizeof(void*) == 4 && sizeof(long) == 4;   // the layout checks hold for the 32-bit VC7.1 build
struct C2DVector { float X, Y; C2DVector(float x, float y) : X(x), Y(y) { } };
struct CInputProcessCreatureMovementWatchForControlAngleChange { virtual void v00() { } virtual void v01() { } virtual void Init() { LastJoyDirection = C2DVector(0.0f, 0.0f); }
    unsigned char _pad04[0x24]; C2DVector LastJoyDirection; CInputProcessCreatureMovementWatchForControlAngleChange() : LastJoyDirection(3.0f, 4.0f) { } };
int main()
{
    CInputProcessCreatureMovementWatchForControlAngleChange p; p.Init();
    if (p.LastJoyDirection.X != 0.0f || p.LastJoyDirection.Y != 0.0f || (g_is32 && (char*)&p.LastJoyDirection - (char*)&p != 0x28)) { std::printf("00691c20_TEST FAIL\n"); return 1; }
    std::printf("00691c20_TEST PASS\n");
    return 0;
}