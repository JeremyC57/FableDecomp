// Test: CAIStateGroup_NymphFloatAround::IsRunnable -- adds "CTCNymph" when interface 0xF5 is missing, runnable while it is present
#include <cstdio>
#include <cstring>
struct CCreature {
    bool hasNymph; bool canAdd; int adds;
    bool HasTC(long t) const { return t == 0xF5 && hasNymph; }
    void AddTC(const char* name, bool, long) { ++adds; if (canAdd && !std::strcmp(name, "CTCNymph")) hasNymph = true; }
};
static bool IsRunnable(CCreature* c)
{
    if (!c->HasTC(0xF5)) c->AddTC("CTCNymph", false, 0);
    return c->HasTC(0xF5);
}
int main()
{
    CCreature a = { false, true, 0 }, b = { true, true, 0 }, c = { false, false, 0 };
    if (!IsRunnable(&a) || a.adds != 1 || !IsRunnable(&b) || b.adds != 0 || IsRunnable(&c) || c.adds != 1) { std::printf("0091c0d0_TEST FAIL\n"); return 1; }
    std::printf("0091c0d0_TEST PASS\n");
    return 0;
}
