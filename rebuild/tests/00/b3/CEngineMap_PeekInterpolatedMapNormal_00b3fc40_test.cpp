// Behaviour test for CEngineMap::PeekInterpolatedMapNormal (0x00B3FC40): a standalone model -- floor of both coordinates, the four
// corner map normals (x,y) (x+1,y) (x,y+1) (x+1,y+1) blended with bilinear weights from the fractional parts, then normalised.
// Distinct corner normals and asymmetric fractions make every weight / corner / index mutant visible; integer and negative inputs
// pin the floor.
#include <cstdio>
#include <cmath>

struct V { float X, Y, Z; };
static long g_calls; static long g_cx[4], g_cy[4];
static V PeekMapNormal(long x, long y)            // a distinct normal per cell
{
    if (g_calls < 4) { g_cx[g_calls] = x; g_cy[g_calls] = y; }
    ++g_calls;
    V n = { (float)x * 0.5f + 1.0f, (float)y * 0.25f - 2.0f, 3.0f + (float)(x * 7 + y * 3) * 0.125f };
    return n;
}
static long FloatToLongFloor(float v) { return (long)std::floor(v); }   // GFFloatToLongFloor

static V PeekInterpolatedMapNormal(float xp, float yp)
{
    long int_xp = FloatToLongFloor(xp);
    long int_yp = FloatToLongFloor(yp);
    float frac_x = xp - int_xp;
    float frac_y = yp - int_yp;
    V normal00 = PeekMapNormal(int_xp, int_yp);
    V normal10 = PeekMapNormal(int_xp + 1, int_yp);
    V normal01 = PeekMapNormal(int_xp, int_yp + 1);
    V normal11 = PeekMapNormal(int_xp + 1, int_yp + 1);
    float w00 = (1.0f - frac_x) * (1.0f - frac_y), w10 = frac_x * (1.0f - frac_y), w01 = (1.0f - frac_x) * frac_y, w11 = frac_x * frac_y;
    V normal = { normal00.X * w00 + normal10.X * w10 + normal01.X * w01 + normal11.X * w11,
                 normal00.Y * w00 + normal10.Y * w10 + normal01.Y * w01 + normal11.Y * w11,
                 normal00.Z * w00 + normal10.Z * w10 + normal01.Z * w01 + normal11.Z * w11 };
    float f = 1.0f / std::sqrt(normal.X * normal.X + normal.Y * normal.Y + normal.Z * normal.Z);   // retail: GFFastOneOverSqrt
    normal.X *= f; normal.Y *= f; normal.Z *= f;
    return normal;
}

static int fails = 0;
static void Check(bool ok, int id) { if (!ok) { std::printf("case %d failed\n", id); ++fails; } }
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
// independent expectation: bilinear blend written per corner, normalised
static bool Expect(float xp, float yp, long ix, long iy, float fx, float fy, int id)
{
    g_calls = 0;
    V r = PeekInterpolatedMapNormal(xp, yp);
    bool ok = g_calls == 4 && g_cx[0] == ix && g_cy[0] == iy && g_cx[1] == ix + 1 && g_cy[1] == iy && g_cx[2] == ix && g_cy[2] == iy + 1 &&
              g_cx[3] == ix + 1 && g_cy[3] == iy + 1;
    g_calls = 4;   // stop recording
    V a = PeekMapNormal(ix, iy), b = PeekMapNormal(ix + 1, iy), c = PeekMapNormal(ix, iy + 1), d = PeekMapNormal(ix + 1, iy + 1);
    float ex = a.X * (1 - fx) * (1 - fy) + b.X * fx * (1 - fy) + c.X * (1 - fx) * fy + d.X * fx * fy;
    float ey = a.Y * (1 - fx) * (1 - fy) + b.Y * fx * (1 - fy) + c.Y * (1 - fx) * fy + d.Y * fx * fy;
    float ez = a.Z * (1 - fx) * (1 - fy) + b.Z * fx * (1 - fy) + c.Z * (1 - fx) * fy + d.Z * fx * fy;
    float m = std::sqrt(ex * ex + ey * ey + ez * ez);
    ok = ok && Near(r.X, ex / m) && Near(r.Y, ey / m) && Near(r.Z, ez / m) && Near(r.X * r.X + r.Y * r.Y + r.Z * r.Z, 1.0f);
    Check(ok, id);
    return ok;
}

int main()
{
    Expect(3.25f, 5.6f, 3, 5, 0.25f, 0.6f, 1);        // asymmetric fractions
    Expect(3.0f, 5.0f, 3, 5, 0.0f, 0.0f, 2);          // exact integers: corner 00 only
    Expect(-0.25f, 2.75f, -1, 2, 0.75f, 0.75f, 3);    // negative x floors down
    Expect(7.9f, -3.1f, 7, -4, 0.9f, 0.9f, 4);
    Expect(10.5f, 0.125f, 10, 0, 0.5f, 0.125f, 5);
    Expect(-2.0f, -1.5f, -2, -2, 0.0f, 0.5f, 6);      // negative exact integer stays; negative half floors down
    Expect(2.5f, 3.5f, 2, 3, 0.5f, 0.5f, 7);          // halves on both rounding parities floor down
    Expect(0.0f, -0.5f, 0, -1, 0.0f, 0.5f, 8);        // zero and the first negative half
    if (fails) { std::printf("00b3fc40_TEST FAIL\n"); return 1; }
    std::printf("00b3fc40_TEST PASS\n");
    return 0;
}