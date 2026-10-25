// behaviour test for 0x00842a80 CCreatureAction_StartFillingTankard::CCreatureAction_StartFillingTankard(CThingCreatureBase&): the base action is built for the owner with speed -1, no end
// time, the owner's random loaded "FILL_GLASS_INTO" animation, an empty sound pair and flags 0 / 0; the name temporary is released
#include <cstdio>
#include <cstring>
static int gStrings = 0;
struct Str {
    const char* s;
    Str(const char* t) : s(t) { ++gStrings; }
    ~Str() { --gStrings; }
};
struct Sound { long a, b; Sound() : a(0), b(0) {} };
struct Anim { const char* name; };
static Anim gAnim;
struct Owner { int id; };
static const Anim& RandomAnim(const Owner&, const Str& name) { gAnim.name = name.s; return gAnim; }
struct Base {
    const Owner* owner; float f1, f2; const Anim* anim; long s1, s2, c, d; int liveStrings;
    Base(const Owner& o, float a, float b, const Anim& an, const Sound& snd, long cc, long dd)
        : owner(&o), f1(a), f2(b), anim(&an), s1(snd.a), s2(snd.b), c(cc), d(dd), liveStrings(gStrings) {}
};
struct Action : Base {
    Action(const Owner& o) : Base(o, -1.0f, 0.0f, RandomAnim(o, "FILL_GLASS_INTO"), Sound(), 0, 0) {}
};
int main()
{
    int fails = 0;
    Owner o; o.id = 3;
    Action act(o);
    if (act.owner != &o) fails |= 1;
    if (act.f1 != -1.0f || act.f2 != 0.0f) fails |= 2;
    if (act.anim != &gAnim || std::strcmp(gAnim.name, "FILL_GLASS_INTO") != 0) fails |= 4;
    if (act.s1 || act.s2 || act.c || act.d != 0) fails |= 8;
    if (act.liveStrings != 1 || gStrings != 0) fails |= 16;   // the name lives across the base call, then dies
    if (fails) { std::printf("00842a80_TEST FAIL %d\n", fails); return 1; }
    std::printf("00842a80_TEST PASS\n");
    return 0;
}