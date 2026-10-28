// behaviour test for 0x007ed780 CCreatureAction_ShowItemToThing::CCreatureAction_ShowItemToThing(CThingCreatureBase&, CThing&, CThing&):
// the base action uses speeds -1 / 0.75, the owner's random loaded "STANDARD_SHOW_OBJECT" animation, an empty sound pair and
// flags 0 / 0; the name temporary dies before the two intelligent pointers attach to the receiver and the item
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
struct Thing { int id; };
static const Anim& RandomAnim(const Owner&, const Str& name) { gAnim.name = name.s; return gAnim; }
struct Base {
    const Owner* owner; float f1, f2; const Anim* anim; long s1, s2, c, d; int liveStrings;
    Base(const Owner& o, float a, float b, const Anim& an, const Sound& snd, long cc, long dd)
        : owner(&o), f1(a), f2(b), anim(&an), s1(snd.a), s2(snd.b), c(cc), d(dd), liveStrings(gStrings) {}
};
struct Ptr {
    Thing* item; int liveStrings;
    Ptr(Thing& t) : item(&t), liveStrings(gStrings) {}
};
struct Action : Base {
    Ptr PReceivingThing;
    Ptr PItemBeingShown;
    Action(Owner& o, Thing& receiver, Thing& item)
        : Base(o, -1.0f, 0.75f, RandomAnim(o, "STANDARD_SHOW_OBJECT"), Sound(), 0, 0),
          PReceivingThing(receiver), PItemBeingShown(item) {}
};
int main()
{
    int fails = 0;
    Owner o; o.id = 3;
    Thing r; r.id = 4;
    Thing i; i.id = 5;
    Action act(o, r, i);
    if (act.owner != &o) fails |= 1;
    if (act.f1 != -1.0f || act.f2 != 0.75f) fails |= 2;
    if (act.anim != &gAnim || std::strcmp(gAnim.name, "STANDARD_SHOW_OBJECT") != 0) fails |= 4;
    if (act.s1 || act.s2 || act.c || act.d) fails |= 8;
    if (act.liveStrings != 1 || gStrings != 0) fails |= 16;
    if (act.PReceivingThing.item != &r || act.PItemBeingShown.item != &i) fails |= 32;
    if (act.PReceivingThing.liveStrings != 0) fails |= 64;   // the name is gone before the pointers attach
    if (fails) { std::printf("007ed780_TEST FAIL %d\n", fails); return 1; }
    std::printf("007ed780_TEST PASS\n");
    return 0;
}