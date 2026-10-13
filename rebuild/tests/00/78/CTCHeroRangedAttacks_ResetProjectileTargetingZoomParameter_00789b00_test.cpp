#include <cstdio>

struct Def { unsigned char pad[0x136]; bool In; };
struct R { unsigned char pad[0x10]; Def* def; unsigned char p2[0x2c]; float Zoom;
    void Reset() { Def* d = def; if (d && d->In) { Zoom = 0.0f; return; } Zoom = 1.0f; } };
int main()
{
    Def d; R r; r.def = 0; r.Reset(); if (r.Zoom != 1.0f) { std::printf("00789b00_TEST FAIL\n"); return 1; }
    d.In = true; r.def = &d; r.Reset(); if (r.Zoom != 0.0f) { std::printf("00789b00_TEST FAIL\n"); return 1; }
    d.In = false; r.Reset(); if (r.Zoom != 1.0f) { std::printf("00789b00_TEST FAIL\n"); return 1; }
    std::printf("00789b00_TEST PASS\n");
    return 0;
}