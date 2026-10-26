// Behaviour test for CTCHitLocations::GetPCombatFirstHitLocationIntersectionWithTriangle (0x008169B0): only enabled
// locations whose volume intersects are collected (at most 8, with their pair value and both hit points), the list
// is sorted and handed with the triangle's vertices and the outs to the best-hit search.
#include <cstdio>

class CThing { public: int id; };
class C3DVector { public: float X, Y, Z; };
struct CTriangle { C3DVector* PVertices; };
struct CHitVolume {
    bool Hit;
    virtual bool IntersectTriangle(CThing*, const CTriangle&, C3DVector* a, C3DVector* b, long flags)
    { if (!Hit) return false; a->X = 1.0f; a->Y = 2.0f; a->Z = 3.0f; b->X = 4.0f; b->Y = 5.0f; b->Z = (float)flags; return true; }
};
class CHitLocationDef { public: CHitVolume* PVolume; };
namespace NHitLocations {
class CHitLocation { public: CThing* POwnerThing; CHitLocationDef* PDef; void* PArmourDef; long HitResponseAction; long BlockResponseAction; bool Enabled; };
}
struct SHitLocationEntry { NHitLocations::CHitLocation first; long second; };
struct THitLocationIntersectionElement { long Value; NHitLocations::CHitLocation* PHitLocation; C3DVector HitA; C3DVector HitB; };
struct CHitLocationIntersectionLess {
    bool operator()(const THitLocationIntersectionElement& a, const THitLocationIntersectionElement& b) const { return a.Value < b.Value; }
};
static unsigned long g_sorted = 99;
namespace std {
void sort(THitLocationIntersectionElement* first, THitLocationIntersectionElement* last, CHitLocationIntersectionLess comp)
{
    g_sorted = (unsigned long)(last - first);
    for (THitLocationIntersectionElement* i = first; i != last; ++i)
        for (THitLocationIntersectionElement* j = i + 1; j != last; ++j)
            if (comp(*j, *i)) { THitLocationIntersectionElement t = *i; *i = *j; *j = t; }
}
}
static unsigned long g_count = 0; static C3DVector* g_vertices = 0; static float g_lastBZ = 0.0f;
NHitLocations::CHitLocation* GetPBestHitLocationFromList(THitLocationIntersectionElement* elements, unsigned long count, C3DVector* vertices, C3DVector*, C3DVector*)
{ g_count = count; g_vertices = vertices; if (!count) return 0; g_lastBZ = elements[0].HitB.Z; return elements[0].PHitLocation; }
class CTCHitLocations { public:
    SHitLocationEntry* HitLocations_start; SHitLocationEntry* HitLocations_finish;
    NHitLocations::CHitLocation* GetPCombatFirstHitLocationIntersectionWithTriangle(const CTriangle& triangle, C3DVector* outA, C3DVector* outB, long flags);
};
NHitLocations::CHitLocation* CTCHitLocations::GetPCombatFirstHitLocationIntersectionWithTriangle(const CTriangle& triangle,
    C3DVector* outA, C3DVector* outB, long flags)
{
    THitLocationIntersectionElement hits[8];
    unsigned long count = 0;
    for (SHitLocationEntry* entry = HitLocations_start; entry != HitLocations_finish; ++entry) {
        if (entry->first.Enabled) {
            C3DVector hitA;
            C3DVector hitB;
            if (entry->first.PDef->PVolume->IntersectTriangle(entry->first.POwnerThing, triangle, &hitA, &hitB, flags) && count < 8) {
                hits[count].Value = entry->second;
                hits[count].PHitLocation = &entry->first;
                hits[count].HitA = hitA;
                hits[count].HitB = hitB;
                ++count;
            }
        }
    }
    std::sort(hits, hits + count, CHitLocationIntersectionLess());
    C3DVector* vertices = triangle.PVertices;
    return GetPBestHitLocationFromList(hits, count, vertices, outA, outB);
}
int main()
{
    CHitVolume hitting, missing; hitting.Hit = true; missing.Hit = false;
    CHitLocationDef dh, dm; dh.PVolume = &hitting; dm.PVolume = &missing;
    SHitLocationEntry e[12];
    for (int i = 0; i < 12; ++i) { e[i].first.Enabled = true; e[i].first.PDef = &dh; e[i].first.POwnerThing = 0; e[i].second = 100 - i; }
    e[0].first.Enabled = false; e[1].first.PDef = &dm;
    C3DVector verts[3]; CTriangle tri; tri.PVertices = verts; C3DVector a, b;
    CTCHitLocations h; h.HitLocations_start = e; h.HitLocations_finish = e + 12;
    NHitLocations::CHitLocation* best = h.GetPCombatFirstHitLocationIntersectionWithTriangle(tri, &a, &b, 7);
    // entries 2..11 hit; the first 8 (2..9) are kept; sorted ascending by value -> entry 9 (value 91) first
    if (g_sorted != 8 || g_count != 8 || g_vertices != verts || best != &e[9].first || g_lastBZ != 7.0f) { std::printf("008169b0_TEST FAIL\n"); return 1; }
    h.HitLocations_finish = e + 2;
    if (h.GetPCombatFirstHitLocationIntersectionWithTriangle(tri, &a, &b, 1) != 0 || g_count != 0) { std::printf("008169b0_TEST FAIL\n"); return 1; }
    std::printf("008169b0_TEST PASS\n");
    return 0;
}