#include <cstdio>
// Behaviour test for CCreatureInteraction::RemoveCreature (0x008631B0): same logic over a copy of the model.
struct CThing { int id; };
struct CThingCreatureBase : CThing { };
struct CThingPointerList {
    CThing* items[4]; int count;
    void remove(CThing* const& value) { int w = 0; for (int r = 0; r < count; ++r) if (items[r] != value) items[w++] = items[r]; count = w; }
};
struct CCreatureInteraction {
    CThingPointerList Participants;
    void RemoveCreature(CThingCreatureBase* creature) { Participants.remove(creature); }
};
int main()
{
    CThingCreatureBase a, b;
    CCreatureInteraction ci; ci.Participants.count = 3; ci.Participants.items[0] = &a; ci.Participants.items[1] = &b; ci.Participants.items[2] = &a;
    ci.RemoveCreature(&a);
    if (ci.Participants.count != 1 || ci.Participants.items[0] != &b) { std::printf("008631b0_TEST FAIL\n"); return 1; }
    std::printf("008631b0_TEST PASS\n");
    return 0;
}