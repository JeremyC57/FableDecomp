// Behaviour test for CNavigatorManager::OutputWorld (0x00A15890): a model of the flood fill over a small grid -- the start
// cell's type, every reachable cell written once with type 4 (switchable), 0 / 2 / 3 (preferability cost 0 / 0x80 / other),
// 1 (blocked, still expanded) or 5 (blocked edge, not expanded), and every blocked copy freed at the end.
#include <stdio.h>
#include <vector>
#include <algorithm>
struct Cell { bool Blocked, Switchable, Edge; unsigned char Cost; };
static const int W = 4, H = 3;
static Cell g_grid[H][W];
static int g_live;
static int TypeOf(const Cell& c)
{
    if (c.Switchable) return 4;
    if (!c.Blocked) return c.Cost == 0 ? 0 : (c.Cost == 0x80 ? 2 : 3);
    return c.Edge ? 5 : 1;
}
static void Output(int sx, int sy, int out[H][W])
{
    std::vector<int> open, visited;
    std::vector<int*> copies;
    for (int y = 0; y < H; ++y) for (int x = 0; x < W; ++x) out[y][x] = -1;
    const Cell& s = g_grid[sy][sx];
    out[sy][sx] = s.Switchable ? 4 : (s.Blocked ? 1 : TypeOf(s));
    if (s.Blocked && !s.Switchable) { copies.push_back(new int(0)); ++g_live; }
    open.push_back(sy * W + sx);
    while (open.size()) {
        int cur = open.back(); open.pop_back(); visited.push_back(cur);
        static const int dx[4] = { 1, -1, 0, 0 }, dy[4] = { 0, 0, 1, -1 };
        for (int k = 0; k < 4; ++k) {
            int x = cur % W + dx[k], y = cur / W + dy[k];
            if (x < 0 || y < 0 || x >= W || y >= H) continue;
            int id = y * W + x;
            const Cell& c = g_grid[y][x];
            if (c.Blocked && !c.Switchable) { copies.push_back(new int(id)); ++g_live; }
            if (std::find(visited.begin(), visited.end(), id) != visited.end()) continue;
            if (std::find(open.begin(), open.end(), id) != open.end()) continue;
            out[y][x] = TypeOf(c);
            if (out[y][x] == 5) visited.push_back(id); else open.push_back(id);
        }
    }
    while (copies.size()) { delete copies.back(); copies.pop_back(); --g_live; }
}
int main()
{
    int ok = 1, out[H][W];
    g_grid[0][1].Cost = 0x80; g_grid[0][2].Cost = 7; g_grid[1][1].Switchable = true; g_grid[1][1].Blocked = true;
    g_grid[1][2].Blocked = true; g_grid[2][3].Blocked = true; g_grid[2][3].Edge = true;
    Output(0, 0, out);
    ok &= out[0][0] == 0 && out[0][1] == 2 && out[0][2] == 3 && out[1][1] == 4 && out[1][2] == 1 && out[2][3] == 5;
    for (int y = 0; y < H; ++y) for (int x = 0; x < W; ++x) ok &= out[y][x] != -1;
    ok &= g_live == 0;
    Output(2, 1, out);
    ok &= out[1][2] == 1 && out[0][0] == 0 && g_live == 0;
    printf(ok ? "00a15890_TEST PASS\n" : "00a15890_TEST FAIL\n");
    return ok ? 0 : 1;
}
