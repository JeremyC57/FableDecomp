// behaviour test for 0x00843700 CCreatureAction_PickUpGenericBox constructor: the base receives (owner, -1.0, 0.0, the animation looked up by the name "ST_PICK_UP_GENERIC_BOX", a zeroed sound pair, 0, 0), the name temporary is released after the base call, then PThingToPickUp points at the target
#include <cstdio>
#include <cstring>
static int gLog[8]; static int gN = 0;
struct Name { const char* s; Name(const char* t) : s(t) { gLog[gN++] = 1; } ~Name() { gLog[gN++] = 3; } };
struct Sound { long a, b; Sound() : a(0), b(0) { } };
static const char* gLooked = 0;
static int gAnim = 77;
static const int& Lookup(void*, const Name& n) { gLooked = n.s; gLog[gN++] = 2; return gAnim; }
struct Base {
    void* owner; float f0, f1; const int* anim; long s0, s1, c, d;
    Base(void* o, float a, float b, const int& an, const Sound& s, long c_, long d_) : owner(o), f0(a), f1(b), anim(&an), s0(s.a), s1(s.b), c(c_), d(d_) { gLog[gN++] = 4; }
};
struct Ptr { void* item; Ptr(void* p) : item(p) { gLog[gN++] = 5; } };
struct Action : Base {
    Ptr target;
    Action(void* owner, void* t) : Base(owner, -1.0f, 0.0f, Lookup(owner, Name("ST_PICK_UP_GENERIC_BOX")), Sound(), 0, 0), target(t) { }
};
int main()
{
    int owner = 0, thing = 0, fails = 0;
    Action act(&owner, &thing);
    if (act.owner != &owner || act.f0 != -1.0f || act.f1 != 0.0f || act.anim != &gAnim) fails |= 1;
    if (act.s0 != 0 || act.s1 != 0 || act.c != 0 || act.d != 0) fails |= 2;
    if (!gLooked || std::strcmp(gLooked, "ST_PICK_UP_GENERIC_BOX") != 0) fails |= 4;
    if (act.target.item != &thing) fails |= 8;
    const int order[5] = { 1, 2, 4, 3, 5 };                  // name, lookup, base, ~name, member
    if (gN != 5 || std::memcmp(gLog, order, sizeof(order)) != 0) fails |= 16;
    if (fails) { std::printf("00843700_TEST FAIL %d\n", fails); return 1; }
    std::printf("00843700_TEST PASS\n");
    return 0;
}