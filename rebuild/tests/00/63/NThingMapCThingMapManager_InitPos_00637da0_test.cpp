#include <cstdio>
struct Coord { long X, Y; };
struct Map { Coord Origin; };
struct Cell { Map* PMap; long Count; };
struct Pos { Cell* PCell; long X, Y; };
static long g_shift[2] = { -1, 2 };
static void InitPos(Cell* cells, long width, const Coord& pos, Pos& out, long level)
{
    long shift = g_shift[level];
    Cell* cell = &cells[pos.Y / 128 * width + pos.X / 128];
    out.PCell = cell;
    if (shift == -1) { if (cell && cell->Count > 0) { out.X = 0; out.Y = 0; return; } }
    else if (cell && cell->Count > 0) { out.X = (pos.X - cell->PMap->Origin.X) >> shift; out.Y = (pos.Y - cell->PMap->Origin.Y) >> shift; return; }
    out.PCell = 0;
}
int main()
{
    Map m = { { 128, 0 } };
    Cell cells[4] = { { 0, 0 }, { &m, 1 }, { 0, 0 }, { &m, 1 } };
    Coord p = { 200, 40 };
    Pos o = { 0, 7, 7 };
    InitPos(cells, 2, p, o, 1);
    bool ok = o.PCell == &cells[1] && o.X == 18 && o.Y == 10;
    InitPos(cells, 2, p, o, 0); ok = ok && o.PCell == &cells[1] && o.X == 0 && o.Y == 0;
    Coord q = { 10, 10 }; InitPos(cells, 2, q, o, 1); ok = ok && o.PCell == 0;
    if (!ok) { std::printf("00637da0_TEST FAIL\n"); return 1; }
    std::printf("00637da0_TEST PASS\n");
    return 0;
}