// Behaviour test for CEngineScreenEffectDisplacementRenderer::ApplyDisplacement 0x00B87870: a model of the render-state
// setters (state changes only when the value differs, changed states join the update list), the shader enables, the user
// vertex constants and the draw, driven through a copy of the function's logic.
#include <cstdio>
#include <cstring>

enum { AB, AT, ATF, AREF, SRC, DST, SEN, SFN, SOF, SOZ, SOP, SREF, SMASK, ZB, CULL, DITH,
       BM00, BM10 = BM00 + 3, BM01 = BM10 + 3, BM11 = BM01 + 3, FMAG = BM11 + 3, FMIN = FMAG + 3, UVU = FMIN + 3, UVV = UVU + 3, NSTATE = UVV + 3 };
struct Mgr {
    unsigned long state[NSTATE]; int updates;
    void Set(unsigned long v, int s) { if (state[s] != v) { state[s] = v; ++updates; } }
    void SetF(float f, int s) { unsigned long v; std::memcpy(&v, &f, 4); Set(v, s); }
    void SetBump(long st, float m00, float m10, float m01, float m11) { SetF(m00, BM00 + st); SetF(m10, BM10 + st); SetF(m01, BM01 + st); SetF(m11, BM11 + st); }
};
struct Shader { bool vs, ps; float c[2][4]; int prepared; int ntex; const void* tex[3]; int drawn; };
static bool UseStencil = true;
static void ApplyDisplacement(Mgr& m, Shader& sh, float amt, const void* disp, const void* back)
{
    bool oldVS = sh.vs, oldPS = sh.ps; sh.vs = true; sh.ps = true;
    m.Set(1, AB); m.Set(1, AT); m.Set(7, ATF); m.Set(1, AREF); m.Set(5, SRC); m.Set(6, DST);
    if (UseStencil) { m.Set(1, SEN); m.Set(3, SFN); m.Set(1, SOF); m.Set(1, SOZ); m.Set(2, SOP); m.Set(1, SREF); m.Set(1, SMASK); }
    m.Set(0, ZB); m.Set(1, CULL); m.Set(0, DITH);
    for (long s = 0; s < 3; ++s) m.SetBump(s, amt, 0.0f, 0.0f, amt);
    float c0[4] = { amt, 0.0f, 0.0f, amt }; std::memcpy(sh.c[0], c0, sizeof c0);
    float off = amt * -0.5f - 0.001953125f; float c1[4] = { off, off, 0.0f, 0.0f }; std::memcpy(sh.c[1], c1, sizeof c1);
    for (long s = 0; s < 3; ++s) { m.Set(2, FMAG + s); m.Set(2, FMIN + s); m.Set(s == 2 ? 3 : 1, UVU + s); m.Set(s == 2 ? 3 : 1, UVV + s); }
    ++sh.prepared;
    sh.tex[0] = disp; sh.tex[1] = back; sh.tex[2] = back; sh.ntex = 3; ++sh.drawn;
    sh.vs = oldVS; sh.ps = oldPS;
}
int main()
{
    Mgr m; std::memset(&m, 0, sizeof m); Shader sh; std::memset(&sh, 0, sizeof sh);
    int a = 1, b = 2;
    ApplyDisplacement(m, sh, 0.25f, &a, &b);
    float v; std::memcpy(&v, &m.state[BM11 + 2], 4);
    bool ok = m.state[SEN] == 1 && m.state[SOP] == 2 && m.state[UVU + 2] == 3 && m.state[UVV + 1] == 1 && v == 0.25f
        && m.state[BM10] == 0 && sh.c[1][0] == 0.25f * -0.5f - 0.001953125f && sh.c[1][1] == sh.c[1][0] && sh.drawn == 1
        && sh.tex[0] == &a && sh.tex[2] == &b && !sh.vs && !sh.ps;
    int first = m.updates;
    ApplyDisplacement(m, sh, 0.25f, &a, &b);
    ok = ok && m.updates == first;                 // re-applying the same states changes nothing
    UseStencil = false; Mgr m2; std::memset(&m2, 0, sizeof m2);
    ApplyDisplacement(m2, sh, 0.5f, &a, &b);
    ok = ok && m2.state[SEN] == 0 && m2.state[SREF] == 0 && m2.state[AB] == 1;
    if (!ok) { std::printf("00b87870_TEST FAIL\n"); return 1; }
    std::printf("00b87870_TEST PASS\n");
    return 0;
}
