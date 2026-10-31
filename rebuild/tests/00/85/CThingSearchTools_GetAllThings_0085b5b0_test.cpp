// CThingSearchTools::GetAllThings<CThingGameObject, list, CFilter_Bind4<...> >() const -- behaviour test: walks the class list and
// appends, in order, the scan-visible game objects that pass all four filters (not the striker, strike-to-strike, overlapping
// sphere, can collide); the four filters short-circuit left to right.
#include <stdio.h>
#include <list>

struct Thing { bool InScans; unsigned long TypeFlags; int Id; bool Strike, Overlap, Collide; };
static int g_calls;
struct IsNot { const Thing* Not; bool operator()(const Thing& t) const { ++g_calls; return &t != Not; } };
struct StrikeToStrike { bool operator()(const Thing& t) const { ++g_calls; return t.Strike; } };
struct Overlapping { bool operator()(const Thing& t) const { ++g_calls; return t.Overlap; } };
struct CanCollide { bool operator()(const Thing& t) const { ++g_calls; return t.Collide; } };
struct Bind4 {
    IsNot F1; StrikeToStrike F2; Overlapping F3; CanCollide F4;
    bool operator()(const Thing& t) const { return F1(t) && F2(t) && F3(t) && F4(t); }
};
static const unsigned long GameObjectMask = 0x6;
static void GetAllThings(const std::list<Thing*>& things, const Bind4& filter, std::list<Thing*>& result)
{
    for (std::list<Thing*>::const_iterator it = things.begin(); it != things.end(); ++it) {
        Thing* thing = *it;
        if (thing->InScans && (thing->TypeFlags & GameObjectMask) == GameObjectMask && filter(*thing))
            result.push_back(thing);
    }
}

int main()
{
    Thing t[6] = {
        { true, 0x7, 0, true, true, true },    // the striker: rejected by IsNot
        { true, 0x6, 1, true, true, true },    // passes
        { false, 0x6, 2, true, true, true },   // not in scans
        { true, 0x2, 3, true, true, true },    // not a game object
        { true, 0xe, 4, true, false, true },   // sphere does not overlap
        { true, 0x6, 5, true, true, true },    // passes
    };
    std::list<Thing*> all;
    for (int i = 0; i < 6; ++i) all.push_back(&t[i]);
    Bind4 f; f.F1.Not = &t[0];
    std::list<Thing*> out;
    g_calls = 0;
    GetAllThings(all, f, out);
    bool ok = out.size() == 2 && out.front()->Id == 1 && out.back()->Id == 5 && g_calls == 1 + 4 + 3 + 4;
    printf(ok ? "0085b5b0_TEST PASS\n" : "0085b5b0_TEST FAIL\n");
    return ok ? 0 : 1;
}