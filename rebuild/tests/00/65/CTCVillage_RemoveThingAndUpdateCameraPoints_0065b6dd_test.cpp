// Behaviour test for CTCVillage::RemoveThingAndUpdateCameraPoints / RemoveVillageMember (0x0065B6DD): the thing is
// always removed from the tasks, then erased from only the first list (people, torches, buildings, crops, patrols,
// markers) that holds it; later lists keep their copy; a thing in no list leaves every list unchanged.
#include <cstdio>
#include <list>
#include <algorithm>

struct CThing { int id; };

struct CTCVillage {
    std::list<const CThing*> VillageBuildings, VillagePeople, VillageTorches, VillageCrops, VillagePatrols, VillageMarkers;
    int tasksRemoved;
    CTCVillage() : tasksRemoved(0) {}
    void RemoveThingFromAllTasks(const CThing&) { ++tasksRemoved; }
    void RemoveVillageMember(const CThing& thing);
};

template <class L> static bool EraseFirst(L& l, const CThing* key)
{
    typename L::iterator it = std::find(l.begin(), l.end(), key);
    if (it != l.end()) {
        l.erase(it);
        return true;
    }
    return false;
}

void CTCVillage::RemoveVillageMember(const CThing& thing)
{
    RemoveThingFromAllTasks(thing);
    if (EraseFirst(VillagePeople, &thing)) return;
    if (EraseFirst(VillageTorches, &thing)) return;
    if (EraseFirst(VillageBuildings, &thing)) return;
    if (EraseFirst(VillageCrops, &thing)) return;
    if (EraseFirst(VillagePatrols, &thing)) return;
    EraseFirst(VillageMarkers, &thing);
}

static int Count(const std::list<const CThing*>& l, const CThing* t) { return (int)std::count(l.begin(), l.end(), t); }

int main()
{
    int ok = 1;
    CThing a = { 1 }, b = { 2 }, c = { 3 }, d = { 4 };
    CTCVillage v;
    v.VillagePeople.push_back(&a); v.VillagePeople.push_back(&b);
    v.VillageTorches.push_back(&a); v.VillageTorches.push_back(&c);
    v.VillageCrops.push_back(&c);
    v.VillageMarkers.push_back(&d);

    v.RemoveVillageMember(a);                    // people hit: torches keep a
    ok &= Count(v.VillagePeople, &a) == 0 && Count(v.VillagePeople, &b) == 1 && Count(v.VillageTorches, &a) == 1;
    v.RemoveVillageMember(c);                    // torches hit: crops keep c
    ok &= Count(v.VillageTorches, &c) == 0 && Count(v.VillageCrops, &c) == 1;
    v.RemoveVillageMember(c);                    // now crops
    ok &= Count(v.VillageCrops, &c) == 0;
    v.RemoveVillageMember(d);                    // last list
    ok &= v.VillageMarkers.empty();
    CThing e = { 5 };
    v.RemoveVillageMember(e);                    // nowhere
    ok &= v.VillagePeople.size() == 1 && v.VillageTorches.size() == 1;
    ok &= v.tasksRemoved == 5;
    std::printf(ok ? "0065b6dd_TEST PASS\n" : "0065b6dd_TEST FAIL\n");
    return ok ? 0 : 1;
}