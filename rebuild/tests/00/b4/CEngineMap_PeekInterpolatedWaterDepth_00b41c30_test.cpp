// behaviour test for CEngineMap::PeekInterpolatedWaterDepth 0x00B41C30
#include <cstdio>
#include <cmath>
class CEngineMap { public:
    float depth[16][16];
    bool PeekHasWaterFast(long x, long y) const { return x >= 0 && y >= 0 && x < 16 && y < 16 && depth[x][y] > 0.0f; }
    float PeekWaterDepth(long x, long y) const { return (x >= 0 && y >= 0 && x < 16 && y < 16) ? depth[x][y] : 0.0f; }
    float PeekInterpolatedWaterDepth(long x, long y, long radius) const
    {
        long minX = x - radius;
        long minY = y - radius;
        long maxX = x + radius;
        long maxY = y + radius;
        if (!PeekHasWaterFast(x, y)) {
            bool found = false;
            for (long i = minX; i <= maxX && !found; ++i) {
                for (long j = minY; j <= maxY && !found; ++j) {
                    if (PeekHasWaterFast(i, j)) {
                        minX = i - radius;
                        minY = j - radius;
                        maxX = i + radius;
                        maxY = j + radius;
                        found = true;
                    }
                }
            }
            if (!found)
                return 0.0f;
        }
        float total = 0.0f;
        float count = 0.0f;
        for (long i = minX; i <= maxX; ++i) {
            for (long j = minY; j <= maxY; ++j) {
                float d = PeekWaterDepth(i, j);
                if (d > 0.001f) {
                    total += d;
                    count += 1.0f;
                }
            }
        }
        return count > 0.0f ? total / count : 0.0f;
    }
};
static CEngineMap m;
int main()
{
    int i, j;
    for (i = 0; i < 16; ++i) for (j = 0; j < 16; ++j) m.depth[i][j] = 0.0f;
    // all dry -> 0
    if (m.PeekInterpolatedWaterDepth(5, 5, 1) != 0.0f) { std::printf("00b41c30_TEST FAIL\n"); return 1; }
    m.depth[5][5] = 2.0f; m.depth[5][6] = 4.0f;
    // wet centre: average of 2 and 4 over the 3x3 square
    if (std::fabs(m.PeekInterpolatedWaterDepth(5, 5, 1) - 3.0f) > 1e-5f) { std::printf("00b41c30_TEST FAIL\n"); return 1; }
    // dry centre (4,4): first wet cell found scanning x outer, y inner is (5,5); re-centred square -> 3
    if (std::fabs(m.PeekInterpolatedWaterDepth(4, 4, 1) - 3.0f) > 1e-5f) { std::printf("00b41c30_TEST FAIL\n"); return 1; }
    // dry centre, re-centred on (5,6) when (5,5) is outside: centre (6,7) radius 1 -> first wet (5,6): square 4..6 x 5..7 -> (5,5)=2,(5,6)=4 -> 3
    if (std::fabs(m.PeekInterpolatedWaterDepth(6, 7, 1) - 3.0f) > 1e-5f) { std::printf("00b41c30_TEST FAIL\n"); return 1; }
    // tiny depths ignored
    m.depth[10][10] = 0.0005f;
    if (m.PeekInterpolatedWaterDepth(10, 10, 0) != 0.0f) { std::printf("00b41c30_TEST FAIL\n"); return 1; }
    std::printf("00b41c30_TEST PASS\n");
    return 0;
}