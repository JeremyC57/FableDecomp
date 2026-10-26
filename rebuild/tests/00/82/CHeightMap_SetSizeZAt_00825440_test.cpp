#include <cstdio>
#include <cstdlib>
#include <cstring>
static const float g_zero_0122dedc = 0.0f, g_maxSize_012724c0 = 2048.0f, g_epsilon_0129ba3c = 1e-4f;
struct SCellIndex { long m_x; long m_y; }; struct SCell { float m_sizeZ; float m_1; float m_2; };
class CHeightMap { public: unsigned char _pad[0x10]; SCell* m_pCells; unsigned char _pad14[0x1f0]; long m_columns; long m_rows;
    void SetSizeZAt(const SCellIndex* cell, float size) { float value; if (size < g_zero_0122dedc) { value = g_zero_0122dedc; } else { float limit = g_maxSize_012724c0 - g_epsilon_0129ba3c; value = size > limit ? limit : size; } m_pCells[(m_rows - cell->m_y) * (m_columns + 1) + cell->m_x].m_sizeZ = value; } };

int main() {
    CHeightMap m; SCell cells[64]; std::memset(cells, 0, sizeof cells); m.m_pCells = cells; m.m_columns = 3; m.m_rows = 5; SCellIndex c; c.m_x = 2; c.m_y = 1;
    m.SetSizeZAt(&c, 10.5f); if (cells[(5 - 1) * 4 + 2].m_sizeZ != 10.5f || cells[(5 - 1) * 4 + 2].m_1 != 0) { std::printf("00825440_TEST FAIL\n"); return 1; } m.SetSizeZAt(&c, -3.0f); if (cells[18].m_sizeZ != 0.0f) { std::printf("00825440_TEST FAIL\n"); return 1; }
    m.SetSizeZAt(&c, 5000.0f); if (cells[18].m_sizeZ != 2048.0f - 1e-4f) { std::printf("00825440_TEST FAIL\n"); return 1; } c.m_x = 0; c.m_y = 5; m.SetSizeZAt(&c, 1.0f); if (cells[0].m_sizeZ != 1.0f) { std::printf("00825440_TEST FAIL\n"); return 1; }
    std::printf("00825440_TEST PASS\n"); return 0;
}