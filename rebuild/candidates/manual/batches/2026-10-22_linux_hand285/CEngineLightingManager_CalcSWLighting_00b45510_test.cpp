#include <cstdio>
#include <cmath>
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
static float Sqr(float x) { return x * x; }
struct V { float X, Y, Z; };
struct C { float R, G, B, A; };
static C Light(const V& d, const C& diff, const C& amb, const C& back, const V& n)
{
    float facing = -(d.Z * n.Z + d.Y * n.Y + d.X * n.X);
    float df = MIN(Sqr(MAX(facing, 0.0f)), 1.0f);
    float bf = MAX(-facing, 0.0f);
    float v[4] = { diff.R * df + amb.R + back.R * bf, diff.G * df + amb.G + back.G * bf, diff.B * df + amb.B + back.B * bf, diff.A * df + amb.A + back.A * bf };
    for (int i = 0; i < 4; ++i) { if (v[i] < 0.0f) v[i] = 0.0f; else if (v[i] > 1.0f) v[i] = 1.0f; }
    C r = { v[0], v[1], v[2], v[3] };
    return r;
}
static int Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main()
{
    int bad = 0;
    V d = { 0.0f, 0.0f, -1.0f };
    C diff = { 0.5f, 0.25f, 0.0f, 1.0f }, amb = { 0.1f, 0.1f, 0.1f, 0.0f }, back = { 0.2f, 0.0f, 0.9f, 0.0f };
    V up = { 0.0f, 0.0f, 1.0f };
    C a = Light(d, diff, amb, back, up);               // facing 1: full diffuse, no back
    if (!Near(a.R, 0.6f) || !Near(a.G, 0.35f) || !Near(a.B, 0.1f) || !Near(a.A, 1.0f)) bad = 1;
    V down = { 0.0f, 0.0f, -1.0f };
    C b = Light(d, diff, amb, back, down);             // facing -1: back only
    if (!Near(b.R, 0.3f) || !Near(b.G, 0.1f) || !Near(b.B, 1.0f) || !Near(b.A, 0.0f)) bad = 1;
    V half = { 0.0f, 0.6f, 0.5f };
    C c = Light(d, diff, amb, back, half);             // facing 0.5: diffuse factor 0.25
    if (!Near(c.R, 0.225f) || !Near(c.G, 0.1625f)) bad = 1;
    if (bad) { std::printf("00b45510_TEST FAIL\n"); return 1; }
    std::printf("00b45510_TEST PASS\n");
    return 0;
}
