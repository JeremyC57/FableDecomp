// Behaviour test for row 0x00BE2CD0 CEngineLandscapeMeshBuilder::BuildSharedIndexBufferGenerationTables (links the row's object; the
// CArray<unsigned short> / STLport vector inline bodies come from the row's own COMDATs, only the out-of-line
// vector<unsigned short>::_M_fill_insert is stubbed here, allocating with malloc because the row's deallocate frees with free).
// Cases: the 64-bit overflow guard on both sides (negative product, product * 6 above 0x7fffffff: the count is left untouched),
// a hand-worked 1x1 quad (corner numbering, both tri-list arms, the first-polygon strip start and the five-index stitch),
// the caller's strip buffer vs the temporary CArray buffer (tri_striped_indicies == 0), the tri list present vs absent, and
// grids 1x1 .. 16x16 compared index for index with an independent model whose per-arm counters prove every reachable strip arm
// (the order flip at the strip start, the one-index continuation, the five-index stitch) is taken. Two arms are unreachable with
// these offset tables and are asserted never taken: the first polygon always takes the flip arm with count 0 and leaves count 4,
// and from then on every polygon arrives with (count & 1) != poly_within_quad (+1 and +5 both flip the parity as the quad half
// alternates), so neither the six-index flip (count > 3) nor the `count < 3` three-index start ever runs. In the continuation
// test the two compares (poly_verts[0] == strip[-2], poly_verts[1] == strip[-1]) are equal on every reachable polygon of every
// grid 1x1..16x16 (checked exhaustively against the model), so dropping either one is an equivalent mutant.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class C2DCoordI { public: long X, Y; };
class CEngineLandscapeMeshBuilder {
public:
    void BuildSharedIndexBufferGenerationTables(C2DCoordI* verts, unsigned short* tri_striped_indicies, unsigned short* tri_list_indicies,
                                                long width, long height, long& tri_striped_index_count);
    void* VTable;
};

namespace std {
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    void _M_fill_insert(T* position, unsigned int n, const T& x);
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
static int g_fill_inserts;
template<> void vector<unsigned short, allocator<unsigned short> >::_M_fill_insert(unsigned short* position, unsigned int n,
                                                                                   const unsigned short& x)
{
    ++g_fill_inserts;
    unsigned int size = (unsigned int)(_M_finish - _M_start);
    unsigned int before = (unsigned int)(position - _M_start);
    unsigned short* data = (unsigned short*)malloc((size + n) * sizeof(unsigned short));
    unsigned int i;
    for (i = 0; i < before; ++i) data[i] = _M_start[i];
    for (i = 0; i < n; ++i) data[before + i] = x;
    for (i = before; i < size; ++i) data[n + i] = _M_start[i];
    free(_M_start);
    _M_start = data;
    _M_finish = data + size + n;
    _M_end_of_storage = _M_finish;
}
}

// Independent model of the generation (with per-arm counters).
static long g_arm_flip_long, g_arm_flip_start, g_arm_continue, g_arm_short, g_arm_stitch;
static void Model(C2DCoordI* verts, unsigned short* strip, unsigned short* list, long width, long height, long& count)
{
    static const long ox[2][2][3] = { { { 0, 1, 1 }, { 0, 0, 1 } }, { { 0, 1, 0 }, { 1, 0, 1 } } };
    static const long oy[2][2][3] = { { { 0, 0, 1 }, { 0, 1, 1 } }, { { 0, 0, 1 }, { 0, 1, 1 } } };
    long ids[17][17];
    long nverts = 0, npolys = 0;
    for (int a = 0; a < 17; ++a)
        for (int b = 0; b < 17; ++b) ids[a][b] = -1;
    count = 0;
    for (long x = 0; x < width; ++x)
        for (long y = 0; y < height; ++y)
            for (long p = 0; p < 2; ++p) {
                long odd = (x ^ y) & 1;
                unsigned short v[3];
                for (int i = 0; i < 3; ++i) {
                    long cx = x + ox[odd][p][i], cy = y + oy[odd][p][i];
                    if (ids[cx][cy] < 0) {
                        verts[nverts].X = cx; verts[nverts].Y = cy;
                        ids[cx][cy] = nverts++;
                    }
                    v[i] = (unsigned short)ids[cx][cy];
                }
                if (list) {
                    list[npolys * 3 + 0] = p == 1 ? v[0] : v[1];
                    list[npolys * 3 + 1] = p == 1 ? v[1] : v[0];
                    list[npolys * 3 + 2] = v[2];
                }
                ++npolys;
                unsigned short* s = strip + count;
                if (p == (count & 1)) {
                    if (count > 3) {
                        ++g_arm_flip_long;
                        s[0] = s[-1]; s[1] = v[0]; s[2] = v[0]; s[3] = v[0]; s[4] = v[1]; s[5] = v[2]; count += 6;
                    } else {
                        ++g_arm_flip_start;
                        s[0] = v[0]; s[1] = v[0]; s[2] = v[1]; s[3] = v[2]; count += 4;
                    }
                } else if (count > 2 && v[0] == s[-2] && v[1] == s[-1]) {
                    ++g_arm_continue;
                    s[0] = v[2]; count += 1;
                } else if (count < 3) {
                    ++g_arm_short;
                    s[0] = v[0]; s[1] = v[1]; s[2] = v[2]; count += 3;
                } else {
                    ++g_arm_stitch;
                    s[0] = s[-1]; s[1] = v[0]; s[2] = v[0]; s[3] = v[1]; s[4] = v[2]; count += 5;
                }
            }
}

static C2DCoordI g_verts[289], g_mverts[289];
static unsigned short g_strip[16 * 16 * 6 + 8], g_mstrip[16 * 16 * 6 + 8], g_list[16 * 16 * 6 + 8], g_mlist[16 * 16 * 6 + 8];

static int Fail(const char* what, long w, long h)
{
    printf("00be2cd0_TEST FAIL %s (%ldx%ld)\n", what, w, h);
    return 0;
}

// Runs the row and the model on one grid and compares everything the row writes.
static int Grid(CEngineLandscapeMeshBuilder& builder, long w, long h, bool own_strip, bool with_list)
{
    memset(g_verts, 0x5a, sizeof(g_verts)); memset(g_mverts, 0x5a, sizeof(g_mverts));
    memset(g_strip, 0x5a, sizeof(g_strip)); memset(g_mstrip, 0x5a, sizeof(g_mstrip));
    memset(g_list, 0x5a, sizeof(g_list)); memset(g_mlist, 0x5a, sizeof(g_mlist));
    long count = -7, mcount = -7;
    int inserts = std::g_fill_inserts;
    builder.BuildSharedIndexBufferGenerationTables(g_verts, own_strip ? g_strip : 0, with_list ? g_list : 0, w, h, count);
    Model(g_mverts, g_mstrip, with_list ? g_mlist : 0, w, h, mcount);
    if (count != mcount) return Fail("strip count", w, h);
    if (memcmp(g_verts, g_mverts, sizeof(g_verts)) != 0) return Fail("vertex coordinates", w, h);
    if (memcmp(g_list, g_mlist, sizeof(g_list)) != 0) return Fail("tri list", w, h);
    if (own_strip) {
        if (std::g_fill_inserts != inserts) return Fail("temporary buffer used with a caller strip", w, h);
        if (memcmp(g_strip, g_mstrip, sizeof(g_strip)) != 0) return Fail("strip", w, h);
    } else {
        if (std::g_fill_inserts != inserts + 1) return Fail("no temporary strip buffer", w, h);
        for (size_t i = 0; i < sizeof(g_strip) / sizeof(g_strip[0]); ++i)
            if (g_strip[i] != 0x5a5a) return Fail("strip written to the caller's null buffer", w, h);
    }
    return 1;
}

int main()
{
    CEngineLandscapeMeshBuilder builder;
    builder.VTable = 0;
    int ok = 1;

    // Overflow guard: a negative product and a product * 6 above 0x7fffffff return before anything is written.
    long count = -7;
    builder.BuildSharedIndexBufferGenerationTables(g_verts, g_strip, g_list, -1, 1, count);
    ok &= count == -7 ? 1 : Fail("negative size not rejected", -1, 1);
    builder.BuildSharedIndexBufferGenerationTables(g_verts, g_strip, g_list, 0x8000, 0x8000, count);
    ok &= count == -7 ? 1 : Fail("overflowing size not rejected", 0x8000, 0x8000);
    builder.BuildSharedIndexBufferGenerationTables(g_verts, g_strip, g_list, 0x10000, 0x10000, count);
    ok &= count == -7 ? 1 : Fail("64-bit size not rejected", 0x10000, 0x10000);
    // A zero-area grid passes the guard and resets the count.
    builder.BuildSharedIndexBufferGenerationTables(g_verts, g_strip, g_list, 0, 5, count);
    ok &= count == 0 ? 1 : Fail("empty grid count", 0, 5);

    // Hand-worked 1x1: corners (0,0)=0 (1,0)=1 (1,1)=2 (0,1)=3; tri list {1,0,2} {0,3,2}; strip 0 0 1 2 | 2 0 0 3 2.
    memset(g_strip, 0, sizeof(g_strip)); memset(g_list, 0, sizeof(g_list));
    builder.BuildSharedIndexBufferGenerationTables(g_verts, g_strip, g_list, 1, 1, count);
    static const unsigned short strip1[9] = { 0, 0, 1, 2, 2, 0, 0, 3, 2 };
    static const unsigned short list1[6] = { 1, 0, 2, 0, 3, 2 };
    static const long vx1[4] = { 0, 1, 1, 0 }, vy1[4] = { 0, 0, 1, 1 };
    ok &= count == 9 ? 1 : Fail("1x1 count", 1, 1);
    ok &= memcmp(g_strip, strip1, sizeof(strip1)) == 0 ? 1 : Fail("1x1 strip", 1, 1);
    ok &= memcmp(g_list, list1, sizeof(list1)) == 0 ? 1 : Fail("1x1 tri list", 1, 1);
    for (int v = 0; v < 4; ++v)
        ok &= g_verts[v].X == vx1[v] && g_verts[v].Y == vy1[v] ? 1 : Fail("1x1 corner", 1, 1);

    // Grids against the model, with caller / temporary strip buffers and with / without a tri list.
    static const long sizes[][2] = { { 1, 1 }, { 1, 2 }, { 2, 1 }, { 2, 2 }, { 3, 2 }, { 2, 5 }, { 4, 4 }, { 5, 3 }, { 7, 16 }, { 16, 16 } };
    for (int s = 0; s < (int)(sizeof(sizes) / sizeof(sizes[0])); ++s) {
        ok &= Grid(builder, sizes[s][0], sizes[s][1], true, true);
        ok &= Grid(builder, sizes[s][0], sizes[s][1], true, false);
        ok &= Grid(builder, sizes[s][0], sizes[s][1], false, true);
    }
    ok &= g_arm_flip_start > 0 && g_arm_continue > 0 && g_arm_stitch > 0 ? 1 : Fail("strip arm not covered", 0, 0);
    ok &= g_arm_flip_long == 0 && g_arm_short == 0 ? 1 : Fail("unreachable arm reached", 0, 0);

    if (!ok) return 1;
    printf("00be2cd0_TEST PASS\n");
    return 0;
}