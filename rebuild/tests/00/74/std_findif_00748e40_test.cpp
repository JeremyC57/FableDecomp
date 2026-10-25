// Behaviour test for 0x00748E40 std::__find_if (random access, 4-way unrolled + switch tail) over
// pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus> with CDeadDangerThing: the first entry whose thing
// is gone (null item), is dead, or whose danger status is DANGER_STATUS_NONE; last when every entry is live
#include <cstdio>

namespace model {

struct CThing { int Id; bool Dead; };
struct Entry {
    const CThing* Item;                               // CIntelligentPointer<const CThing>
    int DangerStatus;                                 // NStealth::CDangerStatus {DangerStatus, Timer}
    int Timer;
    const CThing* GetPItem() const { return Item; }
};
bool dead_danger_thing(const Entry& e) { return e.GetPItem() == 0 || e.GetPItem()->Dead || e.DangerStatus == 0; }

Entry* find_if(Entry* first, Entry* last)
{
    int trip_count = (int)(last - first) >> 2;
    for (; trip_count > 0; --trip_count) {
        if (dead_danger_thing(*first)) return first;
        ++first;
        if (dead_danger_thing(*first)) return first;
        ++first;
        if (dead_danger_thing(*first)) return first;
        ++first;
        if (dead_danger_thing(*first)) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (dead_danger_thing(*first)) return first;
        ++first;
    case 2:
        if (dead_danger_thing(*first)) return first;
        ++first;
    case 1:
        if (dead_danger_thing(*first)) return first;
        ++first;
    case 0:
    default:
        return last;
    }
}

}

int main()
{
    using namespace model;
    CThing things[12];
    CThing corpse;
    corpse.Id = 99;
    corpse.Dead = true;
    int bad = 0;
    for (int n = 0; n <= 11; ++n) {
        Entry v[12];
        for (int i = 0; i < n; ++i) { things[i].Id = i; things[i].Dead = false; v[i].Item = &things[i]; v[i].DangerStatus = 2; v[i].Timer = i; }
        if (find_if(v, v + n) != v + n) bad = 1;
        for (int m = 0; m < n; ++m)
            for (int why = 0; why < 3; ++why) {
                Entry saved = v[m];
                if (why == 0) v[m].Item = 0;
                else if (why == 1) v[m].Item = &corpse;
                else v[m].DangerStatus = 0;
                if (find_if(v, v + n) != v + m) bad = 2;
                v[m] = saved;
            }
    }
    if (bad) { std::printf("00748e40_TEST FAIL\n"); return 1; }
    std::printf("00748e40_TEST PASS\n");
    return 0;
}