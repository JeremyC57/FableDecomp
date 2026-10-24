// behaviour test for 0x00B91F00 CEngineSceneGrid::MovePrimitivesUp
#include <cstdio>

struct Cell {
    Cell* Parent;
    int   count;
    void MovePrimitivesTo(Cell& target) { target.count += count; count = 0; }
};
struct Grid {
    unsigned long Size;
    Cell**        cells;
    void MovePrimitivesUp()
    {
        for (unsigned long y = 0; y < Size; ++y)
            for (unsigned long x = 0; x < Size; ++x) {
                Cell* cell = cells[y * Size + x];
                if (cell)
                    cell->MovePrimitivesTo(*cell->Parent);
            }
    }
};

int main()
{
    Cell parent[1]; parent[0].Parent = 0; parent[0].count = 1;
    Cell c[4];
    for (int i = 0; i < 4; ++i) { c[i].Parent = &parent[0]; c[i].count = i + 1; }
    Cell* table[4] = { &c[0], 0, &c[2], &c[3] };
    Grid g; g.Size = 2; g.cells = table;
    g.MovePrimitivesUp();
    if (parent[0].count != 1 + 1 + 3 + 4 || c[0].count != 0 || c[1].count != 2 || c[2].count != 0 || c[3].count != 0) { std::printf("00b91f00_TEST FAIL\n"); return 1; }
    Grid e; e.Size = 0; e.cells = 0;
    e.MovePrimitivesUp();
    std::printf("00b91f00_TEST PASS\n");
    return 0;
}