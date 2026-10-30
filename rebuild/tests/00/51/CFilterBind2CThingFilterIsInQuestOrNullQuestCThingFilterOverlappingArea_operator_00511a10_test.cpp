// Behaviour test for 0x00511a10 the combined filter operator() (its OverlappingArea part, see src/00511a10.cpp):
// ceil the bounding radius, grow the area box by it, keep things whose truncated centre lies in [X0,X1) x [Y0,Y1).
#include <cstdio>
#include <cmath>
struct Box { long X0, Y0, X1, Y1; };
static long CeilToLong(float v) { return (long)std::ceil((double)v); }
static bool Overlaps(const Box& a, float cx, float cy, float radius)
{
    Box area = a;
    float r = (float)CeilToLong(radius);
    area.X1 = (long)((float)area.X1 + r);
    area.X0 = (long)((float)area.X0 - r);
    area.Y0 = (long)((float)area.Y0 - r);
    area.Y1 = (long)((float)area.Y1 + r);
    long x = (long)cx, y = (long)cy;
    return x >= area.X0 && x < area.X1 && y >= area.Y0 && y < area.Y1;
}
int main()
{
    Box b = { 0, 0, 10, 10 };
    if (!Overlaps(b, 5.0f, 5.0f, 0.0f) || Overlaps(b, 11.5f, 5.0f, 0.0f) || !Overlaps(b, 11.5f, 5.0f, 1.2f)
        || Overlaps(b, -3.5f, 5.0f, 2.0f) || !Overlaps(b, -2.5f, 5.0f, 2.0f) || CeilToLong(2.0f) != 2 || CeilToLong(2.1f) != 3) {
        std::printf("00511a10_TEST FAIL\n"); return 1;
    }
    std::printf("00511a10_TEST PASS\n"); return 0;
}