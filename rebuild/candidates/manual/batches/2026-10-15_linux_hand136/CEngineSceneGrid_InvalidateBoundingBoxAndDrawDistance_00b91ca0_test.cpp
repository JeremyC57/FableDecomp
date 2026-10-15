// Behaviour test for 0x00B91CA0 (see src/00b91ca0.cpp): the recursive grid-cell invalidation over three levels.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#define CHECK(c) do { if (!(c)) { std::printf("00b91ca0_TEST FAIL\n"); return 1; } } while (0)

struct CEngineSceneGridCell { unsigned char BoundingBoxValid : 1; unsigned char TotalCountValid : 1; };
struct CEngineSceneGrid {
    CEngineSceneGrid* LevelDown; unsigned int Size; CEngineSceneGridCell** Cells;
    void Invalidate(unsigned int x, unsigned int y, bool recurse)
    {
        CEngineSceneGridCell* cell = Cells[y * Size + x];
        if (cell) {
            cell->BoundingBoxValid = false;
            if (recurse && LevelDown)
                for (unsigned int j = 0; j < 2; ++j)
                    for (unsigned int i = 0; i < 2; ++i)
                        LevelDown->Invalidate(x * 2 + i, y * 2 + j, true);
        }
    }
};
int main()
{
    CEngineSceneGridCell c1[1], c2[4], c3[16];
    CEngineSceneGridCell* p1[1], *p2[4], *p3[16];
    int i;
    for (i = 0; i < 1; ++i) { c1[i].BoundingBoxValid = 1; c1[i].TotalCountValid = 1; p1[i] = &c1[i]; }
    for (i = 0; i < 4; ++i) { c2[i].BoundingBoxValid = 1; c2[i].TotalCountValid = 1; p2[i] = &c2[i]; }
    for (i = 0; i < 16; ++i) { c3[i].BoundingBoxValid = 1; c3[i].TotalCountValid = 1; p3[i] = &c3[i]; }
    p3[5] = 0;
    CEngineSceneGrid g3 = { 0, 4, p3 }, g2 = { &g3, 2, p2 }, g1 = { &g2, 1, p1 };
    g2.Invalidate(1, 0, false);
    CHECK(c2[1].BoundingBoxValid == 0 && c2[0].BoundingBoxValid == 1 && c3[2].BoundingBoxValid == 1);
    g1.Invalidate(0, 0, true);
    for (i = 0; i < 4; ++i) CHECK(c2[i].BoundingBoxValid == 0 && c2[i].TotalCountValid == 1);
    for (i = 0; i < 16; ++i) if (i != 5) CHECK(c3[i].BoundingBoxValid == 0 && c3[i].TotalCountValid == 1);
    CHECK(c1[0].BoundingBoxValid == 0 && c3[5].BoundingBoxValid == 1);
    std::printf("00b91ca0_TEST PASS\n");
    return 0;
}
