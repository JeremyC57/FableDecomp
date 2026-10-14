#include <cstdio>
struct C2DBoxF { float TLX, TLY, BRX, BRY; C2DBoxF(float a, float b, float c, float d) : TLX(a), TLY(b), BRX(c), BRY(d) { } };
struct CDisplayManager {
    unsigned char _pad[0x18c]; long BackBufferDimensions_X; long BackBufferDimensions_Y;
    C2DBoxF GetAVisibleTelevisionScreenArea() const { return C2DBoxF(0.0f, 0.0f, (float)BackBufferDimensions_X, (float)BackBufferDimensions_Y); }
};
int main()
{
    CDisplayManager m; m.BackBufferDimensions_X = 1280; m.BackBufferDimensions_Y = 720;
    C2DBoxF b = m.GetAVisibleTelevisionScreenArea();
    if (b.TLX != 0.0f || b.TLY != 0.0f || b.BRX != 1280.0f || b.BRY != 720.0f || (char*)&m.BackBufferDimensions_Y - (char*)&m != 0x190) { std::printf("009becb0_TEST FAIL\n"); return 1; }
    std::printf("009becb0_TEST PASS\n"); return 0;
}
