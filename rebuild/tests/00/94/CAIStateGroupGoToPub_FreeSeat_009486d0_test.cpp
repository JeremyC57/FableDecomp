// Behaviour test for CAIStateGroup_GoToPub::FreeSeat (0x009486D0): a self-contained copy of the model (things with a component
// table, intelligent pointers, the table / carrying / tankard components as recorders) run through the function's logic.
#include <cstdio>

enum { TCI_CARRYING = 0x46, TCI_TAVERN_TABLE = 0xB8, TCI_TAVERN_TANKARD = 0xBA };

class CThing { public:
    bool Dead;
    void* Components[0x100];
    CThing() : Dead(false) { for (int i = 0; i < 0x100; ++i) Components[i] = 0; }
    bool IsDead() const { return Dead; }
    template<class T> bool GetTC(int id, T** out) { if (Components[id]) { *out = (T*)Components[id]; return true; } return false; }
};

struct CIntelligentPointer {
    CThing* Item;
    CIntelligentPointer() : Item(0) { }
    CIntelligentPointer(CThing* item) : Item(item) { }
    CThing* GetPItem() const { return Item; }
    CThing* Get() const { return Item; }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { Item = other.GetPItem(); return *this; }
    CThing* operator->() const { return Get(); }
};
inline bool operator!=(const CIntelligentPointer& pointer, int) { return pointer.Get() != 0; }

// Same spelling as engine/ThingAlive.h (retail IS_THING_ALIVE<T>, FableWin 0x01CCA150).
template<class T> inline bool IS_THING_ALIVE(const T& thing)
{
    if (thing != 0) {
        if (!thing->IsDead()) return true;
    }
    return false;
}

class CTCTavernTable { public:
    int calls; long seat; long point;
    CTCTavernTable() : calls(0), seat(0), point(0) { }
    void FreeSeat(long seat_index, long action_point_index) { ++calls; seat = seat_index; point = action_point_index; }
};
class CTCCarrying { public:
    CThing* primary; int removed;
    CTCCarrying() : primary(0), removed(0) { }
    CThing* GetThingInPrimarySlot() const { return primary; }
    CThing* RemoveThingFromPrimarySlot(bool) { CThing* t = primary; primary = 0; ++removed; return t; }
};
class CTCTavernTankard { public:
    int replaced;
    CTCTavernTankard() : replaced(0) { }
    void ReplaceTankard() { ++replaced; }
};

struct CAIStateGroup_GoToPub {
    CThing* Creature;
    CIntelligentPointer PTable;
    CIntelligentPointer PTankard;
    long SeatIndex;
    long ActionPointIndex;
    CThing& GetCreature() const { return *Creature; }
    void FreeSeat()
    {
        if (PTable.Get() && ActionPointIndex != -1) {
            CTCTavernTable* table;
            PTable.Get()->GetTC(TCI_TAVERN_TABLE, &table);
            table->FreeSeat(SeatIndex, ActionPointIndex);
            CTCCarrying* carrying;
            GetCreature().GetTC(TCI_CARRYING, &carrying);
            if (IS_THING_ALIVE(PTankard)) {
                CThing* held = carrying->GetThingInPrimarySlot();
                if (held == PTankard.Get())
                    carrying->RemoveThingFromPrimarySlot(false);
                CTCTavernTankard* tankard;
                PTankard.Get()->GetTC(TCI_TAVERN_TANKARD, &tankard);
                tankard->ReplaceTankard();
            }
            ActionPointIndex = -1;
            PTankard = 0;
            PTable = 0;
            SeatIndex = -1;
        }
    }
};

struct World {
    CThing creature, tableThing, tankardThing, other;
    CTCTavernTable table; CTCCarrying carrying; CTCTavernTankard tankard;
    CAIStateGroup_GoToPub group;
    World()
    {
        creature.Components[TCI_CARRYING] = &carrying;
        tableThing.Components[TCI_TAVERN_TABLE] = &table;
        tankardThing.Components[TCI_TAVERN_TANKARD] = &tankard;
        group.Creature = &creature;
        group.PTable = &tableThing;
        group.PTankard = &tankardThing;
        group.SeatIndex = 3;
        group.ActionPointIndex = 7;
    }
    bool Cleared() const
    {
        return group.PTable.Get() == 0 && group.PTankard.Get() == 0 && group.SeatIndex == -1 && group.ActionPointIndex == -1;
    }
};

int main()
{
    bool ok = true;
    {   // no table: nothing happens, nothing is cleared
        World w; w.group.PTable = 0; w.carrying.primary = &w.tankardThing;
        w.group.FreeSeat();
        if (w.table.calls || w.carrying.removed || w.tankard.replaced) ok = false;
        if (w.group.PTankard.Get() != &w.tankardThing || w.group.SeatIndex != 3 || w.group.ActionPointIndex != 7) ok = false;
    }
    {   // no action point: nothing happens
        World w; w.group.ActionPointIndex = -1; w.carrying.primary = &w.tankardThing;
        w.group.FreeSeat();
        if (w.table.calls || w.carrying.removed || w.tankard.replaced) ok = false;
        if (w.group.PTable.Get() != &w.tableThing || w.group.PTankard.Get() != &w.tankardThing || w.group.SeatIndex != 3) ok = false;
    }
    {   // seated, holding the live tankard: seat freed with (SeatIndex, ActionPointIndex), tankard taken out of hand and replaced
        World w; w.carrying.primary = &w.tankardThing;
        w.group.FreeSeat();
        if (w.table.calls != 1 || w.table.seat != 3 || w.table.point != 7) ok = false;
        if (w.carrying.removed != 1 || w.carrying.primary != 0 || w.tankard.replaced != 1) ok = false;
        if (!w.Cleared()) ok = false;
    }
    {   // seated, live tankard on the table (holding something else): no removal, tankard still replaced
        World w; w.carrying.primary = &w.other;
        w.group.FreeSeat();
        if (w.table.calls != 1 || w.carrying.removed != 0 || w.carrying.primary != &w.other || w.tankard.replaced != 1) ok = false;
        if (!w.Cleared()) ok = false;
    }
    {   // dead tankard: only the seat is freed
        World w; w.tankardThing.Dead = true; w.carrying.primary = &w.tankardThing;
        w.group.FreeSeat();
        if (w.table.calls != 1 || w.carrying.removed != 0 || w.tankard.replaced != 0) ok = false;
        if (!w.Cleared()) ok = false;
    }
    {   // no tankard at all: only the seat is freed
        World w; w.group.PTankard = 0;
        w.group.FreeSeat();
        if (w.table.calls != 1 || w.table.seat != 3 || w.table.point != 7 || w.carrying.removed != 0 || w.tankard.replaced != 0) ok = false;
        if (!w.Cleared()) ok = false;
    }
    if (!ok) { std::printf("009486d0_TEST FAIL\n"); return 1; }
    std::printf("009486d0_TEST PASS\n");
    return 0;
}