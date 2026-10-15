// Behaviour test for 0x00760BC0: 2-D bounds over a point list.
#include <cstdio>
struct V { float X, Y, Z; };
struct Box { float MinX, MinY, MaxX, MaxY; };
static void GetBounds(const V* b, const V* e, Box& box)
{
    const V* it = b; float x = it->X, y = it->Y; box.MinX = x; box.MaxX = x; box.MinY = y; box.MaxY = y;
    for (++it; it != e; ++it) { if (it->X < box.MinX) box.MinX = it->X; if (it->Y < box.MinY) box.MinY = it->Y;
        if (it->X > box.MaxX) box.MaxX = it->X; if (it->Y > box.MaxY) box.MaxY = it->Y; }
}
int main()
{
    V p[4] = {{1, 2, 9}, {-3, 5, 0}, {4, -1, 0}, {0, 0, 7}};
    Box b; GetBounds(p, p + 4, b);
    bool ok = b.MinX == -3 && b.MaxX == 4 && b.MinY == -1 && b.MaxY == 5;
    GetBounds(p, p + 1, b); ok = ok && b.MinX == 1 && b.MaxX == 1 && b.MinY == 2 && b.MaxY == 2;
    if (!ok) { std::printf("00760bc0_TEST FAIL\n"); return 1; }
    std::printf("00760bc0_TEST PASS\n");
    return 0;
}
