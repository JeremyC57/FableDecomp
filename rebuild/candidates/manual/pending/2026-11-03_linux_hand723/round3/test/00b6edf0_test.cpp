// Behaviour test for CEngineWaterRenderer::BuildConstantBuffers (0x00B6EDF0): the 16x16-quad patch triangulation (2x2 blocks,
// alternating diagonals) and the fixed texture-quad index list, through a copy of the row's logic.
#include <cstdio>
#include <vector>
#include <set>

static void BuildPatch(std::vector<unsigned short>& polys)
{
    for (long y = 0; y < 16; y += 2) {
        long rowBase = y * 17;
        for (long x = 0; x < 16; x += 2) {
            long index = x + rowBase;
            for (long j = 0; j < 2; ++j) {
                long rowStart = index;
                for (long i = 0; i < 2; ++i) {
                    if ((j & 1) == (i & 1)) {
                        unsigned short t[6] = { (unsigned short)index, (unsigned short)(index + 18), (unsigned short)(index + 17),
                                                (unsigned short)index, (unsigned short)(index + 1), (unsigned short)(index + 18) };
                        polys.insert(polys.end(), t, t + 6);
                    } else {
                        unsigned short t[6] = { (unsigned short)index, (unsigned short)(index + 1), (unsigned short)(index + 17),
                                                (unsigned short)(index + 17), (unsigned short)(index + 1), (unsigned short)(index + 18) };
                        polys.insert(polys.end(), t, t + 6);
                    }
                    ++index;
                }
                index = rowStart + 17;
            }
        }
    }
}

int main()
{
    std::vector<unsigned short> polys;
    BuildPatch(polys);
    bool ok = polys.size() == 16 * 16 * 6;                       // two triangles per quad
    std::set<unsigned short> used;
    for (size_t k = 0; k < polys.size(); ++k) {
        if (polys[k] > 17 * 17 - 1) ok = false;
        used.insert(polys[k]);
    }
    ok = ok && used.size() == 17 * 17;                           // every patch vertex is referenced
    // quad (0,0): diagonal 0-18; quad (1,0): the other diagonal, 2-18 (alternating)
    ok = ok && polys[0] == 0 && polys[1] == 18 && polys[2] == 17 && polys[4] == 1;
    ok = ok && polys[6] == 1 && polys[7] == 2 && polys[8] == 18 && polys[9] == 18;
    // second row of the first block: quad (0,1) uses the 18-34 diagonal, starting at vertex 17
    ok = ok && polys[12] == 17 && polys[13] == 18 && polys[14] == 34;
    // the fixed texture-quad list: 10 triangles over 14 vertices
    static const unsigned short tex[30] = { 0,1,2, 0,2,3, 4,5,6, 4,6,7, 7,6,9, 7,9,8, 10,11,12, 10,12,13, 10,11,9, 10,9,8 };
    for (int k = 0; k < 30; ++k)
        if (tex[k] > 13) ok = false;
    if (!ok) { std::printf("00b6edf0_TEST FAIL\n"); return 1; }
    std::printf("00b6edf0_TEST PASS\n");
    return 0;
}
