#include <cstdio>
#include <cstring>
// Behaviour of 0x00CA5510 (chunk 901 piece): calls operation 2 of the owner stored two words before the block, with the owner
struct SBlockOwner_Retail;
struct SBlockOps_Retail { void (*op0)(SBlockOwner_Retail*); void (*op1)(SBlockOwner_Retail*); void (*op2)(SBlockOwner_Retail*); };
struct SBlockOwner_Retail { SBlockOps_Retail* ops; long tag; };
static SBlockOwner_Retail* g_got = 0; static int g_which = 0;
static void op0(SBlockOwner_Retail*) { g_which = 1; }
static void op1(SBlockOwner_Retail*) { g_which = 2; }
static void op2(SBlockOwner_Retail* o) { g_which = 3; g_got = o; }
void func_00ca5510(void* block) { SBlockOwner_Retail* owner = reinterpret_cast<SBlockOwner_Retail**>(block)[-2]; owner->ops->op2(owner); }
int main()
{
    SBlockOps_Retail ops = { op0, op1, op2 }; SBlockOwner_Retail owner = { &ops, 7 };
    void* words[4]; words[0] = &owner; words[1] = 0; words[2] = 0; words[3] = 0;
    func_00ca5510(&words[2]);
    if (g_which != 3 || g_got != &owner) { std::printf("00ca5510_TEST FAIL\n"); return 1; }
    std::printf("00ca5510_TEST PASS\n"); return 0;
}