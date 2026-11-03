// Behaviour test for CTCVillage::AddThingToVillage (0x0065B854): a model of the type switch and the per-type lists.
#include <cstdio>
#include <vector>
enum { T_CREATURE = 1, T_BUILDING = 3, T_OBJECT = 5, T_TRACK_NODE = 10, T_MARKER = 11 };
struct Thing { int type; unsigned long props; int creatureProperty; bool dead; bool isStart; int track; bool dummy; bool tavern;
               bool laggy; bool scripted; bool inLimbo; bool persist; bool hasLight; bool lightOn; std::vector<Thing*> owned; };
struct Village {
    bool day; int hour, lightsOn, lightsOff; int tasks[2]; int nTasks; int patrolTrack; int numGuards;
    std::vector<Thing*> people, buildings, torches, crops, horns, gates, cages, campfires, patrols, markers, dummies;
    int cropTasks, patrolTasks;
    bool HasTask(int t) const { for (int i = 0; i < nTasks; ++i) if (tasks[i] == t) return true; return false; }
    static bool Bit(unsigned long p, int n) { return ((p >> n) & 1) != 0; }
    void Add(Thing& t) {
        switch (t.type) {
        case T_CREATURE:
            people.push_back(&t); if (t.creatureProperty == 3) ++numGuards;
            if (t.laggy && !t.scripted) { t.inLimbo = true; t.persist = true; }
            break;
        case T_BUILDING:
            buildings.push_back(&t);
            for (size_t i = 0; i < t.owned.size(); ++i) {
                Thing* o = t.owned[i];
                if (o->dead || o->type != T_OBJECT || !o->hasLight) continue;
                if (Bit(o->props, 8) || Bit(o->props, 9)) o->lightOn = t.tavern ? !day : (hour >= lightsOn || hour < lightsOff);
            }
            break;
        case T_OBJECT:
            if (Bit(t.props, 9) || Bit(t.props, 8)) { torches.push_back(&t); if (!t.dead && t.hasLight) t.lightOn = !day; break; }
            if (Bit(t.props, 1)) { crops.push_back(&t); if (HasTask(2)) ++cropTasks; break; }
            if (Bit(t.props, 24)) { horns.push_back(&t); break; }
            if (Bit(t.props, 28)) { gates.push_back(&t); break; }
            if (Bit(t.props, 3)) { cages.push_back(&t); break; }
            if (Bit(t.props, 14)) { campfires.push_back(&t); break; }
            break;
        case T_TRACK_NODE:
            if (t.isStart && t.track == patrolTrack && HasTask(1)) { patrols.push_back(&t); ++patrolTasks; }
            break;
        case T_MARKER: markers.push_back(&t); break;
        default: if (t.dummy) dummies.push_back(&t); break;
        }
    }
};
int main()
{
    Village v = {}; v.day = false; v.hour = 22; v.lightsOn = 19; v.lightsOff = 6; v.tasks[0] = 1; v.tasks[1] = 2; v.nTasks = 2; v.patrolTrack = 7;
    Thing guard = {}; guard.type = T_CREATURE; guard.creatureProperty = 3; guard.laggy = true;
    Thing torch = {}; torch.type = T_OBJECT; torch.props = 1ul << 9; torch.hasLight = true;
    Thing crop = {}; crop.type = T_OBJECT; crop.props = 1ul << 1;
    Thing gate = {}; gate.type = T_OBJECT; gate.props = 1ul << 28;
    Thing node = {}; node.type = T_TRACK_NODE; node.isStart = true; node.track = 7;
    Thing marker = {}; marker.type = T_MARKER;
    Thing dummy = {}; dummy.type = 13; dummy.dummy = true;
    Thing lamp = {}; lamp.type = T_OBJECT; lamp.props = 1ul << 8; lamp.hasLight = true;
    Thing house = {}; house.type = T_BUILDING; house.owned.push_back(&lamp);
    Thing* all[] = { &guard, &torch, &crop, &gate, &node, &marker, &dummy, &house };
    for (int i = 0; i < 8; ++i) v.Add(*all[i]);
    if (v.numGuards != 1 || !guard.inLimbo || !guard.persist || v.torches.size() != 1 || !torch.lightOn || v.crops.size() != 1 ||
        v.cropTasks != 1 || v.gates.size() != 1 || v.patrols.size() != 1 || v.patrolTasks != 1 || v.markers.size() != 1 ||
        v.dummies.size() != 1 || v.buildings.size() != 1 || !lamp.lightOn) {
        std::printf("0065b854_TEST FAIL\n"); return 1;
    }
    std::printf("0065b854_TEST PASS\n");
    return 0;
}