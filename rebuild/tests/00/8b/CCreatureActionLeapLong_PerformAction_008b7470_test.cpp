#include <cstdio>
static long g_prop = -1;
struct Thing { unsigned char pad[0x28]; unsigned long flags; void OnPropertyChanged(long p) { g_prop = p; } };
struct Ptr { virtual ~Ptr() {} Thing* item; Thing* GetPItem() const { return item; } };
struct Base { virtual void PerformAction() {} unsigned char pad[4]; Ptr PCreature; unsigned char body[0x114 - 0x10]; };
struct Act : Base { unsigned char fin; void PerformAction() { if (PCreature.GetPItem()->flags & 0x2000) PCreature.GetPItem()->OnPropertyChanged(77); fin = 1; } };
int main()
{
    Thing t; t.flags = 0; Act a; a.fin = 0; a.PCreature.item = &t; a.PerformAction(); long p1 = g_prop; int f1 = a.fin; t.flags = 0x2000; a.PerformAction();
    if (p1 != -1 || f1 != 1 || g_prop != 77 || a.fin != 1) { std::printf("008b7470_TEST FAIL\n"); return 1; }
    std::printf("008b7470_TEST PASS\n");
    return 0;
}