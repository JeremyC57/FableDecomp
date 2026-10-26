// behaviour test for 0x0072F0F0 CTCDummyVillager::CTCDummyVillager(CThing*)
#include <cstdio>
class CThing { public: int id; };
struct Ptr { void* item; Ptr() : item(0) { } };
struct Base { CThing* ParentThing; bool ToBeRemoved; Base(CThing* t) : ParentThing(t), ToBeRemoved(false) { } };
struct Villager : Base {
    unsigned int DefType; Ptr Home; unsigned long long HomeUID; Ptr Work; unsigned long long WorkUID;
    Ptr Creature; unsigned long long CreatureUID; long Day; long Frame; bool Respawnable; bool Guard;
    Villager(CThing* t) : Base(t), DefType(0), Home(), HomeUID(0), Work(), WorkUID(0), Creature(), CreatureUID(0),
        Day(0x7fffffff), Frame(0x7fffffff), Respawnable(true), Guard(false) { }
};
int main()
{
    CThing t; t.id = 3;
    Villager v(&t);
    bool ok = v.ParentThing == &t && v.DefType == 0 && !v.Home.item && !v.Work.item && !v.Creature.item
        && v.HomeUID == 0 && v.WorkUID == 0 && v.CreatureUID == 0 && v.Day == 0x7fffffff && v.Frame == 0x7fffffff
        && v.Respawnable && !v.Guard;
    if (!ok) { std::printf("0072f0f0_TEST FAIL\n"); return 1; }
    std::printf("0072f0f0_TEST PASS\n");
    return 0;
}