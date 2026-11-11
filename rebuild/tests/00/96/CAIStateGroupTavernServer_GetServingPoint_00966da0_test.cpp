// Behaviour test for 0x00966DA0 CAIStateGroup_TavernServer::GetServingPoint: the row's body over stub classes (std::list,
// a scripted random sequence, tables that accept or refuse to hand out a serving point).
#include <cstdio>
#include <cstdlib>
#include <list>
static void check(bool b) { if (!b) { std::puts("00966da0_TEST FAIL"); std::exit(1); } }
enum ETCInterfaceType { TCI_THING_OWNER = 0x21, TCI_TAVERN_TABLE = 0xB8 };
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up, Forward; };
class CThing;
class CTCTavernTable {
public:
    enum ServingMode { SERVING_MODE_0 = 0 };
    bool accept; int calls; CThing* tankard_out;
    bool GetServingPoint(CThing&, C3DVector& pos, CRightHandedSet&, long& ap, long& seat, CThing** tankard, ServingMode mode, bool reserve)
    {
        ++calls; check(mode == SERVING_MODE_0 && reserve);
        if (!accept) return false;
        pos.X = 7.0f; ap = 3; seat = 4; *tankard = tankard_out; return true;
    }
};
class CThing {
public:
    bool dead; bool has_table; CTCTavernTable table;
    CThing() : dead(false), has_table(false) { table.accept = false; table.calls = 0; table.tankard_out = 0; }
    bool IsDead() const { return dead; }
    bool HasTC(ETCInterfaceType t) const { return t == TCI_TAVERN_TABLE && has_table; }
    bool GetTC(CTCTavernTable** out) { if (!has_table) return false; *out = &table; return true; }
};
template<class T> class CIntelligentPointer { public: T* p; CIntelligentPointer(T* q = 0) : p(q) { } T* Get() const { return p; } T* operator->() const { return p; } };
template<class T> bool operator!=(const CIntelligentPointer<T>& a, int) { return a.Get() != 0; }
template<class T> bool IS_THING_ALIVE(const T& t) { if (t != 0) { if (!t->IsDead()) return true; } return false; }
static unsigned long g_rand[16]; static int g_randPos;
static unsigned long GFRandom(unsigned long range, unsigned long*) { unsigned long r = g_rand[g_randPos++]; return range ? r % range : 0; }
class CTCThingOwner { public: std::list<CIntelligentPointer<CThing> > objects; std::list<CIntelligentPointer<CThing> >& GetObjectList() { return objects; } };
struct CAIStateGroup_TavernServer {
    CTCThingOwner* owner; CThing creature; unsigned long seed;
    CIntelligentPointer<CThing> PTable, PTankard;
    long ServingActionPointIndex, SeatIndex; C3DVector ServingPos; CRightHandedSet ServingRHSet;
    CThing& GetCreature() { return creature; }
    unsigned long* GetPWorldSeed() { return &seed; }
    bool GetServingPoint();
};
bool CAIStateGroup_TavernServer::GetServingPoint()
{
    std::list<CThing*> tables;
    for (std::list<CIntelligentPointer<CThing> >::iterator it = owner->GetObjectList().begin(); it != owner->GetObjectList().end(); ++it)
        if (IS_THING_ALIVE(*it) && (*it)->HasTC(TCI_TAVERN_TABLE))
            tables.push_back((*it).Get());
    if (tables.empty())
        return false;
    PTable = 0;
    while (!tables.empty()) {
        long index = GFRandom(tables.size(), GetPWorldSeed());
        std::list<CThing*>::iterator table_it;
        for (table_it = tables.begin(); table_it != tables.end(); ++table_it) {
            if (index == 0) { PTable = *table_it; break; }
            --index;
        }
        CTCTavernTable* table;
        CThing* tankard;
        if (PTable->GetTC(&table)
            && table->GetServingPoint(GetCreature(), ServingPos, ServingRHSet, ServingActionPointIndex, SeatIndex, &tankard,
                                      CTCTavernTable::SERVING_MODE_0, true)) {
            PTankard = tankard;
            return true;
        }
        tables.erase(table_it);
    }
    return false;
}
int main()
{
    CThing things[5]; CThing jug;
    things[0].has_table = true;                       // live table, refuses
    things[1].has_table = true; things[1].dead = true; // dead table: skipped
    things[2].has_table = false;                      // not a table
    things[3].has_table = true; things[3].table.accept = true; things[3].table.tankard_out = &jug;
    things[4].has_table = true;                       // refuses
    CTCThingOwner owner;
    owner.objects.push_back(CIntelligentPointer<CThing>(0));
    for (int i = 0; i < 5; ++i) owner.objects.push_back(CIntelligentPointer<CThing>(&things[i]));
    CAIStateGroup_TavernServer s; s.owner = &owner; s.ServingActionPointIndex = -1; s.SeatIndex = -1;
    // candidates [0, 3, 4]: pick index 2 (things[4], refuses), then 0 (things[0], refuses), then 0 (things[3], accepts)
    g_rand[0] = 5; g_rand[1] = 4; g_rand[2] = 6; g_randPos = 0;
    check(s.GetServingPoint());
    check(g_randPos == 3 && s.PTable.Get() == &things[3] && s.PTankard.Get() == &jug);
    check(s.ServingActionPointIndex == 3 && s.SeatIndex == 4 && s.ServingPos.X == 7.0f);
    check(things[4].table.calls == 1 && things[0].table.calls == 1 && things[3].table.calls == 1 && things[1].table.calls == 0);
    // nobody accepts: every candidate tried once, false
    things[3].table.accept = false; g_randPos = 0; g_rand[0] = 0; g_rand[1] = 0; g_rand[2] = 0;
    check(!s.GetServingPoint());
    check(g_randPos == 3 && things[3].table.calls == 2 && things[0].table.calls == 2 && things[4].table.calls == 2);
    // no live tables: false without touching PTable
    CTCThingOwner empty; s.owner = &empty; s.PTable = &jug; g_randPos = 0;
    check(!s.GetServingPoint() && s.PTable.Get() == &jug && g_randPos == 0);
    std::puts("00966da0_TEST PASS");
    return 0;
}