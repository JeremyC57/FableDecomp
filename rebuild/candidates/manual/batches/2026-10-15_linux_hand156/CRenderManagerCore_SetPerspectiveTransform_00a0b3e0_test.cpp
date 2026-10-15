// Behaviour test for 0x00A0B3E0: SetPerspectiveTransform(16 floats) loads them unchanged as D3DTS_PROJECTION (3)
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %%d\n", __LINE__); } } while (0)
struct D3DM { float m[16]; };
struct M34 { float v[12]; };
static void SetD3DMatrix(D3DM& d, const float* s16) { for (int i = 0; i < 16; ++i) d.m[i] = s16[i]; }
static void From34(D3DM& d, const M34& s)
{
    float t[16] = { s.v[0], s.v[1], s.v[2], 0.0f, s.v[3], s.v[4], s.v[5], 0.0f,
                    s.v[6], s.v[7], s.v[8], 0.0f, s.v[9], s.v[10], s.v[11], 1.0f };
    SetD3DMatrix(d, t);
}
struct Dev { long state; D3DM got; int calls; Dev() : state(-1), calls(0) {} void SetTransform(long s, const D3DM* m) { state = s; got = *m; ++calls; } };
static M34 Sample() { M34 s; for (int i = 0; i < 12; ++i) s.v[i] = (float)(i + 1); return s; }
static void CheckM(const Dev& d)
{
    CHECK(d.calls == 1);
    CHECK(d.got.m[0] == 1.0f && d.got.m[2] == 3.0f && d.got.m[3] == 0.0f && d.got.m[4] == 4.0f && d.got.m[7] == 0.0f);
    CHECK(d.got.m[10] == 9.0f && d.got.m[11] == 0.0f && d.got.m[12] == 10.0f && d.got.m[14] == 12.0f && d.got.m[15] == 1.0f);
}
static void Run()
{
    float f[16]; for (int i = 0; i < 16; ++i) f[i] = (float)(i * 2);
    Dev d; D3DM m; SetD3DMatrix(m, f); d.SetTransform(3, &m);
    CHECK(d.state == 3 && d.calls == 1);
    for (int i = 0; i < 16; ++i) CHECK(d.got.m[i] == (float)(i * 2));
}

int main()
{
    Run();
    if (g_fail) { std::printf("00a0b3e0_TEST FAIL\n"); return 1; }
    std::printf("00a0b3e0_TEST PASS\n");
    return 0;
}
