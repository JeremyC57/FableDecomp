// behaviour test for 0x00511710: the bound filter needs both: the quest test and the centre inside the radius-grown area
#include <cstdio>
#include <cmath>
static bool InArea(float cx, float cy, float r, long x0, long y0, long x1, long y1)
{
    float rr = (float)(long)std::ceil(r);
    long X1 = (long)(x1 + rr), X0 = (long)(x0 - rr), Y0 = (long)(y0 - rr), Y1 = (long)(y1 + rr);
    long x = (long)cx, y = (long)cy;
    return x >= X0 && x < X1 && y >= Y0 && y < Y1;
}
static bool Bound(bool inQuest, float cx, float cy, float r) { return inQuest && InArea(cx, cy, r, 0, 0, 10, 10); }
int main()
{
    int ok = Bound(true, 5, 5, 0) && !Bound(false, 5, 5, 0) && Bound(true, 11.5f, 5, 1.2f) && !Bound(true, 13, 5, 1.0f);
    if (!ok) { std::printf("00511710_TEST FAIL\n"); return 1; }
    std::printf("00511710_TEST PASS\n");
    return 0;
}