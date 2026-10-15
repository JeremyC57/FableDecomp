// Behaviour test for 0x00A0B710: no device -> nothing; device -> created with the converted name and the fixed descriptor.
#include <cstdio>
struct Desc { long a, b; bool f[7]; };
struct Device { int calls; Desc last; long Create(const char*, const Desc* d) { ++calls; last = *d; return 77; } };
struct Owner { Device* dev; };
struct Mgr { Owner* owner; long handle;
  void Update(const char* name) {
    if (owner->dev) { Desc d; d.a = 2; d.b = 2; for (int i = 0; i < 7; ++i) d.f[i] = false; d.f[2] = true;
      handle = owner->dev->Create(name, &d); } } };
int main()
{
    Owner o = {0}; Mgr m = {&o, -1};
    m.Update("x");
    bool ok = m.handle == -1;
    Device dv; dv.calls = 0; o.dev = &dv; m.Update("x");
    ok = ok && m.handle == 77 && dv.calls == 1 && dv.last.a == 2 && dv.last.b == 2 && dv.last.f[2] && !dv.last.f[0] && !dv.last.f[6];
    if (!ok) { std::printf("00a0b710_TEST FAIL\n"); return 1; }
    std::printf("00a0b710_TEST PASS\n");
    return 0;
}