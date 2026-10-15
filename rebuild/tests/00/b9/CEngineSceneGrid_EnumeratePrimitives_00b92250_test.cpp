// Behaviour test for 0x00B92250 CEngineSceneGrid::EnumeratePrimitives: cell chain first, then the four children (row-major in j).
#include <cstdio>

struct CPrim { CPrim* m_pNextInCell; int id; };
static int g_seen[32]; static int g_n = 0;
struct CEnum { virtual void Visit(CPrim* p) { g_seen[g_n++] = p->id; } };
struct CCell { CPrim* FirstPrimitive; };
struct CGrid {
    CGrid* LevelDown; unsigned int Size; CCell** cells;
    void EnumeratePrimitives(unsigned int x, unsigned int y, CEnum* e)
    {
        CCell* cell = cells[y * Size + x];
        if (cell) {
            for (CPrim* p = cell->FirstPrimitive; p; p = p->m_pNextInCell) e->Visit(p);
            if (LevelDown)
                for (unsigned int i = 0; i < 2; ++i)
                    for (unsigned int j = 0; j < 2; ++j)
                        LevelDown->EnumeratePrimitives(x * 2 + j, y * 2 + i, e);
        }
    }
};

int main()
{
    CPrim p[6];
    for (int i = 0; i < 6; ++i) { p[i].id = i; p[i].m_pNextInCell = 0; }
    p[0].m_pNextInCell = &p[1];
    CCell fine[16]; CCell* fineP[16];
    for (int i = 0; i < 16; ++i) { fine[i].FirstPrimitive = 0; fineP[i] = &fine[i]; }
    fine[2].FirstPrimitive = &p[2]; fine[3].FirstPrimitive = &p[3]; fine[7].FirstPrimitive = &p[4]; fine[6].FirstPrimitive = &p[5];
    fineP[3] = 0;
    CGrid g1; g1.LevelDown = 0; g1.Size = 4; g1.cells = fineP;
    CCell c[4]; CCell* cP[4]; for (int i = 0; i < 4; ++i) { c[i].FirstPrimitive = 0; cP[i] = &c[i]; }
    c[1].FirstPrimitive = &p[0];
    CGrid g0; g0.LevelDown = &g1; g0.Size = 2; g0.cells = cP;
    CEnum e;
    g0.EnumeratePrimitives(1, 0, &e);
    int want[5] = { 0, 1, 2, 5, 4 };
    if (g_n != 5) { std::printf("00b92250_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i) if (g_seen[i] != want[i]) { std::printf("00b92250_TEST FAIL\n"); return 1; }
    std::printf("00b92250_TEST PASS\n");
    return 0;
}