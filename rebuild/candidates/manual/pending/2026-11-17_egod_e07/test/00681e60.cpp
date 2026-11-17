// Behaviour test for CTCCreatureOpinionOfHero::SetPersonalityOverride(long, bool) (0x00681E60): without replace, an existing override
// (current def name != the opinion def's personality) is kept; 0 resets the mask to all-true with the wife attitudes (13..17) off; a
// def that is not an OPINION_PERSONALITY (or not found) recurses with (0, true); a good def fills the mask with its default, turns the
// wife attitudes off, applies every map entry below OPINION_ATTITUDE_TYPE_LAST, and rescales each transient's run-out by new / old
// TimeScalingRunOut (only when the old one is > 0.0001), at least 1 frame, keeping the remaining delta. Model of the row's logic.
#include <cstdio>
#include <cstring>
#include <map>
#include <list>
struct Def { int name; bool is_personality; bool def_default; std::map<int, bool> enabled; float runout[4]; bool lookup_ok; };   // is_personality: the def's class type; lookup_ok: GetDef finds it
struct Transient { int Opinion; float OffsetPerFrameRunOut; long FramesToRunOut; };
struct Creature {
    Def* current; int opinion_personality_name; bool mask[19]; /* mask[18]: guard byte past the 18 attitudes */ std::list<Transient> transients; Def* defs[8]; int recursed; int lookups;
    bool GetDef(Def* d) { ++lookups; if (!d->lookup_ok) return false; current = d; return true; }
    void Set(long personality_def, bool replace)
    {
        if (!replace && current->name != opinion_personality_name) return;
        if (personality_def) {
            Def* old = current;
            Def* d = defs[personality_def];
            if (!(d && d->is_personality) || !GetDef(d)) { ++recursed; Set(0, true); return; }   // class type first, then the lookup
            for (int i = 0; i < 18; ++i) mask[i] = current->def_default;
            for (long c1 = 13; c1 < 18; ++c1) mask[c1] = false;
            bool found[19]; std::memset(found, 0, sizeof(found));
            for (std::map<int, bool>::const_iterator it = current->enabled.begin(); it != current->enabled.end(); ++it)
                if (it->first < 18) { found[it->first] = true; mask[it->first] = it->second; }
            for (std::list<Transient>::iterator it = transients.begin(); it != transients.end(); ++it) {
                Transient& t = *it;
                float relative_time_scaling = current->runout[t.Opinion];
                if (old->runout[t.Opinion] > 0.0001f) relative_time_scaling /= old->runout[t.Opinion];
                float remaining = t.FramesToRunOut * t.OffsetPerFrameRunOut;
                long frames = (long)(t.FramesToRunOut * relative_time_scaling);
                if (frames < 1) frames = 1;
                t.FramesToRunOut = frames; t.OffsetPerFrameRunOut = remaining / frames;
            }
        } else {
            for (int i = 0; i < 18; ++i) mask[i] = true;
            for (long c1 = 13; c1 < 18; ++c1) mask[c1] = false;
        }
    }
};
static int fail(int n) { std::printf("00681e60_TEST FAIL %d\n", n); return 1; }
static unsigned guard(const Creature& c) { return ((const unsigned char*)c.mask)[18]; }
static bool near(float a, float b) { return a - b < 1e-4f && b - a < 1e-4f; }
int main()
{
    Def base = { 1, true, true, std::map<int, bool>(), { 2.0f, 2.0f, 0.0001f, 2.0f }, true };
    Def calm = { 2, true, false, std::map<int, bool>(), { 4.0f, 1.0f, 3.0f, 0.1f }, true };
    calm.enabled[3] = true; calm.enabled[14] = true; calm.enabled[18] = true; calm.enabled[17] = true;
    Def wrong = { 3, false, true, std::map<int, bool>(), { 1, 1, 1, 1 }, true };          // found, but not an OPINION_PERSONALITY
    Creature c; std::memset(c.mask, 1, 18); std::memset(&c.mask[18], 0x55, 1);         // mask poisoned true; byte 18 = guard past the 18 attitudes
    c.current = &base; c.opinion_personality_name = 1; c.recursed = 0; c.lookups = 0;
    for (int i = 0; i < 8; ++i) c.defs[i] = 0;
    c.defs[2] = &calm; c.defs[3] = &wrong;
    Transient t0 = { 0, 1.0f, 10 }, t2 = { 2, 2.0f, 10 }, t3 = { 3, 1.0f, 10 };
    c.transients.push_back(t0); c.transients.push_back(t2); c.transients.push_back(t3);
    c.Set(2, false);                                                   // current == default personality: allowed
    if (c.current != &calm || guard(c) != 0x55) return fail(1);         // key 18 (== OPINION_ATTITUDE_TYPE_LAST) ignored; nothing past the mask
    for (int i = 0; i < 18; ++i) {
        bool want = (i == 3 || i == 14 || i == 17);                    // default false; map entries applied after the wife reset; 18 ignored
        if (c.mask[i] != want) return fail(2 + i);
    }
    std::list<Transient>::iterator it = c.transients.begin();
    if (it->FramesToRunOut != 20 || !near(it->OffsetPerFrameRunOut, 0.5f)) return fail(30);          // 4/2: twice as long
    ++it; if (it->FramesToRunOut != 30 || !near(it->OffsetPerFrameRunOut, 20.0f / 30.0f)) return fail(31);   // old 0.0001: not divided
    ++it; if (it->FramesToRunOut != 1 || !near(it->OffsetPerFrameRunOut, 10.0f)) return fail(32);    // 0.05 -> 0 frames -> at least 1
    c.Set(0, false);                                                   // calm is an override now (name 2 != 1): kept
    if (c.current != &calm || c.mask[0]) return fail(33);
    c.Set(0, true);                                                    // reset
    for (int i = 0; i < 18; ++i) if (c.mask[i] != (i < 13)) return fail(34 + i);
    if (guard(c) != 0x55) return fail(80);
    c.current = &base; c.Set(3, true);                                 // not a personality: recurse with (0, true)
    if (c.recursed != 1 || c.current != &base || !c.mask[0] || c.mask[13]) return fail(60);
    if (c.lookups != 1) return fail(83);                               // wrong class type: GetDef is not even asked (only calm's lookup so far)
    c.Set(5, true); if (c.recursed != 2) return fail(61);              // not found
    Def warm = { 4, true, true, std::map<int, bool>(), { 1, 1, 1, 1 }, true };   // default true: wife attitudes still forced off
    warm.enabled[2] = false; c.defs[6] = &warm; c.current = &base;
    c.Set(6, true);
    for (int i = 0; i < 18; ++i) if (c.mask[i] != (i < 13 && i != 2)) return fail(62 + i);
    if (guard(c) != 0x55) return fail(81);
    c.current = &calm; c.Set(3, true);                                 // an override in place + a bad def: the recursion must REPLACE (0, true)
    if (c.recursed != 3 || c.current != &calm || !c.mask[2] || c.mask[13] || guard(c) != 0x55) return fail(82);
    Def missing = { 5, true, false, std::map<int, bool>(), { 1, 1, 1, 1 }, false };   // the right class type, but GetDef fails
    c.defs[7] = &missing; c.current = &base; std::memset(c.mask, 0, 18); int before = c.lookups;
    c.Set(7, true);
    if (c.recursed != 4 || c.lookups != before + 1 || c.current != &base || !c.mask[0] || c.mask[13]) return fail(84);
    std::printf("00681e60_TEST PASS\n");
    return 0;
}
