// Behaviour test for CWorldMap::UpdateAllEdges (0x0063DFD0): border cells take the adjoining map's first column/row, else this map's last inner cell; then the corner.
#include <cstdio>
#include <cstring>
struct Box { int TLX, TLY, BRX, BRY; };
static int g_world[16][16];        // world-space heights
static int g_owner[16][16];        // map number per world cell (0 = none)
static Box g_area[3];
static bool g_loaded[3];
static int g_copies;
static void CopyPos(int srcX, int srcY, int dstX, int dstY) { ++g_copies; g_world[dstY][dstX] = g_world[srcY][srcX]; }
static void UpdateAllEdges(int worldBRX, int worldBRY)
{
    for (int map = 1; map < 3; ++map) {
        if (!g_loaded[map]) continue;
        Box a = g_area[map];
        if (a.BRX < worldBRX)
            for (int y = a.TLY; y < a.BRY; ++y) {
                int other = g_owner[y][a.BRX + 1];
                if (other != 0) { if (g_loaded[other]) CopyPos(g_area[other].TLX, y, a.BRX, y); }
                else CopyPos(a.BRX - 1, y, a.BRX, y);
            }
        if (a.BRY < worldBRY)
            for (int x = a.TLX; x < a.BRX; ++x) {
                int other = g_owner[a.BRY + 1][x];
                if (other != 0) { if (g_loaded[other]) CopyPos(x, g_area[other].TLY, x, a.BRY); }
                else CopyPos(x, a.BRY - 1, x, a.BRY);
            }
        CopyPos(a.BRX - 1, a.BRY - 1, a.BRX, a.BRY);
    }
}
int main()
{
    std::memset(g_owner, 0, sizeof g_owner);
    for (int y = 0; y < 16; ++y) for (int x = 0; x < 16; ++x) g_world[y][x] = y * 100 + x;
    Box a1 = { 0, 0, 4, 4 }, a2 = { 5, 0, 9, 4 };
    g_area[1] = a1; g_area[2] = a2; g_loaded[1] = true; g_loaded[2] = true;
    for (int y = 0; y <= 4; ++y) for (int x = 5; x <= 9; ++x) g_owner[y][x] = 2;
    g_copies = 0;
    UpdateAllEdges(12, 12);
    // map 1 right edge copies map 2's first column (x=5); bottom edge replicates row 3
    if (g_world[2][4] != 2 * 100 + 5 || g_world[4][1] != 3 * 100 + 1 || g_world[4][4] != 3 * 100 + 3) { std::printf("%d %d %d 0063dfd0_TEST FAIL\n", g_world[2][4], g_world[4][1], g_world[4][4]); return 1; }
    if (g_copies != (4 + 4 + 1) * 2) { std::printf("copies %d 0063dfd0_TEST FAIL\n", g_copies); return 1; }
    std::printf("0063dfd0_TEST PASS\n");
    return 0;
}
