// Behaviour test for 00a3a180 (C2DBoxI::GetDistance): zero inside the box, the distance to the nearest edge or corner outside it, truncated to long.
#include <cstdio>
#include <cmath>
class C2DCoordI {
public:
    long X;
    long Y;
};
static float GFGet2DDistance(const C2DCoordI& a, const C2DCoordI& b)   // stands in for 0x009E2430
{
    float dx = (float)(a.X - b.X), dy = (float)(a.Y - b.Y);
    return std::sqrt(dx * dx + dy * dy);
}
class C2DBoxI {
public:
    long TLX;
    long TLY;
    long BRX;
    long BRY;
    long GetDistance(const C2DCoordI& pos) const;
};
long C2DBoxI::GetDistance(const C2DCoordI& pos) const
{
    C2DCoordI nearest = pos;
    if (nearest.X < TLX)
        nearest.X = TLX;
    else if (nearest.X > BRX)
        nearest.X = BRX;
    if (nearest.Y < TLY)
        nearest.Y = TLY;
    else if (nearest.Y > BRY)
        nearest.Y = BRY;
    return (long)GFGet2DDistance(pos, nearest);
}
int main()
{
    C2DBoxI box;
    box.TLX = 10; box.TLY = 20; box.BRX = 30; box.BRY = 40;
    C2DCoordI p;
    p.X = 15; p.Y = 25;
    if (box.GetDistance(p) != 0) { std::printf("00a3a180_TEST FAIL\n"); return 1; }
    p.X = 0; p.Y = 30;                         // left of the box: 10
    if (box.GetDistance(p) != 10) { std::printf("00a3a180_TEST FAIL\n"); return 1; }
    p.X = 20; p.Y = 47;                        // below: 7
    if (box.GetDistance(p) != 7) { std::printf("00a3a180_TEST FAIL\n"); return 1; }
    p.X = 33; p.Y = 16;                        // beyond the top-right corner: 3-4-5
    if (box.GetDistance(p) != 5) { std::printf("00a3a180_TEST FAIL\n"); return 1; }
    p.X = 11; p.Y = 18;                        // above, sqrt(4) = 2
    if (box.GetDistance(p) != 2) { std::printf("00a3a180_TEST FAIL\n"); return 1; }
    std::printf("00a3a180_TEST PASS\n");
    return 0;
}