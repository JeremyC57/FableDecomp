#include <cstdio>
static int g_base = 0;
struct Thing { long removed; Thing() : removed(0) {} void RemoveTC(long t) { removed = t; } };
struct Ptr { virtual ~Ptr() {} Thing* p; Thing* GetPItem() const { return p; } };
struct Base { virtual ~Base() {} virtual void FinishAction() { ++g_base; } Ptr PCreature; };
struct Act : Base { void FinishAction() { Base::FinishAction(); PCreature.GetPItem()->RemoveTC(200); } };
int main()
{
    Thing t; Act a; a.PCreature.p = &t; a.FinishAction();
    if (g_base != 1 || t.removed != 200) { std::printf("008c4020_TEST FAIL\n"); return 1; }
    std::printf("008c4020_TEST PASS\n");
    return 0;
}