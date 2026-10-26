// Test for 006818f0 CTCCreatureOpinionOfHero::IsWillingToContinueFollowingHero: true for attitudes 0-2, 5, 6, 9-16.
#include <cstdio>
class CThing { public: long Attitude; };
class COpinionReactionManager { public:
    const CThing* LastThing;
    long GetThingAttitudeToPlayer(const CThing* thing) { LastThing = thing; return thing->Attitude; }
};
class CWorld { public: COpinionReactionManager* POpinionReactionManager; };
class CTCCreatureOpinionOfHero { public:
    CThing* ParentThing; CWorld* World;
    CWorld* GetWorld() const { return World; }
    bool IsWillingToContinueFollowingHero() const
    {
        CThing* thing = ParentThing;
        COpinionReactionManager* manager = GetWorld()->POpinionReactionManager;
        switch (manager->GetThingAttitudeToPlayer(thing)) {
        case 0: case 1: case 2: case 5: case 6: case 9: case 10: case 11: case 12: case 13: case 14: case 15: case 16:
            return true;
        default:
            return false;
        }
    }
};
int main()
{
    // the retail jump-table index bytes 00 00 00 01 01 00 00 01 01 00 00 00 00 00 00 00 00 (0 = true) for attitudes 0..16
    static const bool expected[17] = { true, true, true, false, false, true, true, false, false,
                                       true, true, true, true, true, true, true, true };
    COpinionReactionManager m = { 0 };
    CWorld w = { &m };
    CThing t = { 0 };
    CTCCreatureOpinionOfHero o = { &t, &w };
    for (long a = -2; a <= 20; ++a) {
        t.Attitude = a;
        bool want = (a >= 0 && a <= 16) ? expected[a] : false;
        if (o.IsWillingToContinueFollowingHero() != want || m.LastThing != &t) { std::printf("006818f0_TEST FAIL\n"); return 1; }
    }
    std::printf("006818f0_TEST PASS\n");
    return 0;
}