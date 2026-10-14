#include <cstdio>
struct Thing { unsigned char pad[0x91]; unsigned char flags; int killed; Thing() : flags(0), killed(0) {} void Kill(bool s) { killed += s ? 1 : 10; } };
struct Ptr { virtual ~Ptr() {} Thing* p; Thing* GetPItem() const { return p; } };
inline bool ALIVE(const Ptr& p) { if (p.GetPItem() != 0) { if (!(p.GetPItem()->flags & 1)) return true; } return false; }
struct Act { virtual ~Act() {} unsigned char pad[0xa4]; Ptr PItem; void PerformAction() { if (ALIVE(PItem)) PItem.GetPItem()->Kill(true); } };
int main()
{
    Thing t; Act a; a.PItem.p = 0; a.PerformAction();
    a.PItem.p = &t; a.PerformAction(); t.flags = 1; a.PerformAction();
    if (t.killed != 1) { std::printf("008446e0_TEST FAIL\n"); return 1; }
    std::printf("008446e0_TEST PASS\n");
    return 0;
}