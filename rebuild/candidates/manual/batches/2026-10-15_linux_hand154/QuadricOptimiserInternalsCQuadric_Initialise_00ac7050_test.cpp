// Behaviour test for 0x00ac7050 (CQuadric::Initialise): the reconstructed logic run over a host model.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct Quadric { double AA, AB, AC, AD, BB, BC, BD, CC, CD, DD, Weight; };
struct V3 { float X, Y, Z; };
static void Init(Quadric& q, const V3& n, const V3& p, float w)
{
    float a = n.X, b = n.Y, c = n.Z; float d = -(n.Z * p.Z + n.Y * p.Y + n.X * p.X);
    q.AA = a * a; q.AB = a * b; q.AC = a * c; q.AD = a * d; q.BB = b * b; q.BC = b * c; q.BD = b * d; q.CC = c * c; q.CD = c * d; q.DD = d * d; q.Weight = w;
}

int main()
{
    Quadric q; V3 n = { 0.0f, 0.0f, 1.0f }; V3 p = { 5.0f, 6.0f, 2.0f };
    Init(q, n, p, 3.0f);
    CHECK(q.CC == 1.0 && q.CD == -2.0 && q.DD == 4.0 && q.AA == 0.0 && q.Weight == 3.0);
    // a point on the plane has zero error: v^T Q v with v = (x, y, 2, 1)
    double x = 9, y = -4, z = 2;
    double e = q.AA*x*x + 2*q.AB*x*y + 2*q.AC*x*z + 2*q.AD*x + q.BB*y*y + 2*q.BC*y*z + 2*q.BD*y + q.CC*z*z + 2*q.CD*z + q.DD;
    CHECK(e == 0.0);
    if (g_fail) { std::printf("00ac7050_TEST FAIL\n"); return 1; }
    std::printf("00ac7050_TEST PASS\n");
    return 0;
}
