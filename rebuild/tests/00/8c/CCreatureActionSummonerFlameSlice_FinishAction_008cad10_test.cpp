#include <cstdio>
struct Thing { unsigned char pad[0x91]; unsigned char flags; int killed; Thing() : flags(0), killed(0) {} void Kill(bool s) { killed += s ? 1 : 10; } };
struct Ptr { virtual ~Ptr() {} Thing* p; Thing* GetPItem() const { return p; } };
inline bool ALIVE(const Ptr& p) { if (p.GetPItem() != 0) { if (!(p.GetPItem()->flags & 1)) return true; } return false; }
static int g_base = 0;
struct Base { virtual ~Base() {} virtual void FinishAction() { ++g_base; } unsigned char body[0x110]; };
struct Act : Base { Ptr P; void FinishAction() { Base::FinishAction(); if (ALIVE(P)) P.GetPItem()->Kill(true); } };
int main()
{
    Thing t; Act a; a.P.p = 0; a.FinishAction();
    a.P.p = &t; a.FinishAction(); t.flags = 1; a.FinishAction();
    if (t.killed != 1 || g_base != 3) { std::printf("008cad10_TEST FAIL\n"); return 1; }
    std::printf("008cad10_TEST PASS\n");
    return 0;
}