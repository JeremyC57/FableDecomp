// Behaviour test for CTCCarriedActionUseGive::vfunc_28 (0x007ED170).
#include <cstdio>

static int g_gives = 0, g_failed = 0; static void* g_receiver = 0; static void* g_item = 0;
struct User { bool hasTargeting; bool targetIsCreature; bool canInteract; void* target; };
struct Give {
    void* ParentThing; bool parentHas1A;
    void Use(User& u)
    {
        if (u.hasTargeting && u.targetIsCreature && u.canInteract) {
            ++g_gives; g_receiver = u.target; g_item = ParentThing;
            return;
        }
        if (parentHas1A) ++g_failed;
    }
};

int main()
{
    int item, rcv;
    Give g = { &item, true };
    User u = { true, true, true, &rcv };
    g.Use(u);
    if (g_gives != 1 || g_receiver != &rcv || g_item != &item || g_failed) { std::printf("007ed170_TEST FAIL\n"); return 1; }
    u.canInteract = false; g.Use(u);
    if (g_gives != 1 || g_failed != 1) { std::printf("007ed170_TEST FAIL\n"); return 1; }
    u.hasTargeting = false; g.parentHas1A = false; g.Use(u);
    if (g_failed != 1) { std::printf("007ed170_TEST FAIL\n"); return 1; }
    std::printf("007ed170_TEST PASS\n");
    return 0;
}