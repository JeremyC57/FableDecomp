#include <cstdio>
// Behaviour model: out-of-range values fail with -3 and record -2; a changed value is stored and marked dirty.
struct Ctx { long err; };
struct Src { unsigned long dirty; Ctx* ctx; float v;
    long Set(float r) { if (r < 0.0f || r > 10.0f) { ctx->err = -2; return -3; } if (r != v) { v = r; dirty |= 0x400000; } return 0; } };
int main()
{
    Ctx c = { 0 }; Src s = { 0, &c, 0.0f };
    if (s.Set(-0.5f) != -3 || c.err != -2 || s.dirty) { std::printf("00c64c10_TEST FAIL\n"); return 1; }
    if (s.Set(10.0f + 1.0f) != -3) { std::printf("00c64c10_TEST FAIL\n"); return 1; }
    if (s.Set(0.0f) != 0 || s.dirty) { std::printf("00c64c10_TEST FAIL\n"); return 1; }
    if (s.Set(0.5f) != 0 || s.v != 0.5f || s.dirty != 0x400000) { std::printf("00c64c10_TEST FAIL\n"); return 1; }
    std::printf("00c64c10_TEST PASS\n");
    return 0;
}