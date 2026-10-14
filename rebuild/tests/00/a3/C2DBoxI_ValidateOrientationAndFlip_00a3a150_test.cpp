#include <cstdio>
struct C2DBoxI { long TLX; long TLY; long BRX; long BRY; void ValidateOrientationAndFlip(); };
void C2DBoxI::ValidateOrientationAndFlip()
{
    if (BRX < TLX) { long t = TLX; TLX = BRX; BRX = t; }
    if (BRY < TLY) { long t = TLY; TLY = BRY; BRY = t; }
}
int main()
{
    C2DBoxI b = { 9, 2, 3, 8 };
    b.ValidateOrientationAndFlip();
    if (b.TLX != 3 || b.BRX != 9 || b.TLY != 2 || b.BRY != 8) { std::printf("00a3a150_TEST FAIL\n"); return 1; }
    C2DBoxI c = { 1, 7, 1, 5 };
    c.ValidateOrientationAndFlip();
    if (c.TLX != 1 || c.BRX != 1 || c.TLY != 5 || c.BRY != 7) { std::printf("00a3a150_TEST FAIL\n"); return 1; }
    std::printf("00a3a150_TEST PASS\n");
    return 0;
}