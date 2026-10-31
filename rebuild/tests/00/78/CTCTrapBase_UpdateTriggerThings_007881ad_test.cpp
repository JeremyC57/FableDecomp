// Behaviour test for 0x007881AD CTCTrapBase::UpdateTriggerThings: clear the trigger list, then collect the creatures able
// to be engaged in combat inside the box (centre = truncated owner X/Y, half-size = truncated def TriggerRadius).
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class C2DCoordI { public: long X, Y; C2DCoordI(long x, long y) : X(x), Y(y) { } };
class C2DBoxI { public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(const C2DCoordI& c, long r) : TLX(c.X - r), TLY(c.Y - r), BRX(c.X + r), BRY(c.Y + r) { }
};
struct Thing { C3DVector Pos; bool Creature; bool Engageable; };
static Thing g_world[4];
struct ThingList {
    const Thing* items[8]; int count;
    void clear() { count = 0; }
};
class CTrapDef { public: float TriggerRadius; };
struct Trap {
    Thing* ParentThing;
    ThingList TriggerThings;
    const CTrapDef* PTrapDef;
    void GetAllThingsInRoughArea(const C2DBoxI& area, ThingList& out) const
    {
        for (int i = 0; i < 4; ++i) {
            const Thing& t = g_world[i];
            long x = (long)t.Pos.X, y = (long)t.Pos.Y;
            if (t.Creature && t.Engageable && x >= area.TLX && x <= area.BRX && y >= area.TLY && y <= area.BRY)
                out.items[out.count++] = &t;
        }
    }
    void UpdateTriggerThings()
    {
        TriggerThings.clear();
        const CTrapDef* def = PTrapDef;
        const C3DVector& pos = ParentThing->Pos;
        GetAllThingsInRoughArea(C2DBoxI(C2DCoordI((long)pos.X, (long)pos.Y), (long)def->TriggerRadius), TriggerThings);
    }
};
int main()
{
    Thing owner = { { 10.7f, 20.2f, 0.0f }, false, false };
    g_world[0].Pos.X = 12.0f; g_world[0].Pos.Y = 21.0f; g_world[0].Creature = true;  g_world[0].Engageable = true;
    g_world[1].Pos.X = 12.0f; g_world[1].Pos.Y = 21.0f; g_world[1].Creature = true;  g_world[1].Engageable = false;
    g_world[2].Pos.X = 30.0f; g_world[2].Pos.Y = 21.0f; g_world[2].Creature = true;  g_world[2].Engageable = true;
    g_world[3].Pos.X = 7.0f;  g_world[3].Pos.Y = 17.0f; g_world[3].Creature = true;  g_world[3].Engageable = true;
    CTrapDef def = { 3.9f };                     // truncates to 3: box (7,17)-(13,23)
    Trap trap; trap.ParentThing = &owner; trap.PTrapDef = &def;
    trap.TriggerThings.count = 5;                // stale entries are cleared first
    trap.UpdateTriggerThings();
    if (trap.TriggerThings.count != 2 || trap.TriggerThings.items[0] != &g_world[0] || trap.TriggerThings.items[1] != &g_world[3]) {
        std::printf("007881ad_TEST FAIL\n"); return 1;
    }
    std::printf("007881ad_TEST PASS\n");
    return 0;
}