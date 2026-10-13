#include <cstdio>

struct F { bool Active; unsigned char rest[0x1b]; float GetAlpha() { return 0.25f; } };
struct I { unsigned char pad[0x160]; F* First; float Get(long i) { F* f = &First[i]; if (!f->Active) return 1.0f; return f->GetAlpha(); } };
int main()
{
    F fs[3]; fs[2].Active = false; fs[1].Active = true; I inv; inv.First = fs;
    if (inv.Get(2) != 1.0f || inv.Get(1) != 0.25f) { std::printf("00581d2a_TEST FAIL\n"); return 1; }
    std::printf("00581d2a_TEST PASS\n");
    return 0;
}
