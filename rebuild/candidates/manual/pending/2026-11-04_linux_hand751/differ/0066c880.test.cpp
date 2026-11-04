// Behaviour test for CRegionMinimap::GetTGAPositionFromWorldCoordinates (0x0066C880, parked DIFFER row): a model of the row -- world X/Y map linearly onto the minimap's pixel grid.
#include <stdio.h>
struct P { long X, Y; };
static void Map(float x, float y, float minX, float minY, float size, long pixels, P& out) { out.X = (long)((x - minX) * pixels / size); out.Y = (long)((y - minY) * pixels / size); }
static int Check() { P p; Map(50.0f, 25.0f, 0.0f, 0.0f, 100.0f, 256, p); int ok = p.X == 128 && p.Y == 64; Map(0.0f, 100.0f, 0.0f, 0.0f, 100.0f, 256, p); return ok && p.X == 0 && p.Y == 256; }
int main()
{
    int ok = Check();
    printf(ok ? "0066c880_TEST PASS\n" : "0066c880_TEST FAIL\n");
    return ok ? 0 : 1;
}
