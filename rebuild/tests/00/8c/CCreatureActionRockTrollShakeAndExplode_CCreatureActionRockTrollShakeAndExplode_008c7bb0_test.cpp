// behaviour test for 0x008C7BB0: the base receives the owner, action code 250 and an empty sound pair
#include <cstdio>
struct Base { void* owner; long code; long s0, s1; Base(void* o, long c, long a, long b) : owner(o), code(c), s0(a), s1(b) { } };
struct Action : Base { Action(void* o) : Base(o, 250, 0, 0) { } };
int main()
{
    int owner; Action x(&owner);
    if (x.owner != &owner || x.code != 250 || x.s0 != 0 || x.s1 != 0) { std::printf("008c7bb0_TEST FAIL\n"); return 1; }
    std::printf("008c7bb0_TEST PASS\n");
    return 0;
}