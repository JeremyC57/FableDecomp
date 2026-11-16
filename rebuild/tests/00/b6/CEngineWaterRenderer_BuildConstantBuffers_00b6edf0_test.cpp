// Behaviour test for 0x00B6EDF0 CEngineWaterRenderer::BuildConstantBuffers(): a host model of the row with recording stand-ins for
// the buffers and an identity "stripifier" (strip = poly list). Checks: the 17x17-vertex patch gives 8x8 blocks of 2x2 quads = 256
// quads = 1536 indices; quad (i, j) of a block is split along the a..d diagonal (a, a+18, a+17 / a, a+1, a+18) when (j & 1) == (i & 1),
// else along the b..c diagonal (a, a+1, a+17 / a+17, a+1, a+18), so the diagonals form a checkerboard; every index < 289; the strip index
// buffer is 2 * stripLength bytes and WaterForegroundPolyCount = stripLength - 2; the texture index list is the 30 fixed indices; the
// 14 vertices carry the fixed positions / UVs / colours (v4 and v5 transparent white, the v6/v7/v12/v13 band at y = -0.1).
#include <cstdio>
#include <cstring>
struct Vtx { float X, Y, Z; unsigned long C; float U, V; };
struct Out {
    unsigned short polys[2048]; long nPolys;
    long stripBytes, polyCount;
    unsigned short tri[30];
    Vtx vtx[14];
};
static void Row(Out& o)
{
    o.nPolys = 0;
    for (long y = 0; y < 16; y += 2)
        for (long x = 0; x < 16; x += 2) {
            long index = x + y * 17;
            for (long j = 0; j < 2; ++j) {
                long rowStart = index;
                for (long i = 0; i < 2; ++i) {
                    unsigned short* p = o.polys + o.nPolys;
                    if ((j & 1) == (i & 1)) {
                        p[0] = (unsigned short)index; p[1] = (unsigned short)(index + 18); p[2] = (unsigned short)(index + 17);
                        p[3] = (unsigned short)index; p[4] = (unsigned short)(index + 1);  p[5] = (unsigned short)(index + 18);
                    } else {
                        p[0] = (unsigned short)index;        p[1] = (unsigned short)(index + 1); p[2] = (unsigned short)(index + 17);
                        p[3] = (unsigned short)(index + 17); p[4] = (unsigned short)(index + 1); p[5] = (unsigned short)(index + 18);
                    }
                    o.nPolys += 6;
                    ++index;
                }
                index = rowStart + 17;
            }
        }
    long stripLength = o.nPolys;            // identity stripifier stand-in
    o.stripBytes = stripLength * 2;
    o.polyCount = stripLength - 2;
    static const unsigned short tri[30] = { 0,1,2, 0,2,3, 4,5,6, 4,6,7, 7,6,9, 7,9,8, 10,11,12, 10,12,13, 10,11,9, 10,9,8 };
    memcpy(o.tri, tri, sizeof(tri));
    static const Vtx vtx[14] = {
        { -1, 1, 1, 0xffffffff, 0, 0 }, { 1, 1, 1, 0xffffffff, 1, 0 }, { 1, -1, 1, 0xffffffff, 1, 1 }, { -1, -1, 1, 0xffffffff, 0, 1 },
        { -1, 0, 1, 0x00ffffff, 0, 0 }, { 1, 0, 1, 0x00ffffff, 1, 0 }, { 1, -0.1f, 1, 0xffffffff, 1, 0.1f }, { -1, -0.1f, 1, 0xffffffff, 0, 0.1f },
        { -1, -1, 1, 0xffffffff, 0, 1 }, { 1, -1, 1, 0xffffffff, 1, 1 }, { -1, 1, 1, 0xffffffff, 0, 0 }, { 1, 1, 1, 0xffffffff, 1, 0 },
        { 1, -0.1f, 1, 0xffffffff, 1, 1 }, { -1, -0.1f, 1, 0xffffffff, 0, 1 } };
    memcpy(o.vtx, vtx, sizeof(vtx));
}
static int bad = 0;
static void Check(bool c, const char* what) { if (!c) { printf("fail: %s\n", what); bad = 1; } }
int main()
{
    static Out o;
    Row(o);
    Check(o.nPolys == 1536, "1536 indices");
    Check(o.stripBytes == 3072 && o.polyCount == 1534, "strip size / poly count");
    // first block (a = 0): quad (0,0) a..d split, quad (1,0) b..c split, quad (0,1) b..c split, quad (1,1) a..d split
    static const unsigned short first[24] = { 0,18,17, 0,1,18,  1,2,18, 18,2,19,  17,18,34, 34,18,35,  18,36,35, 18,19,36 };
    Check(memcmp(o.polys, first, sizeof(first)) == 0, "first block");
    // checkerboard: for every quad the shared diagonal is a..d iff the quad's (column + row) parity inside the block is even
    long q = 0, maxIndex = 0;
    for (long k = 0; k < o.nPolys; ++k) if (o.polys[k] > maxIndex) maxIndex = o.polys[k];
    Check(maxIndex == 288, "indices inside the 17x17 patch");
    for (long y = 0; y < 16; y += 2) for (long x = 0; x < 16; x += 2) for (long j = 0; j < 2; ++j) for (long i = 0; i < 2; ++i, ++q) {
        long a = (y + j) * 17 + x + i;
        const unsigned short* p = o.polys + q * 6;
        bool ad = (p[1] == a + 18);
        Check(p[0] == a && ad == ((i & 1) == (j & 1)), "diagonal parity");
    }
    Check(o.tri[12] == 7 && o.tri[14] == 9 && o.tri[29] == 8, "texture indices");
    Check(o.vtx[4].C == 0x00ffffff && o.vtx[5].C == 0x00ffffff && o.vtx[6].C == 0xffffffff, "band colours");
    Check(o.vtx[6].Y == -0.1f && o.vtx[13].Y == -0.1f && o.vtx[7].V == 0.1f && o.vtx[12].V == 1.0f, "band rows");
    Check(o.vtx[1].X == 1.0f && o.vtx[1].Y == 1.0f && o.vtx[1].Z == 1.0f && o.vtx[1].U == 1.0f && o.vtx[1].V == 0.0f, "v1");
    printf(bad ? "00b6edf0_TEST FAIL\n" : "00b6edf0_TEST PASS\n");
    return bad;
}