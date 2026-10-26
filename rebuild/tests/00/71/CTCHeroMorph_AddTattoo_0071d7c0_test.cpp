#include <cstdio>
// behaviour test: AddTattoo(const CThing&) clears every tattoo for a removes-all tattoo, otherwise erases the tattoos whose body
// areas overlap the new one (area 8 needs overlapping or unset specific areas) and appends the item's def; marks the morph changed
class CTattooDef { public: bool RemovesAllTattoos; long CoversBodyAreaFlags; long SpecificCoversBodyAreaFlags; };
class CThingBaseDef { public:
    CTattooDef* tattoo; long refs;
    bool GetSubDef(CTattooDef** out) const { *out = tattoo; return tattoo != 0; }
};
struct CVec {
    CThingBaseDef* v[16]; int n;
    CThingBaseDef** begin() { return v; }
    CThingBaseDef** end() { return v + n; }
    CThingBaseDef** erase(CThingBaseDef** pos) { for (CThingBaseDef** p = pos; p + 1 < end(); ++p) *p = p[1]; --n; return pos; }
    void clear() { n = 0; }
    void push_back(CThingBaseDef* d) { v[n++] = d; ++d->refs; }
};
static long g_frame = 0;
static bool TattoosOverlap(const CTattooDef* a, const CTattooDef* b)
{
    long overlap = a->CoversBodyAreaFlags & b->CoversBodyAreaFlags;
    if (overlap == 0)
        return false;
    if (overlap == 8 && (a->SpecificCoversBodyAreaFlags & b->SpecificCoversBodyAreaFlags) == 0 &&
        a->SpecificCoversBodyAreaFlags != 0 && b->SpecificCoversBodyAreaFlags != 0)
        return false;
    return true;
}
class CThing { public: CThingBaseDef* PDef; };
class CTCHeroMorph { public:
    CVec Tattoos; long FrameTattoosChanged; bool Changed;
    void AddTattoo(const CThing& tattooThing)
    {
        CTattooDef* tattoo = 0;
        if (tattooThing.PDef->GetSubDef(&tattoo)) {
            if (tattoo->RemovesAllTattoos) {
                Tattoos.clear();
            } else {
                for (CThingBaseDef** it = Tattoos.begin(); it != Tattoos.end(); ) {
                    CTattooDef* other = 0;
                    CThingBaseDef* def = *it;
                    if (def->GetSubDef(&other) && TattoosOverlap(other, tattoo))
                        it = Tattoos.erase(it);
                    else
                        ++it;
                }
                Tattoos.push_back(tattooThing.PDef);
            }
            Changed = true;
            FrameTattoosChanged = g_frame;
        }
    }
};
int main()
{
    CTattooDef arm = { false, 1, 0 }, face8a = { false, 8, 1 }, face8b = { false, 8, 2 }, face8c = { false, 8, 0 }, all = { true, 0, 0 };
    CThingBaseDef dArm = { &arm, 1 }, dA = { &face8a, 1 }, dB = { &face8b, 1 }, dC = { &face8c, 1 }, dAll = { &all, 1 }, dNone = { 0, 1 };
    CTCHeroMorph m; m.Tattoos.n = 0; m.FrameTattoosChanged = 0; m.Changed = false;
    CThing tArm = { &dArm }, tA = { &dA }, tB = { &dB }, tC = { &dC }, tAll = { &dAll }, tNone = { &dNone };
    g_frame = 5; m.AddTattoo(tArm); m.AddTattoo(tA);
    bool ok = m.Tattoos.n == 2 && m.Changed && m.FrameTattoosChanged == 5 && dArm.refs == 2;
    m.AddTattoo(tB);                       // area 8, disjoint specific areas: both kept
    ok = ok && m.Tattoos.n == 3;
    m.AddTattoo(tC);                       // area 8 with unset specific area: replaces A and B
    ok = ok && m.Tattoos.n == 2 && m.Tattoos.v[0] == &dArm && m.Tattoos.v[1] == &dC;
    m.Changed = false; g_frame = 9; m.AddTattoo(tNone);   // no tattoo sub-def: nothing
    ok = ok && !m.Changed && m.Tattoos.n == 2;
    m.AddTattoo(tAll);                     // removes all, not appended
    ok = ok && m.Tattoos.n == 0 && m.Changed && m.FrameTattoosChanged == 9;
    if (!ok) { std::printf("0071d7c0_TEST FAIL\n"); return 1; }
    std::printf("0071d7c0_TEST PASS\n");
    return 0;
}