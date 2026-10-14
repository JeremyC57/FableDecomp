#include <cstdio>
static int g_bcopy = 0, g_bip = 0; static void* g_set = 0;
struct Bip { virtual ~Bip() {} void* item; Bip() : item(0) { ++g_bip; } void* GetPItem() const { return item; } void SetPItem(void* p) { item = p; g_set = p; } };
struct Ip : Bip { Ip() : Bip() { SetPItem(0); } Ip(const Ip& o) : Bip() { SetPItem(o.GetPItem()); } };
struct Base { virtual ~Base() {} Base() {} Base(const Base& o) { ++g_bcopy; } };
struct Act : Base { unsigned char pad[0xa4]; Ip t; long amount; Act() {} Act(const Act& o) : Base(o), t(o.t), amount(o.amount) {} };
int main()
{
    int item; Act a; a.t.SetPItem(&item); a.amount = 77; g_bip = 0; Act b(a);
    if (g_bcopy != 1 || g_bip != 1 || g_set != &item || b.t.item != &item || b.amount != 77) { std::printf("0084c9a0_TEST FAIL\n"); return 1; }
    std::printf("0084c9a0_TEST PASS\n");
    return 0;
}
