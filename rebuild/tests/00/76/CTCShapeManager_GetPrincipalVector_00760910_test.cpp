#include <cstdio>
// Behaviour model of CTCShapeManager::GetPrincipalVector (0x00760910): thing position minus the selected shape's first point.
struct V { float x, y, z; };
static bool Principal(bool selected, const V* pts, int n, V pos, V& out)
{
    if (selected && n) { out.x = pos.x - pts[0].x; out.y = pos.y - pts[0].y; out.z = pos.z - pts[0].z; return true; }
    return false;
}
int main()
{
    V pts[2] = { {1, 2, 3}, {9, 9, 9} }, pos = { 4, 4, 4 }, out = { 0, 0, 0 };
    if (!Principal(true, pts, 2, pos, out) || out.x != 3 || out.y != 2 || out.z != 1) { std::printf("00760910_TEST FAIL\n"); return 1; }
    if (Principal(false, pts, 2, pos, out) || Principal(true, pts, 0, pos, out)) { std::printf("00760910_TEST FAIL\n"); return 1; }
    std::printf("00760910_TEST PASS\n");
    return 0;
}