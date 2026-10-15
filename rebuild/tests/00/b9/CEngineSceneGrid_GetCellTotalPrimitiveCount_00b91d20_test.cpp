// Behaviour test for 0x00B91D20 CEngineSceneGrid::GetCellTotalPrimitiveCount: recursive sum over the 2x2 children, cached.
#include <cstdio>

struct CCell {
    long LocalCount; unsigned int TotalCount;
    unsigned char BoundingBoxValid : 1; unsigned char TotalCountValid : 1;
};
struct CGrid {
    CGrid* LevelDown; unsigned int Size; CCell** cells;
    unsigned int GetCellTotalPrimitiveCount(unsigned int x, unsigned int y)
    {
        CCell* cell = cells[y * Size + x];
        if (!cell) return 0;
        if (!cell->TotalCountValid) {
            unsigned int total = cell->LocalCount;
            if (LevelDown) {
                for (unsigned int i = 0; i < 2; ++i)
                    for (unsigned int j = 0; j < 2; ++j)
                        total += LevelDown->GetCellTotalPrimitiveCount(x * 2 + j, y * 2 + i);
            }
            cell->TotalCount = total;
            cell->TotalCountValid = 1;
        }
        return cell->TotalCount;
    }
};

int main()
{
    CCell fine[16]; CCell* fineP[16];
    for (int i = 0; i < 16; ++i) { fine[i].LocalCount = i; fine[i].TotalCountValid = 0; fine[i].BoundingBoxValid = 1; fineP[i] = &fine[i]; }
    fineP[5] = 0;
    CGrid g1; g1.LevelDown = 0; g1.Size = 4; g1.cells = fineP;
    CCell coarse[4]; CCell* coarseP[4];
    for (int i = 0; i < 4; ++i) { coarse[i].LocalCount = 100; coarse[i].TotalCountValid = 0; coarseP[i] = &coarse[i]; }
    CGrid g0; g0.LevelDown = &g1; g0.Size = 2; g0.cells = coarseP;
    // coarse (1,0) covers fine (2,0),(3,0),(2,1),(3,1) = 2+3+6+7
    if (g0.GetCellTotalPrimitiveCount(1, 0) != 118 || !coarse[1].TotalCountValid || !fine[2].BoundingBoxValid) { std::printf("00b91d20_TEST FAIL\n"); return 1; }
    // coarse (0,0): 0+1+4+(missing 5)
    if (g0.GetCellTotalPrimitiveCount(0, 0) != 105) { std::printf("00b91d20_TEST FAIL\n"); return 1; }
    coarse[1].LocalCount = 0;   // cached: unchanged
    if (g0.GetCellTotalPrimitiveCount(1, 0) != 118) { std::printf("00b91d20_TEST FAIL\n"); return 1; }
    std::printf("00b91d20_TEST PASS\n");
    return 0;
}