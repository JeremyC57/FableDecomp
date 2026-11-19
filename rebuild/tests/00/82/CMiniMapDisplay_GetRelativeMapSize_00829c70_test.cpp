// Behaviour test for CMiniMapDisplay::GetRelativeMapSize (0x00829C70), a portable model of the row: the region map's extents as a size
// whose long side is 1 (equal extents give (1, 1)); the output is left untouched only if the extents are unordered (never for integers).
#include <stdio.h>
struct V2 { float X, Y; };
static void GetRelativeMapSize(long ex, long ey, V2& size)
{
    if (ex == ey) { size.X = 1.0f; size.Y = 1.0f; }
    else if (ex > ey) { size.X = 1.0f; size.Y = (float)ey / ex; }
    else if (ex < ey) { size.X = (float)ex / ey; size.Y = 1.0f; }
}
int main()
{
    int bad = 0; V2 s;
    GetRelativeMapSize(64, 64, s);  if (s.X != 1.0f || s.Y != 1.0f) ++bad;
    GetRelativeMapSize(200, 50, s); if (s.X != 1.0f || s.Y != 0.25f) ++bad;
    GetRelativeMapSize(30, 120, s); if (s.X != 0.25f || s.Y != 1.0f) ++bad;
    GetRelativeMapSize(3, 2, s);    if (s.X != 1.0f || s.Y != 2.0f / 3.0f) ++bad;
    printf(bad ? "00829c70_TEST FAIL\n" : "00829c70_TEST PASS\n");
    return bad ? 1 : 0;
}
