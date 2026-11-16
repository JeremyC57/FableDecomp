// Behaviour test for 0x00682810 CTCCreatureOpinionOfHero::InformOfDeed(type, handle, def, flag): no hero / no deed log -> nothing;
// a shop keeper in a crime region witnesses the crime deed (types 1..9 the deed itself, 0x35 its ExtraData crime); a guard in a
// crime region witnesses it unless bribed (frame < village bribe end) and the crime was not against him; a guard outside a crime
// region raises the village's guard enmity by 0.2 x the crime fine unless severity is 1 (type 0x35 uses type 8's effects); a
// non-guard seeing a crime returns early on a severity-1 crime when the village calls the hero criminal, and turns enemy (attitude 3)
// on a severity-3 crime when its personality attacks criminals; a flirt seen by the spouse of a female (sex 2) villager or by a
// living wife posts a jealousy deed (0x4b / 0x55, the latter not in marriage state 5); violence (7, 8, 9, 0x35) informs a wife and
// a boasting crowd; an unknown deed stops here; otherwise the deed is queued before the first pending deed of equal or higher
// reaction priority (vanished pending deeds dropped), and effects apply (rate-limited per type; never for a courting flirt).
#include <cstdio>
#include <list>
#include <map>
#include <vector>
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
struct Deed { long type, extra; bool crime; };
struct Fx { long severity; float fine; bool flirting; long priority; };
struct W {
    bool hero, log, useCrimes, limbo, shop, guard, bribed, againstMe, attacksCriminals, heroCriminal, village;
    bool heroAlive, villager, married, targetMarried, female, spouseSees, courtable, courtingFemale;
    std::map<long, Deed> deeds; std::map<long, Fx> fx; std::vector<int> wifeStates; std::vector<bool> wifeSees;
    long minSeconds, frame, fps, last[0x60]; int effects; bool flag, enableUnknown, markerAlive;
    bool wife, crowdMember;
    // outputs
    std::vector<long> shopSaw, guardSaw, posted; long enmity; bool enemy; int violence, crowdHits, offsets, markerMoves;
    std::list<long> pending;
    W() : hero(true), log(true), useCrimes(true), limbo(false), shop(false), guard(false), bribed(false), againstMe(false),
          attacksCriminals(false), heroCriminal(false), village(true), heroAlive(true), villager(true), married(false),
          targetMarried(false), female(false), spouseSees(false), courtable(false), courtingFemale(false), minSeconds(0), frame(1000),
          fps(30), effects(2), flag(false), enableUnknown(false), markerAlive(true), wife(false), crowdMember(false), enmity(-1),
          enemy(false), violence(0), crowdHits(0), offsets(0), markerMoves(0) { for (int i = 0; i < 0x60; ++i) last[i] = -100000; }
    const Deed* Get(long h) { std::map<long, Deed>::iterator i = deeds.find(h); return i == deeds.end() ? 0 : &i->second; }
};
// Mirror of the row's decisions (same statement order as src/00682810.cpp).
static void Inform(W& w, long type, long handle, bool defFlirting) {
    if (!w.hero || !w.log) return;
    bool crimes = w.useCrimes;
    if (w.village && w.limbo) crimes = false;
    if (w.shop && crimes) {
        long ch = -1; bool go = true;
        if (type > 0 && type < 10) ch = handle;
        else if (type == 0x35) { const Deed* d = w.Get(handle); if (!d) go = false; else ch = d->extra; }
        else go = false;
        if (go && ch != -1) { const Deed* c = w.Get(ch); if (c && c->crime) w.shopSaw.push_back(ch); }
    }
    if (w.guard) {
        if (!crimes) {
            const Deed* d = w.Get(handle);
            if (d) {
                const Fx* e = 0;
                if (d->type > 0 && d->type < 10) e = &w.fx[d->type];
                if (d->type == 0x35) e = &w.fx[8];
                if (e && e->severity != 1 && w.village) w.enmity = (long)(e->fine * 0.2f);
            }
        } else {
            long ch = -1; bool go = true;
            if (type > 0 && type < 10) ch = handle;
            else if (type == 0x35) ch = w.Get(handle)->extra;
            else go = false;
            if (go && ch != -1) {
                const Deed* c = w.Get(ch);
                if (c && c->crime) {
                    if (w.bribed && !w.againstMe) return;
                    w.guardSaw.push_back(ch);
                }
            }
        }
    } else {
        const Deed* d = w.Get(handle);
        if (d && d->crime) {
            const Fx& e = w.fx[d->type];
            if (e.severity == 1) { if (w.village && w.heroCriminal) return; }
            else if (w.attacksCriminals && e.severity == 3) w.enemy = true;
        }
    }
    if (defFlirting && w.heroAlive && w.villager && !w.married && !w.targetMarried) {
        if (w.female && w.spouseSees) w.posted.push_back(0x4b);
        if (w.courtable)
            for (size_t i = 0; i < w.wifeStates.size(); ++i)
                if (w.wifeSees[i] && w.wifeStates[i] != 5) w.posted.push_back(0x55);
    }
    if (type == 7 || type == 8 || type == 9 || type == 0x35) {
        if (w.heroAlive && w.villager && w.wife) ++w.violence;
        if (w.villager && w.crowdMember) ++w.crowdHits;
    }
    if (!w.Get(handle)) return;
    bool apply = true;
    if (w.minSeconds > 0) {
        long gap = w.minSeconds * w.fps; long since = gap >= w.frame ? 0 : w.frame - gap;
        apply = w.last[type] <= since;
    }
    if (apply && w.fx[type].flirting && w.courtingFemale) apply = false;
    long prio = w.fx[type].priority;
    std::list<long>::iterator it = w.pending.begin();
    for (;;) {
        if (it == w.pending.end()) { w.pending.push_back(handle); break; }
        const Deed* p = w.Get(*it);
        if (!p) { it = w.pending.erase(it); continue; }
        if (w.fx[p->type].priority >= prio) { w.pending.insert(it, handle); break; }
        ++it;
    }
    if (apply) { w.last[type] = w.frame; w.offsets += w.effects; }
    if (w.enableUnknown && w.flag && w.markerAlive) ++w.markerMoves;
}
static W Base() { W w; w.deeds[10] = Deed(); w.deeds[10].type = 3; w.deeds[10].extra = -1; w.deeds[10].crime = true;
    Fx f = { 2, 50.0f, false, 5 }; for (long t = 0; t < 0x60; ++t) w.fx[t] = f; return w; }
int main() {
    { W w = Base(); w.log = false; Inform(w, 3, 10, false); check(w.pending.empty() && w.offsets == 0, "no deed log: nothing"); }
    { W w = Base(); w.shop = true; Inform(w, 3, 10, false); check(w.shopSaw.size() == 1 && w.shopSaw[0] == 10, "shop keeper witnesses"); }
    { W w = Base(); w.shop = true; w.useCrimes = false; Inform(w, 3, 10, false); check(w.shopSaw.empty(), "no crimes region: shop ignores"); }
    { W w = Base(); w.shop = true; w.deeds[11].type = 0x35; w.deeds[11].extra = 10; w.deeds[11].crime = false; Inform(w, 0x35, 11, false);
      check(w.shopSaw.size() == 1 && w.shopSaw[0] == 10, "0x35 reports its ExtraData crime"); }
    { W w = Base(); w.guard = true; w.bribed = true; Inform(w, 3, 10, false); check(w.guardSaw.empty() && w.pending.empty(), "bribed guard returns"); }
    { W w = Base(); w.guard = true; w.bribed = true; w.againstMe = true; Inform(w, 3, 10, false); check(w.guardSaw.size() == 1, "crime against the bribed guard"); }
    { W w = Base(); w.guard = true; w.useCrimes = false; Inform(w, 3, 10, false); check(w.enmity == 10, "guard outside crimes: 0.2 x fine"); }
    { W w = Base(); w.guard = true; w.useCrimes = false; w.fx[3].severity = 1; Inform(w, 3, 10, false); check(w.enmity == -1, "severity 1: no enmity"); }
    { W w = Base(); w.guard = true; w.useCrimes = false; w.deeds[10].type = 0x35; w.fx[8].fine = 100.0f; Inform(w, 3, 10, false); check(w.enmity == 20, "0x35 uses type 8 effects"); }
    { W w = Base(); w.fx[3].severity = 1; w.heroCriminal = true; Inform(w, 3, 10, false); check(w.pending.empty(), "criminal hero, severity 1: return"); }
    { W w = Base(); w.fx[3].severity = 3; w.attacksCriminals = true; Inform(w, 3, 10, false); check(w.enemy && w.pending.size() == 1, "severity 3: enemy"); }
    { W w = Base(); w.female = true; w.spouseSees = true; w.courtable = true; w.wifeStates.push_back(1); w.wifeStates.push_back(5);
      w.wifeSees.push_back(true); w.wifeSees.push_back(true); Inform(w, 20, 10, true);
      check(w.posted.size() == 2 && w.posted[0] == 0x4b && w.posted[1] == 0x55, "flirt: spouse + one wife (state 5 skipped)"); }
    { W w = Base(); w.female = true; w.spouseSees = true; w.married = true; Inform(w, 20, 10, true); check(w.posted.empty(), "married to the player: no jealousy"); }
    { W w = Base(); w.wife = true; w.crowdMember = true; Inform(w, 8, 10, false); check(w.violence == 1 && w.crowdHits == 1, "violence informs wife + crowd"); }
    { W w = Base(); w.wife = true; Inform(w, 6, 10, false); check(w.violence == 0, "type 6 is not violence"); }
    { W w = Base(); Inform(w, 3, 99, false); check(w.pending.empty() && w.offsets == 0, "unknown deed: stop"); }
    { W w = Base(); w.deeds[20].type = 4; w.deeds[21].type = 5; w.fx[4].priority = 1; w.fx[5].priority = 9; w.pending.push_back(20);
      w.pending.push_back(77); w.pending.push_back(21); Inform(w, 3, 10, false);
      std::list<long>::iterator i = w.pending.begin();
      check(w.pending.size() == 3 && *i++ == 20 && *i++ == 10 && *i == 21, "priority insert, vanished pending dropped"); }
    { W w = Base(); w.minSeconds = 10; w.last[3] = 900; Inform(w, 3, 10, false); check(w.offsets == 0 && w.pending.size() == 1, "rate limited"); }
    { W w = Base(); w.minSeconds = 10; w.last[3] = 700; Inform(w, 3, 10, false); check(w.offsets == 2 && w.last[3] == 1000, "rate window passed"); }
    { W w = Base(); w.fx[3].flirting = true; w.courtingFemale = true; Inform(w, 3, 10, false); check(w.offsets == 0, "courting flirt: no effects"); }
    { W w = Base(); w.enableUnknown = true; w.flag = true; Inform(w, 3, 10, false); check(w.markerMoves == 1, "unknown threat marker"); }
    if (fails == 0) std::printf("00682810_TEST PASS\n");
    return fails;
}
