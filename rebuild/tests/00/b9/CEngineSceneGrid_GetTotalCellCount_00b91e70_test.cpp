// Behaviour test for 0x00B91E70 CEngineSceneGrid::GetTotalCellCount (recursive count of created cells)
#include <cstdio>

struct Grid {
    Grid* LevelDown;
    unsigned int Size;
    int* Cells;                       // non-zero = created cell
    unsigned int GetTotalCellCount(unsigned int x, unsigned int y)
    {
        if (Cells[Size * y + x] == 0)
            return 0;
        unsigned int count = 1;
        if (LevelDown) {
            for (unsigned int dy = 0; dy < 2; ++dy)
                for (unsigned int dx = 0; dx < 2; ++dx)
                    count += LevelDown->GetTotalCellCount(x * 2 + dx, y * 2 + dy);
        }
        return count;
    }
};

int main()
{
    int top[4] = { 1, 0, 0, 1 };                  // 2x2
    int mid[16] = { 1, 1, 0, 0,  0, 1, 0, 0,  0, 0, 0, 0,  0, 0, 1, 0 };   // 4x4
    Grid g1; g1.LevelDown = 0; g1.Size = 4; g1.Cells = mid;
    Grid g0; g0.LevelDown = &g1; g0.Size = 2; g0.Cells = top;
    if (g0.GetTotalCellCount(0, 0) != 1 + 3) { std::printf("00b91e70_TEST FAIL\n"); return 1; }
    if (g0.GetTotalCellCount(1, 0) != 0) { std::printf("00b91e70_TEST FAIL\n"); return 1; }
    if (g0.GetTotalCellCount(1, 1) != 1 + 1) { std::printf("00b91e70_TEST FAIL\n"); return 1; }
    std::printf("00b91e70_TEST PASS\n");
    return 0;
}