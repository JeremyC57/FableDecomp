#include <cstdio>
#include <list>
// Behaviour model of CAIStateGroup_EatAtTable::GetSeat 0x0096CB40: building alive + owner check, village duration, first
// alive, non-limbo table that grants an eating point or else a seat wins.
struct Table { bool eatingPoint, seat; int seatIndex; };
struct Thing { bool dead, limbo; Table* table; };
struct Village { long eatDuration; };
struct Group {
    Thing* building; bool buildingOwns; std::list<Thing*> owned; Village* village;
    Thing* chosen; long seatIndex; float duration; float rnd;
    bool GetSeat()
    {
        if (!(building && !building->dead)) return false;
        if (!buildingOwns) return false;
        if (village) { duration = (float)village->eatDuration; duration += rnd; }
        for (std::list<Thing*>::iterator it = owned.begin(); it != owned.end(); ++it) {
            Thing* t = *it;
            if (t && !t->dead && !t->limbo && t->table) {
                if (t->table->eatingPoint) { seatIndex = t->table->seatIndex; chosen = t; return true; }
                if (t->table->seat) { seatIndex = t->table->seatIndex + 100; chosen = t; return true; }
            }
        }
        return false;
    }
};
int main()
{
    Table full = { false, false, 1 }, seatOnly = { false, true, 2 }, eat = { true, true, 3 };
    Thing b = { false, false, 0 }, deadT = { true, false, &eat }, limboT = { false, true, &eat }, t1 = { false, false, &full },
          t2 = { false, false, &seatOnly }, t3 = { false, false, &eat };
    Village v = { 30 };
    Group g; g.building = &b; g.buildingOwns = true; g.village = &v; g.chosen = 0; g.seatIndex = -1; g.duration = 0; g.rnd = 5.0f;
    g.owned.push_back(0); g.owned.push_back(&deadT); g.owned.push_back(&limboT); g.owned.push_back(&t1); g.owned.push_back(&t2);
    g.owned.push_back(&t3);
    bool ok = g.GetSeat() && g.chosen == &t2 && g.seatIndex == 102 && g.duration == 35.0f;
    g.owned.clear(); g.owned.push_back(&t3); g.chosen = 0;
    ok = ok && g.GetSeat() && g.chosen == &t3 && g.seatIndex == 3;
    g.owned.clear(); g.owned.push_back(&t1); ok = ok && !g.GetSeat();
    b.dead = true; ok = ok && !g.GetSeat();
    b.dead = false; g.buildingOwns = false; ok = ok && !g.GetSeat();
    g.building = 0; ok = ok && !g.GetSeat();
    if (!ok) { std::printf("0096cb40_TEST FAIL\n"); return 1; }
    std::printf("0096cb40_TEST PASS\n");
    return 0;
}