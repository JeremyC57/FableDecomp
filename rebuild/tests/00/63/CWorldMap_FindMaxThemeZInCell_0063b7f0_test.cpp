#include <cstdio>
// Behaviour test: the CWorldMap find-max-in-cell query -- the four corner blocks {0,1} {1,1} {1,0} {0,0} offset from the
// cell position (truncated), each in-map block's height compared, 0 when none is in the map.
struct C3DVector { float X, Y, Z; };
struct C2DCoordI { long x, y; C2DCoordI(long ax, long ay) : x(ax), y(ay) {} };
struct CCellCornerOffset { long X, Y; };
static CCellCornerOffset g_CellCornerOffsets[4] = { { 0, 1 }, { 1, 1 }, { 1, 0 }, { 0, 0 } };
static int g_dims = 0;
struct CWorldMap {
    long MaxX;
    const void* GetDimensions() const { ++g_dims; return this; }
    bool IsPosInMap(const C2DCoordI& p) const { return p.x >= 0 && p.x < MaxX && p.y >= 0; }
    float GetThemeSizeZAtBlock(const C2DCoordI& p) const { return (float)(p.x * 10 + p.y); }
    float FindMaxThemeZInCell(const C3DVector* cell) const
    {
        GetDimensions();
        float maxZ = 0.0f;
        for (unsigned int i = 0; i < 4; ++i) {
            long x = (long)(g_CellCornerOffsets[i].X + cell->X);
            long y = (long)(g_CellCornerOffsets[i].Y + cell->Y);
            if (IsPosInMap(C2DCoordI(x, y))) {
                float z = GetThemeSizeZAtBlock(C2DCoordI(x, y));
                if (z > maxZ)
                    maxZ = z;
            }
        }
        return maxZ;
    }
};
int main()
{
    CWorldMap m; m.MaxX = 100;
    C3DVector c = { 3.7f, 5.2f, 0.0f };
    if (m.FindMaxThemeZInCell(&c) != 46.0f || g_dims != 1) { std::printf("0063b7f0_TEST FAIL\n"); return 1; }          // block (4,6)
    m.MaxX = 4;                                               // only x = 3 blocks are in the map
    if (m.FindMaxThemeZInCell(&c) != 36.0f) { std::printf("0063b7f0_TEST FAIL\n"); return 1; }                            // block (3,6)
    C3DVector off = { -5.0f, -5.0f, 0.0f };
    if (m.FindMaxThemeZInCell(&off) != 0.0f) { std::printf("0063b7f0_TEST FAIL\n"); return 1; }
    std::printf("0063b7f0_TEST PASS\n");
    return 0;
}