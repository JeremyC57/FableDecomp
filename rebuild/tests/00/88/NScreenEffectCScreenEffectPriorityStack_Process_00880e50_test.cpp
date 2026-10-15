// Behaviour test for 0x00880E50: CScreenEffectPriorityStack::Process: the three stacks are processed in order, each with a callback object bound to the owner
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %%d\n", __LINE__); } } while (0)
struct Owner;
struct Proc { virtual ~Proc() {} virtual int Kind() const = 0; Owner* owner; };
struct RadialProc : Proc { RadialProc(Owner* o) { owner = o; } int Kind() const { return 1; } };
struct DispProc : Proc { DispProc(Owner* o) { owner = o; } int Kind() const { return 2; } };
struct FilterProc : Proc { FilterProc(Owner* o) { owner = o; } int Kind() const { return 3; } };
struct Owner {
    int stacks[3]; int log[3]; Owner* owners[3]; int n;
    Owner() : n(0) {}
    void ProcessStack(int* stack, Proc* p) { log[n] = p->Kind() * 10 + (int)(stack - stacks); owners[n] = p->owner; ++n; }
    void Process()
    {
        RadialProc r(this); ProcessStack(&stacks[0], &r);
        DispProc d(this); ProcessStack(&stacks[1], &d);
        FilterProc f(this); ProcessStack(&stacks[2], &f);
    }
};
static void Run()
{
    Owner o; o.Process();
    CHECK(o.n == 3 && o.log[0] == 10 && o.log[1] == 21 && o.log[2] == 32);
    CHECK(o.owners[0] == &o && o.owners[1] == &o && o.owners[2] == &o);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00880e50_TEST FAIL\n"); return 1; }
    std::printf("00880e50_TEST PASS\n");
    return 0;
}