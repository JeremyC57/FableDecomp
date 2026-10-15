// Behaviour test for 0x00B937A0 CEngineSceneGrid::GetCellCreate: create on demand, parent from LevelUp (x/2, y/2), reuse.
#include <cstdio>

struct CCell { CCell* Parent; CCell(CCell* p) : Parent(p) {} };
struct CGrid {
    CGrid* LevelUp; unsigned int Size; CCell** cells;
    CCell* GetCellCreate(unsigned int x, unsigned int y)
    {
        CCell* cell = cells[y * Size + x];
        if (!cell) {
            CCell* parent = 0;
            if (LevelUp) parent = LevelUp->GetCellCreate(x >> 1, y >> 1);
            cell = new CCell(parent);
            cells[y * Size + x] = cell;
        }
        return cell;
    }
};

int main()
{
    CCell* coarse[4] = { 0, 0, 0, 0 };
    CCell* fine[16]; for (int i = 0; i < 16; ++i) fine[i] = 0;
    CGrid g0; g0.LevelUp = 0; g0.Size = 2; g0.cells = coarse;
    CGrid g1; g1.LevelUp = &g0; g1.Size = 4; g1.cells = fine;
    CCell* c = g1.GetCellCreate(3, 2);
    if (!c || fine[2 * 4 + 3] != c || c->Parent != coarse[1 * 2 + 1] || !coarse[3] || coarse[3]->Parent != 0) { std::printf("00b937a0_TEST FAIL\n"); return 1; }
    if (g1.GetCellCreate(3, 2) != c) { std::printf("00b937a0_TEST FAIL\n"); return 1; }
    CCell* d = g1.GetCellCreate(2, 3);
    if (d == c || d->Parent != coarse[3]) { std::printf("00b937a0_TEST FAIL\n"); return 1; }
    std::printf("00b937a0_TEST PASS\n");
    return 0;
}