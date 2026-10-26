// 0063aee0 behaviour test: CWorldMap::IsLineOfSight (match.cpp body verbatim), host stand-in for the theme height lookup.
// C++03, g++ -w; also VC7.1 /Od.
#include <stdio.h>
#include <math.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class C2DCoordI { public:
    long x, y;
    C2DCoordI() {}
    C2DCoordI(long ax, long ay) : x(ax), y(ay) {}
};
static float g_height[16][16];
static int g_lookups = 0;
class CWorldMap {
public:
    float GetThemeMinCameraSizeZOffsetAt(const C2DCoordI& pos) const
    {
        ++g_lookups;
        if (pos.x < 0 || pos.y < 0 || pos.x >= 16 || pos.y >= 16) return 0.0f;
        return g_height[pos.y][pos.x];
    }
    bool IsLineOfSight(const C3DVector& from, const C3DVector& to) const;
};
struct SBlockOffset { long x, y; };
static SBlockOffset s_BlockCornerOffsets[4] = { { 0, 1 }, { 1, 1 }, { 1, 0 }, { 0, 0 } };

bool CWorldMap::IsLineOfSight(const C3DVector& from, const C3DVector& to) const
{
    bool visible = true;
    C3DVector pos = from;
    float dx = to.X - from.X;
    float dy = to.Y - from.Y;
    float dz = to.Z - from.Z;
    float steps;
    C3DVector step;
    C3DVector& st = step;
    if (fabsf(dx) > fabsf(dy)) {
        steps = fabsf(dx);
        step.X = 1.0f;
        float inv = 1.0f / steps;
        step.Y = dy * inv;
        st.Z = dz * inv;
        if (dx < 0.0f)
            step.X = -1.0f;
    } else {
        steps = fabsf(dy);
        float inv = 1.0f / steps;
        step.Y = 1.0f;
        step.X = dx * inv;
        st.Z = dz * inv;
        if (dy < 0.0f)
            step.Y = -1.0f;
    }
    while (steps >= 1.0f) {
        unsigned char blocked = 0;
        for (long i = 0; i < 4; ++i) {
            C2DCoordI cell((long)((float)s_BlockCornerOffsets[i].x + pos.X), (long)((float)s_BlockCornerOffsets[i].y + pos.Y));
            if (GetThemeMinCameraSizeZOffsetAt(cell) > pos.Z)
                ++blocked;
            if (blocked == 4) {
                visible = false;
                break;
            }
        }
        if (!visible)
            break;
        pos.X += step.X;
        pos.Y += step.Y;
        pos.Z = step.Z + pos.Z;
        steps -= 1.0f;
    }
    return visible;
}

static void Clear() { for (int y = 0; y < 16; ++y) for (int x = 0; x < 16; ++x) g_height[y][x] = 0.0f; }
int main()
{
    CWorldMap w;
    Clear();
    // flat ground, line above it: visible
    CHECK(w.IsLineOfSight(C3DVector(1.0f, 1.0f, 5.0f), C3DVector(9.0f, 3.0f, 5.0f)));
    // a 2x2 wall of height 10 at x 4..5, y 1..2 blocks a low line along y=1 (all four corners of block (4,1) high)
    g_height[1][4] = 10.0f; g_height[1][5] = 10.0f; g_height[2][4] = 10.0f; g_height[2][5] = 10.0f;
    CHECK(!w.IsLineOfSight(C3DVector(1.0f, 1.0f, 5.0f), C3DVector(9.0f, 1.0f, 5.0f)));
    // same line reversed (negative X step)
    CHECK(!w.IsLineOfSight(C3DVector(9.0f, 1.0f, 5.0f), C3DVector(1.0f, 1.0f, 5.0f)));
    // a line high enough passes over it
    CHECK(w.IsLineOfSight(C3DVector(1.0f, 1.0f, 12.0f), C3DVector(9.0f, 1.0f, 12.0f)));
    // a rising line: starts low (blocked before reaching the wall? wall at x=4 where z = 2 + 3*2 = 8 < 10) -> blocked
    CHECK(!w.IsLineOfSight(C3DVector(1.0f, 1.0f, 2.0f), C3DVector(9.0f, 1.0f, 18.0f)));
    // rising faster: z at x=4 is 2 + 3*4 = 14 > 10 -> visible (Z advance through step.Z)
    CHECK(w.IsLineOfSight(C3DVector(1.0f, 1.0f, 2.0f), C3DVector(5.0f, 1.0f, 18.0f)));
    // only three corners high: not blocked
    Clear();
    g_height[1][4] = 10.0f; g_height[1][5] = 10.0f; g_height[2][4] = 10.0f;
    CHECK(w.IsLineOfSight(C3DVector(1.0f, 1.0f, 5.0f), C3DVector(9.0f, 1.0f, 5.0f)));
    // Y-major line along x=4 through a full wall block at (4,6)
    Clear();
    g_height[6][4] = 10.0f; g_height[6][5] = 10.0f; g_height[7][4] = 10.0f; g_height[7][5] = 10.0f;
    CHECK(!w.IsLineOfSight(C3DVector(4.0f, 1.0f, 5.0f), C3DVector(4.0f, 12.0f, 5.0f)));
    CHECK(!w.IsLineOfSight(C3DVector(4.0f, 12.0f, 5.0f), C3DVector(4.0f, 1.0f, 5.0f)));
    // a step count below one does no lookups
    g_lookups = 0;
    CHECK(w.IsLineOfSight(C3DVector(4.0f, 4.0f, 0.0f), C3DVector(4.5f, 4.25f, 0.0f)));
    CHECK(g_lookups == 0);
    if (!g_fail) printf("0063aee0_TEST PASS\n");
    return g_fail;
}