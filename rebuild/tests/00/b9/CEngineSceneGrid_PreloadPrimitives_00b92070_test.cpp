// behaviour test for 00b92070 CEngineSceneGrid::PreloadPrimitives (clip-volume overload): an empty cell or one whose box
// is outside the volume preloads nothing; otherwise each primitive inside the volume is preloaded (PreloadStat 2 when
// loaded, 1 otherwise), then the four child cells one level down are visited at (2x+i, 2y+j)
#include <cstdio>
struct Prim { bool inside, loads; int stat; Prim* next; };
struct Cell { Prim* first; bool boxInside; };
struct Grid { int size; Cell** cells; Grid* down; };
static int g_visits[8][8];
static bool Preload(Grid* g, int x, int y)
{
    Cell* cell = g->cells[y * g->size + x];
    bool result = false;
    ++g_visits[y][x];
    if (!cell || !cell->boxInside) return false;
    for (Prim* p = cell->first; p; p = p->next)
        if (p->inside) { bool loaded = p->loads; p->stat = loaded ? 2 : 1; result |= loaded; }
    if (g->down)
        for (int j = 0; j < 2; ++j)
            for (int i = 0; i < 2; ++i)
                result |= Preload(g->down, x * 2 + i, y * 2 + j);
    return result;
}
int main()
{
    Prim c = { true, true, 0, 0 }, b = { false, true, 0, 0 }, a = { true, false, 0, &b };
    Cell top = { &a, true }, child = { &c, true };
    Cell* topCells[1] = { &top };
    Cell* childCells[4] = { 0, 0, 0, &child };
    Grid lower = { 2, childCells, 0 }, upper = { 1, topCells, &lower };
    bool r = Preload(&upper, 0, 0);
    if (!r || a.stat != 1 || b.stat != 0 || c.stat != 2 || g_visits[1][1] != 1) { std::printf("00b92070_TEST FAIL\n"); return 1; }
    top.boxInside = false; a.stat = 0;
    if (Preload(&upper, 0, 0) || a.stat != 0) { std::printf("00b92070_TEST FAIL\n"); return 1; }
    std::printf("00b92070_TEST PASS\n");
    return 0;
}