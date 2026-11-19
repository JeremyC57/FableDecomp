// Behaviour test for CMiniMapDisplay::GetRelativePosOnMap (0x00829CE0), a portable model of the row: a world position as a fraction of
// the region map, (world - origin) / extents per axis; Z is ignored.
#include <stdio.h>
struct V2 { float X, Y; };
struct V3 { float X, Y, Z; };
static void GetRelativePosOnMap(long ox, long oy, long ex, long ey, const V3& w, V2& r)
{
    r.X = (w.X - ox) / ex;
    r.Y = (w.Y - oy) / ey;
}
int main()
{
    int bad = 0; V2 r;
    V3 a = { 30.0f, 45.0f, 99.0f }; GetRelativePosOnMap(10, 20, 40, 50, a, r); if (r.X != 0.5f || r.Y != 0.5f) ++bad;
    V3 b = { -64.0f, 0.0f, 0.0f };  GetRelativePosOnMap(-128, -64, 256, 128, b, r); if (r.X != 0.25f || r.Y != 0.5f) ++bad;
    V3 c = { 10.0f, 20.0f, 0.0f };  GetRelativePosOnMap(10, 20, 8, 8, c, r); if (r.X != 0.0f || r.Y != 0.0f) ++bad;
    printf(bad ? "00829ce0_TEST FAIL\n" : "00829ce0_TEST PASS\n");
    return bad ? 1 : 0;
}
