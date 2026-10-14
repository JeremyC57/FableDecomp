#include <cstdio>
struct Sw { unsigned char pad[0x91]; unsigned char flags; void* user; Sw() : flags(0), user(0) {} void TriggerMe(void* u) { user = u; } };
struct Ptr { virtual ~Ptr() {} void* p; Sw* GetPItem() const { return (Sw*)p; } };
struct Act { virtual ~Act() {} unsigned char pad[4]; Ptr PCreature; unsigned char pad2[0x98]; Ptr PSwitch;
    void PerformAction() { Sw* s = PSwitch.GetPItem(); if (s != 0 && !(s->flags & 1)) s->TriggerMe(PCreature.GetPItem()); } };
int main()
{
    Sw s; int creature; Act a; a.PCreature.p = &creature; a.PSwitch.p = 0; a.PerformAction();
    if (s.user != 0) { std::printf("00842e80_TEST FAIL\n"); return 1; }
    a.PSwitch.p = &s; s.flags = 1; a.PerformAction();
    if (s.user != 0) { std::printf("00842e80_TEST FAIL\n"); return 1; }
    s.flags = 0; a.PerformAction();
    if (s.user != &creature) { std::printf("00842e80_TEST FAIL\n"); return 1; }
    std::printf("00842e80_TEST PASS\n");
    return 0;
}
