// Behaviour test for 0x00A3E4B0: request built only when the system exists and the manager is enabled.
#include <cstdio>
struct Req { int a, type, b, c, d, e; };
struct Player { int calls; Req last; void Play(const char*, const Req* r) { ++calls; last = *r; } };
struct Mgr { bool enabled; Player* sys;
  void F(const char* n, int e, int d, int b, int a, int c) { if (sys && enabled) { Req p; p.b = b; p.d = d; p.e = e; p.c = c; p.type = 4; p.a = a; sys->Play(n, &p); } } };
int main()
{
    Player pl; pl.calls = 0; Mgr m = {false, &pl};
    m.F("x", 1, 2, 3, 4, 5); bool ok = pl.calls == 0;
    m.enabled = true; m.F("x", 1, 2, 3, 4, 5);
    ok = ok && pl.calls == 1 && pl.last.a == 4 && pl.last.type == 4 && pl.last.b == 3 && pl.last.c == 5 && pl.last.d == 2 && pl.last.e == 1;
    m.sys = 0; m.F("x", 1, 2, 3, 4, 5); ok = ok && pl.calls == 1;
    if (!ok) { std::printf("00a3e4b0_TEST FAIL\n"); return 1; }
    std::printf("00a3e4b0_TEST PASS\n");
    return 0;
}
