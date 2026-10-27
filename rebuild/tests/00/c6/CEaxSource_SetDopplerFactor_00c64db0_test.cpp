// behaviour test for 0x00C64DB0 CEaxSource::SetDopplerFactor (model: range check, change detection, dirty bit)
#include <cstdio>
struct Ctx { long err; };
struct Src {
    unsigned long dirty; Ctx* ctx; float cur, def;
    long Set(float v)
    {
        if (v < 0.0f || v > 10.0f) { ctx->err = -2; return -3; }
        if (v != cur) { cur = def = v; dirty |= 0x10000; }
        return 0;
    }
};
int main()
{
    Ctx c = { 0 }; Src s = { 0, &c, 0.0f, 0.0f };
    int ok = s.Set(0.5f) == 0 && s.cur == 0.5f && s.def == 0.5f && s.dirty == 0x10000;
    s.dirty = 0;
    ok = ok && s.Set(0.5f) == 0 && s.dirty == 0;
    ok = ok && s.Set(-1.0f) == -3 && c.err == -2 && s.cur == 0.5f;
    ok = ok && s.Set(10.0f * 2) == -3;
    if (!ok) { std::printf("00c64db0_TEST FAIL\n"); return 1; }
    std::printf("00c64db0_TEST PASS\n");
    return 0;
}