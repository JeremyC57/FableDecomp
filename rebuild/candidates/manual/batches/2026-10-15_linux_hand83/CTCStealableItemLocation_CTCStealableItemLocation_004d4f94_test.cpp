// Behaviour test for 004d4f94 (CTCStealableItemLocation::CTCStealableItemLocation): the base takes the thing, RadiusToBeWithin starts at 1.0 and RadiusToTakeItemsBackTo at 0.0.
#include <cstdio>

struct CThing { long m_id; };

struct CTCBase_Calls {
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase_Calls(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
    virtual ~CTCBase_Calls() { }
};

struct CTCStealableItemLocation : CTCBase_Calls {
    float RadiusToBeWithin;
    float RadiusToTakeItemsBackTo;
    CTCStealableItemLocation(CThing* thing)
        : CTCBase_Calls(thing)
        , RadiusToBeWithin(1.0f)
        , RadiusToTakeItemsBackTo(0.0f)
    {
    }
};

int main()
{
    CThing thing = { 1 };
    CTCStealableItemLocation loc(&thing);
    if (loc.ParentThing != &thing || loc.RadiusToBeWithin != 1.0f || loc.RadiusToTakeItemsBackTo != 0.0f) { std::printf("004d4f94_TEST FAIL\n"); return 1; }
    std::printf("004d4f94_TEST PASS\n");
    return 0;
}
