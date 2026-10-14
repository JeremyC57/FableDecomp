#include <cstdio>
struct C2DCoordI { long X; long Y; };
struct C2DBoxI {
    long TLX; long TLY; long BRX; long BRY;
    C2DBoxI(const C2DCoordI& centre, long radius);
};
C2DBoxI::C2DBoxI(const C2DCoordI& centre, long radius)
    : TLX(centre.X - radius), TLY(centre.Y - radius), BRX(centre.X + radius), BRY(centre.Y + radius) {}
int main()
{
    C2DCoordI c = { 10, -4 };
    C2DBoxI b(c, 3);
    if (b.TLX != 7 || b.TLY != -7 || b.BRX != 13 || b.BRY != -1) { std::printf("00434740_TEST FAIL\n"); return 1; }
    std::printf("00434740_TEST PASS\n");
    return 0;
}
