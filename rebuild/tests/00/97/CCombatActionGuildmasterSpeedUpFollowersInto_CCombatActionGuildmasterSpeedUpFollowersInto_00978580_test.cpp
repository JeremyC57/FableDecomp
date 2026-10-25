// behaviour test for 0x00978580 CCombatAction_Guildmaster_SpeedUpFollowersInto::CCombatAction_Guildmaster_SpeedUpFollowersInto(CThingCreatureBase&): the combat base is built for the owner with
// strike value 150 and the owner's "SWORD_SWING" sound pair; the name temporary lives across the base call, then dies
#include <cstdio>
#include <cstring>
static int gStrings = 0;
struct Str {
    const char* s;
    Str(const char* t) : s(t) { ++gStrings; }
    ~Str() { --gStrings; }
};
struct Sound { long a, b; Sound() : a(0), b(0) {} };
struct Owner { int id; };
static const char* gAsked = 0;
static Sound SoundFor(const Owner& o, const Str& name) { gAsked = name.s; Sound s; s.a = o.id; s.b = o.id + 1; return s; }
struct Base {
    const Owner* owner; long n; long s1, s2; int liveStrings;
    Base(const Owner& o, long nn, const Sound& snd) : owner(&o), n(nn), s1(snd.a), s2(snd.b), liveStrings(gStrings) {}
};
struct Action : Base {
    Action(const Owner& o) : Base(o, 150, SoundFor(o, "SWORD_SWING")) {}
};
int main()
{
    int fails = 0;
    Owner o; o.id = 7;
    Action act(o);
    if (act.owner != &o) fails |= 1;
    if (act.n != 150) fails |= 2;
    if (!gAsked || std::strcmp(gAsked, "SWORD_SWING") != 0 || act.s1 != 7 || act.s2 != 8) fails |= 4;
    if (act.liveStrings != 1 || gStrings != 0) fails |= 8;
    if (fails) { std::printf("00978580_TEST FAIL %d\n", fails); return 1; }
    std::printf("00978580_TEST PASS\n");
    return 0;
}