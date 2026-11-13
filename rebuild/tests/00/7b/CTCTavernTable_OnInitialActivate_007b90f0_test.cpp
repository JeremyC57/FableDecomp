// Behaviour test for 0x007B90F0 CTCTavernTable::OnInitialActivate: a host copy of the row's logic over plain models (the named
// action points, the per-seat vectors, the tankard factory and its components), checked with and without the def's AddTankards.
#include <cstdio>
#include <cstring>
#include <cstdlib>

struct Vec { float X, Y, Z; };
struct RHSet { Vec Up, Forward; };

struct ActionPoint { char Name[32]; Vec Pos; RHSet Set; bool Used; };
struct ActionPoints {                        // CTCNamedUserActionPoints stand-in: hands out the free points with a name prefix
    ActionPoint Points[8]; int Count;
    bool GetFreeNamedActionPoint(Vec& pos, RHSet& set, const char* prefix, long& index)
    {
        for (int i = 0; i < Count; ++i)
            if (!Points[i].Used && std::strncmp(Points[i].Name, prefix, std::strlen(prefix)) == 0) {
                Points[i].Used = true; pos = Points[i].Pos; set = Points[i].Set; index = i; return true;
            }
        return false;
    }
};

struct Tankard {                             // CThingObject + its components
    Vec Pos; RHSet Set; RHSet PhysicsSet; bool CreatedByOwner; const void* Creator; const void* Owner; bool Alive;
};

struct Table {
    const void* Parent;
    bool BeingServed; long NumPeopleSeated;
    long TankardStrlen;
    // per-seat vectors (the four members), with an explicit size
    Tankard* Tankards[16]; Tankard* Food[16]; bool Seats[16]; void* Occupants[16]; unsigned long Size;
};

static Tankard g_pool[16]; static int g_created;
static Tankard* CreateTankard(const Vec& pos) { Tankard* t = &g_pool[g_created++]; std::memset(t, 0, sizeof(*t)); t->Pos = pos; t->Alive = true; return t; }

static void Resize(Table& t, unsigned long n)
{
    for (unsigned long i = t.Size; i < n; ++i) { t.Tankards[i] = 0; t.Food[i] = 0; t.Seats[i] = false; t.Occupants[i] = 0; }
    t.Size = n;
}

// The row's logic (same tests and order as 0x007B90F0).
static void OnInitialActivate(Table& t, ActionPoints* points, bool add_tankards)
{
    Vec pos; RHSet set; long index;
    while (points->GetFreeNamedActionPoint(pos, set, "DUMMY_POINT_TANKARD_", index)) {
        const char* name = points->Points[index].Name;
        long seat = std::atol(name + t.TankardStrlen) - 1;
        if (t.Size < (unsigned long)(seat + 1))
            Resize(t, seat + 1);
        if (add_tankards) {
            Tankard* tankard = CreateTankard(pos);
            tankard->Set = set; tankard->Pos = pos;           // the tankard component
            t.Tankards[seat] = tankard;
            tankard->PhysicsSet = set;
            tankard->CreatedByOwner = true;
            tankard->Creator = t.Parent;
            tankard->Owner = t.Parent;
        } else {
            t.Tankards[seat] = 0;
        }
        t.Food[seat] = 0;
        t.Seats[seat] = false;
        t.Occupants[seat] = 0;
    }
    t.NumPeopleSeated = 0;
    t.BeingServed = false;
}

static void SetPoint(ActionPoint& p, const char* name, float x)
{
    std::memset(&p, 0, sizeof(p)); std::strcpy(p.Name, name); p.Pos.X = x; p.Set.Up.Z = 1.0f; p.Set.Forward.Y = x;
}

int main()
{
    int fails = 0;
    ActionPoints points; Table t; int parent = 0;
    std::memset(&points, 0, sizeof(points)); std::memset(&t, 0, sizeof(t));
    SetPoint(points.Points[0], "DUMMY_POINT_TANKARD_3", 3.0f);
    SetPoint(points.Points[1], "ACTION_POINT_SIT_1", 9.0f);
    SetPoint(points.Points[2], "DUMMY_POINT_TANKARD_1", 1.0f);
    points.Count = 3;
    t.Parent = &parent; t.TankardStrlen = (long)std::strlen("DUMMY_POINT_TANKARD_"); t.BeingServed = true; t.NumPeopleSeated = 4;
    t.Size = 1; t.Seats[0] = true; t.Occupants[0] = &parent;
    OnInitialActivate(t, &points, true);
    if (t.Size != 3 || g_created != 2) ++fails;
    if (!t.Tankards[2] || t.Tankards[2]->Pos.X != 3.0f || t.Tankards[2]->PhysicsSet.Forward.Y != 3.0f || !t.Tankards[2]->CreatedByOwner
        || t.Tankards[2]->Owner != &parent || t.Tankards[2]->Creator != &parent) ++fails;
    if (!t.Tankards[0] || t.Tankards[0]->Pos.X != 1.0f || t.Seats[0] || t.Occupants[0]) ++fails;
    if (t.Tankards[1] || t.NumPeopleSeated != 0 || t.BeingServed) ++fails;
    if (!points.Points[0].Used || points.Points[1].Used || !points.Points[2].Used) ++fails;
    // without AddTankards: the seats are still grown and cleared, no tankard made
    std::memset(&t, 0, sizeof(t)); t.Parent = &parent; t.TankardStrlen = (long)std::strlen("DUMMY_POINT_TANKARD_");
    points.Points[0].Used = points.Points[2].Used = false;
    g_created = 0;
    OnInitialActivate(t, &points, false);
    if (t.Size != 3 || g_created != 0 || t.Tankards[0] || t.Tankards[2]) ++fails;
    if (fails) { std::printf("007b90f0_TEST FAIL\n"); return 1; }
    std::printf("007b90f0_TEST PASS\n");
    return 0;
}