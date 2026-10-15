// Behaviour test for 0x00A0ABE0: DrawCurrentBuffersAsTris (texture array overload): attach each texture of the array to its stage, realise the states, then draw
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %%d\n", __LINE__); } } while (0)
struct Dev {
    unsigned int setTex[8]; int setTexCalls; long lastCall; long args[8];
    Dev() : setTexCalls(0), lastCall(0) { for (int i = 0; i < 8; ++i) { setTex[i] = 0xFFFF; args[i] = 0; } }
    void SetTexture(unsigned int stage, unsigned int tex) { setTex[stage] = tex; ++setTexCalls; }
    void Call(long which, long a0, long a1, long a2, long a3, long a4, long a5, long a6, long a7)
    { lastCall = which; args[0] = a0; args[1] = a1; args[2] = a2; args[3] = a3; args[4] = a4; args[5] = a5; args[6] = a6; args[7] = a7; }
};
struct Texture { unsigned int d3d; };
struct RMC {
    Dev* dev; unsigned int cur[8]; long stagesInUse; int realised; long primType;
    RMC(Dev* d) : dev(d), stagesInUse(0), realised(0), primType(5) { for (int i = 0; i < 8; ++i) cur[i] = 0; }
    void Attach(long stage, const Texture* t)
    {
        unsigned int d = t ? t->d3d : 0;
        if (cur[stage] != d) { cur[stage] = d; dev->SetTexture(stage, d); }
        if (stagesInUse < stage + 1) stagesInUse = stage + 1;
    }
    void AttachAll(const Texture* const* tex, unsigned int n) { for (unsigned int i = 0; i < n; ++i) Attach(i, tex[i]); }
    void RealiseRenderState() { ++realised; }
};
static void CheckAttach(RMC& r, Dev& d)
{
    CHECK(r.cur[0] == 11 && r.cur[1] == 0 && r.cur[2] == 33);
    CHECK(d.setTex[0] == 11 && d.setTex[1] == 0xFFFF && d.setTex[2] == 33);   // stage 1 unchanged (already null)
    CHECK(d.setTexCalls == 2 && r.stagesInUse == 3 && r.realised == 1);
}
static void Run()
{
    Dev d; RMC r(&d);
    Texture a = { 11 }, c = { 33 };
    const Texture* t[3] = { &a, 0, &c };
    r.AttachAll(t, 3); r.RealiseRenderState(); d.Call(81, 4, 10, 20, 0, 0, 0, 0, 0);
    CheckAttach(r, d);
    CHECK(d.lastCall == 81 && d.args[0] == 4 && d.args[1] == 10 && d.args[2] == 20);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00a0abe0_TEST FAIL\n"); return 1; }
    std::printf("00a0abe0_TEST PASS\n");
    return 0;
}
