#include <cstdio>
struct C2DCoordI { long X; long Y; };
struct C2DBoxI { long TLX; long TLY; long BRX; long BRY; void ExtendToContain(const C2DCoordI& coordI); };
void C2DBoxI::ExtendToContain(const C2DCoordI& coordI)
{
    if (TLX > coordI.X) TLX = coordI.X;
    if (BRX < coordI.X) BRX = coordI.X;
    if (TLY > coordI.Y) TLY = coordI.Y;
    if (BRY < coordI.Y) BRY = coordI.Y;
}
int main()
{
    C2DBoxI b = { 0, 0, 10, 10 };
    C2DCoordI a = { -5, 20 };
    b.ExtendToContain(a);
    if (b.TLX != -5 || b.TLY != 0 || b.BRX != 10 || b.BRY != 20) { std::printf("00a39e20_TEST FAIL\n"); return 1; }
    C2DCoordI c = { 3, 4 };
    b.ExtendToContain(c);
    if (b.TLX != -5 || b.TLY != 0 || b.BRX != 10 || b.BRY != 20) { std::printf("00a39e20_TEST FAIL\n"); return 1; }
    std::printf("00a39e20_TEST PASS\n");
    return 0;
}