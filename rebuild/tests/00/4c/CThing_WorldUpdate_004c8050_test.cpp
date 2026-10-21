// Behaviour test for CThing::WorldUpdate (0x004C8050): every update-list component not pending removal is frame-updated
// while the thing lives (a component killing the thing stops the pass, false); then the global pending-removal list drops
// entries whose thing is gone or dead, and for this thing's entries clears PTCDriver for a driver, takes the component
// off the update list and the interface map (the matching component among equal keys), deletes it and drops the entry;
// other things' entries stay. Always false.
#include <cstdio>
#include <list>
#include <vector>
#include <utility>

struct Thing;
struct TC { int type; bool pending, driver; int updates; Thing* killOnUpdate; bool* deleted; };
struct Thing {
    bool dead; TC* driver; std::list<TC*> update; std::vector<std::pair<int, TC*> > map;
    bool WorldUpdate();
};
struct Removal { Thing* thing; TC* tc; };
static std::list<Removal> g_removals;

bool Thing::WorldUpdate()
{
    for (std::list<TC*>::iterator i = update.begin(); i != update.end(); ++i) {
        if (dead) return false;
        TC* tc = *i;
        if (!tc->pending) { ++tc->updates; if (tc->killOnUpdate) tc->killOnUpdate->dead = true; }
    }
    if (dead) return false;
    std::list<Removal>::iterator it = g_removals.begin();
    while (it != g_removals.end()) {
        if (!(it->thing && !it->thing->dead)) it = g_removals.erase(it);
        else if (it->thing == this) {
            TC* tc = it->tc;
            if (tc) {
                if (tc->driver) driver = 0;
                update.remove(tc);
                std::vector<std::pair<int, TC*> >::iterator e = map.begin();
                while (e != map.end() && e->first < tc->type) ++e;
                while (e != map.end() && e->second != tc && e->first == tc->type) ++e;
                map.erase(e);
                *tc->deleted = true;
                it->tc = 0;
            }
            it = g_removals.erase(it);
        } else ++it;
    }
    return false;
}

int main()
{
    int ok = 1;
    bool d1 = false, d2 = false, d3 = false;
    TC a = { 1, false, true, 0, 0, &d1 }, b = { 2, true, false, 0, 0, &d2 }, c = { 2, false, false, 0, 0, &d3 };
    Thing t; t.dead = false; t.driver = &a;
    t.update.push_back(&a); t.update.push_back(&b); t.update.push_back(&c);
    t.map.push_back(std::make_pair(1, &a)); t.map.push_back(std::make_pair(2, &b)); t.map.push_back(std::make_pair(2, &c));
    Thing other; other.dead = false; other.driver = 0;
    Thing gone; gone.dead = true; gone.driver = 0;
    Removal r1 = { &t, &a }, r2 = { &other, 0 }, r3 = { &gone, 0 }, r4 = { 0, 0 }, r5 = { &t, &c };
    g_removals.push_back(r1); g_removals.push_back(r2); g_removals.push_back(r3); g_removals.push_back(r4);
    g_removals.push_back(r5);
    ok &= !t.WorldUpdate();
    ok &= a.updates == 1 && b.updates == 0 && c.updates == 1;
    ok &= d1 && d3 && !d2 && t.driver == 0 && t.update.size() == 1 && t.update.front() == &b;
    ok &= t.map.size() == 1 && t.map[0].second == &b && g_removals.size() == 1 && g_removals.front().thing == &other;
    TC k = { 3, false, false, 0, 0, &d2 };
    Thing u; u.dead = false; u.driver = 0; k.killOnUpdate = &u;
    u.update.push_back(&k); u.update.push_back(&k);
    ok &= !u.WorldUpdate() && k.updates == 1 && g_removals.size() == 1;
    std::printf(ok ? "004c8050_TEST PASS\n" : "004c8050_TEST FAIL\n");
    return ok ? 0 : 1;
}