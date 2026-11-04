// Behaviour test for CThingFilter_BoundingBoxOverlapsMap::operator() (0x006A7FB0): a model of the row.
#include <stdio.h>

struct Box { float tlx, tly, brx, bry; };
static bool Overlaps(bool hasPhysics, float cx, float cy, float r, const Box& map)
{
    if (!hasPhysics) return false;
    Box t = { cx - r, cy - r, cx + r, cy + r };
    if (map.tlx > t.brx || map.brx < t.tlx || map.tly > t.bry || map.bry < t.tly) return false;
    return true;
}
static int run()
{
    Box map = { 0, 0, 10, 10 };
    return Overlaps(true, 5, 5, 1, map) && Overlaps(true, -0.5f, 5, 1, map) && !Overlaps(true, -2, 5, 1, map)
        && !Overlaps(true, 5, 12, 1, map) && !Overlaps(false, 5, 5, 1, map);
}
int main()
{
    int ok = run();
    printf(ok ? "006a7fb0_TEST PASS\n" : "006a7fb0_TEST FAIL\n");
    return ok ? 0 : 1;
}
