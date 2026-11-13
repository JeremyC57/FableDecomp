// Behaviour test for 0x00949AB0 CAIStateGroup_GoToPub::IsRunnable (and the same-TU GetSeat it expands): a host model of the
// state group, its creature's scratchpad / village membership, the pub's tables and the seat calls, run through the scripted
// path, the not-yet-time / not-tavern-time exits, and the random-table path (seed stepped *0x24A1 + 0x24DF, rotated right 13).
#include <cstdio>
#include <cstring>
#include <list>

struct Thing;
struct TavernTable {                                   // CTCTavernTable: which seats it can give
    bool free_seat; long tankard_id;
    TavernTable() : free_seat(false), tankard_id(0) {}
};
struct Thing {                                         // CThing with the components IsRunnable reads
    bool dead; bool owned_entity; Thing* owner; TavernTable* table; bool thing_owner; std::list<Thing*> objects;
    Thing() : dead(false), owned_entity(false), owner(0), table(0), thing_owner(false) {}
};
struct Scratchpad { const char* table_name; const char* seat_name; };

static unsigned int ror13(unsigned int v) { return (v >> 13) | (v << 19); }
static unsigned int GFRandom(unsigned int range, unsigned int* seed)
{
    *seed *= 0x24a1; *seed += 0x24df; *seed = ror13(*seed);
    if (range) return *seed % range;
    return 0;
}
static bool alive(Thing* t) { return t != 0 && !t->dead; }

struct GoToPub {
    // world / creature
    long frame; Scratchpad* scratchpad; bool in_village; bool village; bool tavern_time; Thing* nearest_pub;
    Thing* scripted_table; bool staffed; bool bar; unsigned int seed;
    // state group members
    Thing* PPub; Thing* PTable; long PTankard; long SeatIndex; long ActionPointIndex; long NextFramePub; bool UnderScriptControl;
    int free_seat_calls; const char* seat_asked;
    GoToPub() : frame(100), scratchpad(0), in_village(true), village(true), tavern_time(true), nearest_pub(0), scripted_table(0),
        staffed(true), bar(true), seed(1), PPub(0), PTable(0), PTankard(0), SeatIndex(0), ActionPointIndex(0), NextFramePub(0),
        UnderScriptControl(false), free_seat_calls(0), seat_asked(0) {}
    bool TableGetSeat(Thing* table, const char* seat_name)
    {
        seat_asked = seat_name;
        if (table->table && table->table->free_seat) { SeatIndex = 3; ActionPointIndex = 7; return true; }
        return false;
    }
    bool GetSeat(Thing* table = 0, const char* seat_name = 0)
    {
        if (table && seat_name) {
            PTable = table;
            if (PTable->table && TableGetSeat(PTable, seat_name)) { PTankard = PTable->table->tankard_id; return true; }
            ActionPointIndex = -1;
            return false;
        }
        std::list<Thing*> tables;
        if (!alive(PPub) || !PPub->thing_owner) return false;
        for (std::list<Thing*>::iterator it = PPub->objects.begin(); it != PPub->objects.end(); ++it)
            if (alive(*it) && (*it)->table) tables.push_back(*it);
        if (tables.empty()) return false;
        PTable = 0;
        while (!tables.empty()) {
            long index = (long)GFRandom((unsigned int)tables.size(), &seed);
            std::list<Thing*>::iterator table_it;
            for (table_it = tables.begin(); table_it != tables.end(); ++table_it) {
                if (index == 0) { PTable = *table_it; break; }
                --index;
            }
            if (PTable->table && TableGetSeat(PTable, 0)) { PTankard = PTable->table->tankard_id; return true; }
            tables.erase(table_it);
        }
        ActionPointIndex = -1;
        return false;
    }
    bool IsRunnable()
    {
        UnderScriptControl = std::strcmp(scratchpad->seat_name, "") != 0 && std::strcmp(scratchpad->table_name, "") != 0;
        if (UnderScriptControl) {
            PTable = scripted_table;
            Thing* owned = PTable->owned_entity ? PTable : 0;
            const char* seat_name = scratchpad->seat_name;
            if (owned) {
                PPub = owned->owner;
                if (GetSeat(PTable, seat_name) && bar) return true;
            }
            return false;
        }
        if (NextFramePub < frame && in_village && village && tavern_time) {
            PPub = nearest_pub;
            if (alive(PPub)) {
                if (staffed && GetSeat() && bar) { ++free_seat_calls; return true; }
                ++free_seat_calls;
                return false;
            }
        }
        return false;
    }
};

int main()
{
    bool ok = true;
    Scratchpad named = { "TABLE_1", "SEAT_2" }, empty = { "", "" }, half = { "TABLE_1", "" };
    TavernTable full, open; open.free_seat = true; open.tankard_id = 42;
    Thing pub, t1, t2, t3; pub.thing_owner = true;
    t1.table = &full; t2.table = &open; t3.table = &open; t3.dead = true;
    pub.objects.push_back(&t1); pub.objects.push_back(&t2); pub.objects.push_back(&t3);
    // scripted: the named table owned by the pub gives the named seat
    { GoToPub g; g.scratchpad = &named; t2.owned_entity = true; t2.owner = &pub; g.scripted_table = &t2;
      ok = ok && g.IsRunnable() && g.UnderScriptControl && g.PPub == &pub && g.PTankard == 42 && std::strcmp(g.seat_asked, "SEAT_2") == 0;
      g.bar = false; ok = ok && !g.IsRunnable(); }
    // scripted table without an owner component: not runnable, nothing taken
    { GoToPub g; g.scratchpad = &named; Thing lone; lone.table = &open; g.scripted_table = &lone;
      ok = ok && !g.IsRunnable() && g.UnderScriptControl && g.PPub == 0 && g.PTankard == 0; }
    // scripted table that has no free seat: ActionPointIndex reset
    { GoToPub g; g.scratchpad = &named; t1.owned_entity = true; t1.owner = &pub; g.scripted_table = &t1;
      ok = ok && !g.IsRunnable() && g.ActionPointIndex == -1; }
    // half-set names are not scripted; too early for the pub
    { GoToPub g; g.scratchpad = &half; g.NextFramePub = 200; g.nearest_pub = &pub;
      ok = ok && !g.IsRunnable() && !g.UnderScriptControl && g.PPub == 0 && g.free_seat_calls == 0; }
    // not tavern time / no village
    { GoToPub g; g.scratchpad = &empty; g.tavern_time = false; g.nearest_pub = &pub; ok = ok && !g.IsRunnable() && g.PPub == 0; }
    { GoToPub g; g.scratchpad = &empty; g.village = false; g.nearest_pub = &pub; ok = ok && !g.IsRunnable(); }
    // dead pub: false without freeing a seat
    { GoToPub g; g.scratchpad = &empty; Thing dead_pub; dead_pub.dead = true; g.nearest_pub = &dead_pub;
      ok = ok && !g.IsRunnable() && g.free_seat_calls == 0; }
    // random table path: tries tables until the open one, takes its tankard, frees the seat again
    { GoToPub g; g.scratchpad = &empty; g.nearest_pub = &pub; g.seed = 12345;
      ok = ok && g.IsRunnable() && g.PTable == &t2 && g.PTankard == 42 && g.SeatIndex == 3 && g.free_seat_calls == 1 && g.seat_asked == 0; }
    // not staffed: false, seat freed
    { GoToPub g; g.scratchpad = &empty; g.nearest_pub = &pub; g.staffed = false; ok = ok && !g.IsRunnable() && g.free_seat_calls == 1; }
    // pub whose only live table is full: every table tried, ActionPointIndex -1
    { GoToPub g; g.scratchpad = &empty; Thing pub2; pub2.thing_owner = true; pub2.objects.push_back(&t1); g.nearest_pub = &pub2;
      ok = ok && !g.IsRunnable() && g.ActionPointIndex == -1 && g.free_seat_calls == 1; }
    // the seed step itself
    { unsigned int s = 1; GFRandom(5, &s); ok = ok && s == ror13(0x24a1U + 0x24dfU); }
    if (!ok) { std::printf("00949ab0_TEST FAIL\n"); return 1; }
    std::printf("00949ab0_TEST PASS\n");
    return 0;
}