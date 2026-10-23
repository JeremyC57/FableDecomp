#include <cstdio>
// Model of CEngineLandscapeMap::InvalidateEdge: span parts outside this map go (grid cell by grid cell) to the
// neighbouring maps' landscapes, the in-map remainder to this map's background tree. Directions 0/1 run along y
// (1 probes the cell to the left), 2/3 along x (3 probes the cell above). The 0/1 outside walk has no return and
// falls into the in-map clipping, as retail does.
struct Call { int who; long x, y, len, dir; };
static Call g_calls[32]; static int g_count = 0;
static void Log(int who, long x, long y, long len, long dir)
{ Call c; c.who = who; c.x = x; c.y = y; c.len = len; c.dir = dir; g_calls[g_count++] = c; }
struct Landscape;
struct Map { long WorldPosX, WorldPosY, MapWidth, MapHeight; Landscape* Land; };
static Map g_maps[9];
static Map* PeekMap(long x, long y)
{
    for (int i = 0; i < 9; ++i) {
        Map& m = g_maps[i];
        if (x >= m.WorldPosX && x < m.WorldPosX + m.MapWidth && y >= m.WorldPosY && y < m.WorldPosY + m.MapHeight) return &m;
    }
    return 0;
}
struct Landscape {
    int id; Map* M; bool hasTree;
    void Neighbour(long x, long y, long len, long dir) { Log(id, x, y, len, dir); }
    void InvalidateEdge(long x, long y, long length, long direction)
    {
        if (direction == 0 || direction == 1) {
            long testX = x; if (direction == 1) --testX;
            if (testX < M->WorldPosX || testX >= M->WorldPosX + M->MapWidth || y + length <= M->WorldPosY || y >= M->WorldPosY + M->MapHeight) {
                for (long walkY = y; walkY < y + length; ) {
                    Map* other = PeekMap(testX, walkY);
                    if (other) {
                        if (other->Land) { long count = y - walkY + length; if (other->MapHeight < count) count = other->MapHeight; other->Land->Neighbour(x, walkY, count, direction); }
                        walkY += other->MapHeight;
                    } else walkY += 32;
                }
            }
            if (y < M->WorldPosY) {
                Map* other = PeekMap(testX, y);
                if (other && other->Land) other->Land->Neighbour(x, y, M->WorldPosY - y, direction);
                InvalidateEdge(x, M->WorldPosY, length + (y - M->WorldPosY), direction);
                return;
            }
            if (y + length > M->WorldPosY + M->MapHeight) {
                Map* other = PeekMap(testX, M->WorldPosY + M->MapHeight);
                if (other && other->Land) other->Land->Neighbour(x, M->WorldPosY + M->MapHeight, y + length - (M->WorldPosY + M->MapHeight), direction);
                InvalidateEdge(x, y, M->WorldPosY + M->MapHeight - y, direction);
                return;
            }
        } else {
            long testY = y; if (direction == 3) --testY;
            if (testY < M->WorldPosY || testY >= M->WorldPosY + M->MapHeight || x + length <= M->WorldPosX || x >= M->WorldPosX + M->MapWidth) {
                for (long walkX = x; walkX < x + length; ) {
                    Map* other = PeekMap(walkX, testY);
                    if (other) {
                        if (other->Land) { long count = x - walkX + length; if (other->MapWidth < count) count = other->MapWidth; other->Land->Neighbour(walkX, y, count, direction); }
                        walkX += other->MapWidth;
                    } else walkX += 32;
                }
                return;
            }
            if (x < M->WorldPosX) {
                Map* other = PeekMap(x, testY);
                if (other && other->Land) other->Land->Neighbour(x, y, M->WorldPosX - x, direction);
                InvalidateEdge(M->WorldPosX, y, length + (x - M->WorldPosX), direction);
                return;
            }
            if (x + length > M->WorldPosX + M->MapWidth) {
                Map* other = PeekMap(M->WorldPosX + M->MapWidth, testY);
                if (other && other->Land) other->Land->Neighbour(M->WorldPosX + M->MapWidth, y, x + length - (M->WorldPosX + M->MapWidth), direction);
                InvalidateEdge(x, y, M->WorldPosX + M->MapWidth - x, direction);
                return;
            }
        }
        if (hasTree) Log(100 + id, x, y, length, direction);
    }
};
static Landscape g_lands[9];
static bool Is(int i, int who, long x, long y, long len, long dir)
{ return g_calls[i].who == who && g_calls[i].x == x && g_calls[i].y == y && g_calls[i].len == len && g_calls[i].dir == dir; }
int main()
{
    int bad = 0;
    for (int i = 0; i < 9; ++i) {
        g_maps[i].WorldPosX = (i % 3) * 32; g_maps[i].WorldPosY = (i / 3) * 32; g_maps[i].MapWidth = 32; g_maps[i].MapHeight = 32;
        g_maps[i].Land = &g_lands[i]; g_lands[i].id = i; g_lands[i].M = &g_maps[i]; g_lands[i].hasTree = true;
    }
    Landscape& centre = g_lands[4];
    // along x, starting left of the centre map: the left neighbour gets 12 cells, the centre tree the rest
    centre.InvalidateEdge(20, 40, 30, 2);
    if (g_count != 2 || !Is(0, 3, 20, 40, 12, 2) || !Is(1, 104, 32, 40, 18, 2)) bad = 1;
    // along x past the right edge
    g_count = 0; centre.InvalidateEdge(50, 40, 30, 3);
    if (g_count != 2 || !Is(0, 5, 64, 40, 16, 3) || !Is(1, 104, 50, 40, 14, 3)) bad = 1;
    // along x with the probe row above the map: walk the neighbours and stop
    g_count = 0; centre.InvalidateEdge(20, 32, 50, 3);
    if (g_count != 2 || !Is(0, 0, 20, 32, 32, 3) || !Is(1, 1, 52, 32, 18, 3)) bad = 1;   // the walk steps by the neighbour width from walkX
    // along y with the probe column left of the map: walk, then fall into the in-map clipping
    g_count = 0; centre.InvalidateEdge(32, 40, 10, 1);
    if (g_count != 2 || !Is(0, 3, 32, 40, 10, 1) || !Is(1, 104, 32, 40, 10, 1)) bad = 1;
    // along y starting above the map
    g_count = 0; centre.InvalidateEdge(40, 20, 20, 0);
    if (g_count != 2 || !Is(0, 1, 40, 20, 12, 0) || !Is(1, 104, 40, 32, 8, 0)) bad = 1;
    // fully inside a map with no background tree: nothing is called
    g_count = 0; g_lands[4].hasTree = false; centre.InvalidateEdge(40, 40, 4, 0);
    if (g_count != 0) bad = 1;
    if (bad) { std::printf("00bdcbb0_TEST FAIL\n"); return 1; }
    std::printf("00bdcbb0_TEST PASS\n");
    return 0;
}
