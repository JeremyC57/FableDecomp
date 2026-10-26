// Behaviour test for CTCHitLocations::GetPCombatFirstHitLocationIntersectionWithFiniteLine (retail 0x008168B0): model copy + stubs.
#include <cstdio>

class CThing { public: int id; };
class C3DVector { public: float X, Y, Z; };
static const C3DVector* g_lastOrigin = 0;
static long g_lastFlags = 0;
struct CHitVolume {
    bool hits; float at;
    bool IntersectFiniteLine(CThing*, const C3DVector& start, const C3DVector&, C3DVector* hitA, C3DVector* hitB, long flags)
    {
        g_lastFlags = flags;
        if (!hits) return false;
        hitA->X = at; hitA->Y = start.Y; hitA->Z = 0; hitB->X = at + 1; hitB->Y = 0; hitB->Z = 0;
        return true;
    }
};
class CHitLocationDef { public: CHitVolume* PVolume; };
namespace NHitLocations {
class CHitLocation { public: CThing* POwnerThing; CHitLocationDef* PDef; bool Enabled; };
}
struct SHitLocationEntry { NHitLocations::CHitLocation first; long second; };
struct THitLocationIntersectionElement { long Value; NHitLocations::CHitLocation* PHitLocation; C3DVector HitA; C3DVector HitB; };
struct CHitLocationIntersectionLess {
    bool operator()(const THitLocationIntersectionElement& a, const THitLocationIntersectionElement& b) const { return a.Value < b.Value; }
};
namespace mystd {
void sort(THitLocationIntersectionElement* first, THitLocationIntersectionElement* last, CHitLocationIntersectionLess comp)
{
    for (THitLocationIntersectionElement* i = first; i != last; ++i)
        for (THitLocationIntersectionElement* j = i + 1; j != last; ++j)
            if (comp(*j, *i)) { THitLocationIntersectionElement t = *i; *i = *j; *j = t; }
}
}
static unsigned long g_lastCount = 0;
NHitLocations::CHitLocation* GetPBestHitLocationFromList(THitLocationIntersectionElement* elements,
    unsigned long count, const C3DVector* origin, C3DVector* outA, C3DVector* outB)
{
    g_lastCount = count; g_lastOrigin = origin;
    if (count == 0) return 0;
    *outA = elements[0].HitA; *outB = elements[0].HitB;
    return elements[0].PHitLocation;
}
class CTCHitLocations { public:
    SHitLocationEntry* HitLocations_start;
    SHitLocationEntry* HitLocations_finish;
    NHitLocations::CHitLocation* GetPCombatFirstHitLocationIntersectionWithFiniteLine(const C3DVector& start,
        const C3DVector& end, C3DVector* outA, C3DVector* outB, long flags);
};
NHitLocations::CHitLocation* CTCHitLocations::GetPCombatFirstHitLocationIntersectionWithFiniteLine(const C3DVector& start,
    const C3DVector& end, C3DVector* outA, C3DVector* outB, long flags)
{
    THitLocationIntersectionElement hits[8];
    unsigned long count = 0;
    for (SHitLocationEntry* entry = HitLocations_start; entry != HitLocations_finish; ++entry) {
        if (entry->first.Enabled) {
            C3DVector hitA;
            C3DVector hitB;
            if (entry->first.PDef->PVolume->IntersectFiniteLine(entry->first.POwnerThing, start, end, &hitA, &hitB, flags) && count < 8) {
                hits[count].Value = entry->second;
                hits[count].PHitLocation = &entry->first;
                hits[count].HitA = hitA;
                hits[count].HitB = hitB;
                ++count;
            }
        }
    }
    mystd::sort(hits, hits + count, CHitLocationIntersectionLess());
    return GetPBestHitLocationFromList(hits, count, &start, outA, outB);
}
int main()
{
    int fail = 0;
    CThing owner = { 1 };
    CHitVolume vHit = { true, 5.0f }, vMiss = { false, 0.0f }, vHit2 = { true, 9.0f };
    CHitLocationDef dHit = { &vHit }, dMiss = { &vMiss }, dHit2 = { &vHit2 };
    SHitLocationEntry e[12];
    for (int i = 0; i < 12; ++i) { e[i].first.POwnerThing = &owner; e[i].first.PDef = &dHit; e[i].first.Enabled = true; e[i].second = 20 - i; }
    e[1].first.PDef = &dMiss; e[2].first.Enabled = false; e[3].first.PDef = &dHit2; e[3].second = 1;
    CTCHitLocations locs = { e, e + 4 };
    C3DVector start = { 0, 2, 0 }, end = { 10, 2, 0 }, outA, outB;
    NHitLocations::CHitLocation* best = locs.GetPCombatFirstHitLocationIntersectionWithFiniteLine(start, end, &outA, &outB, 7);
    if (best != &e[3].first || g_lastCount != 2 || outA.X != 9.0f || outB.X != 10.0f || g_lastOrigin != &start || g_lastFlags != 7) fail = 1;
    // more than eight hits: only the first eight are collected
    for (int i = 0; i < 12; ++i) { e[i].first.PDef = &dHit; e[i].first.Enabled = true; e[i].second = 20 - i; }
    locs.HitLocations_finish = e + 12;
    best = locs.GetPCombatFirstHitLocationIntersectionWithFiniteLine(start, end, &outA, &outB, 0);
    if (g_lastCount != 8 || best != &e[7].first) fail = 2;
    locs.HitLocations_finish = e;
    if (locs.GetPCombatFirstHitLocationIntersectionWithFiniteLine(start, end, &outA, &outB, 0) != 0 || g_lastCount != 0) fail = 3;
    if (fail) { std::printf("008168b0_TEST FAIL\n"); return 1; }
    std::printf("008168b0_TEST PASS\n");
    return 0;
}