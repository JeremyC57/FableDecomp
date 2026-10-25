// Behaviour test for 00c465e0: CRegionMap::GetCellSize -- returns the cell size at +0x34
#include <cstdio>
#include <cstring>
struct CRegionMap { unsigned char m_head[0x34]; float m_cellSize; float m_radius; float GetCellSize() const { return m_cellSize; } };
int main()
{
    CRegionMap m; std::memset(&m, 0, sizeof m); m.m_cellSize = 2.5f; m.m_radius = 9.0f;
    float d = m.GetCellSize() - 2.5f;
    int ok = d < 0.0001f && d > -0.0001f && sizeof(CRegionMap) == 0x3c;
    std::printf(ok ? "00c465e0_TEST PASS\n" : "00c465e0_TEST FAIL\n");
    return ok ? 0 : 1;
}
