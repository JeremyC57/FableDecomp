// Behaviour test for 00f079d0 (see src/00f079d0.cpp): CAT_BanditAlly::SwitchBanditAllegience: the interface call sequence and argument order; plain C++98, member access only.
#include <cstdio>
#include <cstring>
// shared test model: a thing with an interface table (sorted type -> component), a dead bit, and an
// intelligent pointer; mirrors the HasTC / GetInterface / FindEntry shapes of the sources.
struct TThing {
    unsigned long bits[9];
    long types[8];
    void* comps[8];
    int count;
    unsigned char flags91;
    unsigned long flags6c;
    TThing() : count(0), flags91(0), flags6c(0) { for (int i = 0; i < 9; ++i) bits[i] = 0; }
    void Add(long type, void* comp) { types[count] = type; comps[count] = comp; ++count; bits[type >> 5] |= 1UL << (type & 31); }
    unsigned long HasTC(long type) const { return bits[type >> 5] & (1UL << (type & 31)); }
    void* Find(long type) const { for (int i = 0; i < count; ++i) if (types[i] == type) return comps[i]; return 0; }
    template<class T> bool GetInterface(T** out, long type) const { if (!HasTC(type)) return false; *out = (T*)Find(type); return true; }
};
struct TPtr {
    TThing* item;
    TPtr() : item(0) { }
    TThing* GetPItem() const { return item; }
    void SetPItem(TThing* t) { item = t; }
};
inline bool Alive(const TPtr& p) { return p.GetPItem() != 0 && !(p.GetPItem()->flags91 & 1); }
#define CHECK(c, addr) do { if (!(c)) { std::printf("%s_TEST FAIL\n", addr); return 1; } } while (0)

struct TThingRef { int id; };
struct TGSI { char log[256]; TThingRef hero;
    TGSI() { log[0] = 0; hero.id = 99; }
    void Add(const char* s) { std::strcat(log, s); }
    TThingRef& GetHero() { Add("H"); return hero; }
    void ClearThingHasInformation(const TThingRef& t) { char b[16]; std::sprintf(b, "C%d;", t.id); Add(b); }
    void EntityUnsetThingAsAllyOfThing(const TThingRef& a, const TThingRef& b) { char s[16]; std::sprintf(s, "U%d,%d;", a.id, b.id); Add(s); }
    void EntitySetThingAsEnemyOfThing(const TThingRef& a, const TThingRef& b) { char s[16]; std::sprintf(s, "E%d,%d;", a.id, b.id); Add(s); }
    void RemoveQuestInfoElement(long e) { char s[16]; std::sprintf(s, "R%ld;", e); Add(s); }
    void EntityStopFollowing(const TThingRef& t) { char s[16]; std::sprintf(s, "S%d;", t.id); Add(s); }
    void EntitySetAsAllowedToFollowHero(const TThingRef& t, bool b) { char s[16]; std::sprintf(s, "F%d,%d;", t.id, b ? 1 : 0); Add(s); } };
struct TAction { TGSI* gsi; TThingRef thing; long element;
    void Switch() {
        gsi->ClearThingHasInformation(thing);
        gsi->EntityUnsetThingAsAllyOfThing(thing, gsi->GetHero());
        gsi->EntityUnsetThingAsAllyOfThing(gsi->GetHero(), thing);
        gsi->EntitySetThingAsEnemyOfThing(thing, gsi->GetHero());
        gsi->EntitySetThingAsEnemyOfThing(gsi->GetHero(), thing);
        gsi->RemoveQuestInfoElement(element);
        gsi->EntityStopFollowing(thing);
        gsi->EntitySetAsAllowedToFollowHero(thing, false); } };
int main() {
    TGSI gsi; TAction a; a.gsi = &gsi; a.thing.id = 7; a.element = 4; a.Switch();
    CHECK(std::strcmp(gsi.log, "C7;HU7,99;HU99,7;HE7,99;HE99,7;R4;S7;F7,0;") == 0, "00f079d0");
    std::printf("00f079d0_TEST PASS\n"); return 0; }