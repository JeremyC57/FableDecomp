// Behaviour test for CAIStateGroup_HoverHornetQueen::SetupArena (0x0095B3E0): model copy with scripted stubs.
#include <cstdio>
#include <cstring>
#include <vector>

struct C3DVector { float X, Y, Z; };
struct CTCSolidToThing;
struct CThing { C3DVector pos; const char* track; CTCSolidToThing* solid; C3DVector InitialPos; const C3DVector& GetPosition() { return pos; } };
struct CTCSolidToThing { CThing* notSolid[8]; int n; void AddThingNotSolidTo(CThing* t) { notSolid[n++] = t; } };
struct C2DBoxI { long MinX, MinY, MaxX, MaxY; C2DBoxI(long a, long b, long c, long d) : MinX(a), MinY(b), MaxX(c), MaxY(d) { } };
struct CCharString { const char* s; CCharString(const char* t, long = -1) : s(t) { } };
struct CThingFilter_HasTrackName { CCharString Name; CThingFilter_HasTrackName(const CCharString& n) : Name(n) { } };
struct CThingFilter_All { CThingFilter_All() { } };
static CThing* g_world[8]; static int g_nworld;
static bool Inside(const C2DBoxI& a, CThing* t) { return (long)t->pos.X >= a.MinX && (long)t->pos.X <= a.MaxX && (long)t->pos.Y >= a.MinY && (long)t->pos.Y <= a.MaxY; }
struct CThingSearchTools {
    void GetAllThingsInRoughArea(const C2DBoxI& area, const CThingFilter_All&, std::vector<CThing*>& out) const
    { for (int i = 0; i < g_nworld; ++i) if (Inside(area, g_world[i])) out.push_back(g_world[i]); }
    bool GetPFirstThingInRoughArea(const C2DBoxI& area, const CThingFilter_HasTrackName& f, CThing** out) const
    {
        for (int i = 0; i < g_nworld; ++i)
            if (Inside(area, g_world[i]) && g_world[i]->track && !std::strcmp(g_world[i]->track, f.Name.s)) { *out = g_world[i]; return true; }
        return false;
    }
};
struct CAIBrain { CThing* PCreature; };
static int g_circles;

struct CAIStateGroup_HoverHornetQueen {
    CAIBrain* PBrain; CThingSearchTools tools; C3DVector ArenaCentrePos;
    CThing* GetCreature() const { return PBrain->PCreature; }
    CThingSearchTools* PeekThingSearchTools() { return &tools; }
    void SetUpCirclePoints() { ++g_circles; }
    void SetupArena();
};

void CAIStateGroup_HoverHornetQueen::SetupArena()
{
    SetUpCirclePoints();
    CThing* self = GetCreature();
    std::vector<CThing*> things;
    {
        const C3DVector& pos = self->GetPosition();
        long x = (long)pos.X;
        long y = (long)pos.Y;
        PeekThingSearchTools()->GetAllThingsInRoughArea(C2DBoxI(x - 60, y - 60, x + 60, y + 60), CThingFilter_All(), things);
    }
    CThing* me = GetCreature();
    if (me->solid) {
        CTCSolidToThing* solid = me->solid;
        unsigned int count = (unsigned int)things.size();
        for (unsigned int i = 0; i < count; ++i)
            solid->AddThingNotSolidTo(things[i]);
    }
    {
        CThing* centre = 0;
        {
            CCharString name("WaspQueenArenaCentre");
            CThingFilter_HasTrackName filter(name);
            const C3DVector& pos = GetCreature()->GetPosition();
            long x = (long)pos.X;
            long y = (long)pos.Y;
            PeekThingSearchTools()->GetPFirstThingInRoughArea(C2DBoxI(x - 60, y - 60, x + 60, y + 60), filter, &centre);
        }
        if (centre)
            ArenaCentrePos = centre->GetPosition();
        else
            ArenaCentrePos = GetCreature()->InitialPos;
    }
}

int main()
{
    int ok = 1;
    CTCSolidToThing solid = { { 0 }, 0 };
    CThing queen = { { 0, 0, 0 }, 0, &solid, { 7, 8, 9 } };
    CThing a = { { 30, 30, 0 }, 0, 0, { 0, 0, 0 } };
    CThing far = { { 100, 0, 0 }, 0, 0, { 0, 0, 0 } };
    CThing centre = { { -20, 10, 3 }, "WaspQueenArenaCentre", 0, { 0, 0, 0 } };
    g_world[0] = &queen; g_world[1] = &a; g_world[2] = &far; g_world[3] = &centre; g_nworld = 4;
    CAIBrain b = { &queen };
    CAIStateGroup_HoverHornetQueen s; s.PBrain = &b;
    s.SetupArena();
    if (g_circles != 1 || solid.n != 3 || solid.notSolid[1] != &a || solid.notSolid[2] != &centre) ok = 0;
    if (s.ArenaCentrePos.X != -20 || s.ArenaCentrePos.Y != 10 || s.ArenaCentrePos.Z != 3) ok = 0;
    // no centre marker: the initial position; no solid component: nothing added
    g_nworld = 3; queen.solid = 0; solid.n = 0;
    s.SetupArena();
    if (solid.n != 0 || s.ArenaCentrePos.X != 7 || s.ArenaCentrePos.Z != 9) ok = 0;
    if (!ok) { std::printf("0095b3e0_TEST FAIL\n"); return 1; }
    std::printf("0095b3e0_TEST PASS\n");
    return 0;
}