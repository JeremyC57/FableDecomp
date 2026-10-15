// Behaviour test for 0x00A0B140: SetWorldTransform(matrix) loads a 3x4 matrix as D3DTS_WORLD (0x100)
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
    Dev d; D3DM m; From34(m, Sample());
    d.SetTransform(0x100, &m);
    CHECK(d.state == 0x100);
    CheckM(d);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00a0b140_TEST FAIL\n"); return 1; }
    std::printf("00a0b140_TEST PASS\n");
    return 0;
}