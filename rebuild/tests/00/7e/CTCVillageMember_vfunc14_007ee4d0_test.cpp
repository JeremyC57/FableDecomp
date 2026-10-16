// Behaviour test for CTCVillageMember::vfunc_14 (0x007EE4D0): with a village thing, its village component removes the
// owner; without one nothing happens.
#include <cstdio>
struct Village { void* removed; int calls; };
struct Member {
    void* ParentThing; Village* village;
    void Remove() { if (village) { village->removed = ParentThing; ++village->calls; } }
};
int main()
{
    int owner; Village v = { 0, 0 };
    Member a = { &owner, &v }; a.Remove();
    Member b = { &owner, 0 }; b.Remove();
    if (v.removed != &owner || v.calls != 1) { std::printf("007ee4d0_TEST FAIL\n"); return 1; }
    std::printf("007ee4d0_TEST PASS\n");
    return 0;
}