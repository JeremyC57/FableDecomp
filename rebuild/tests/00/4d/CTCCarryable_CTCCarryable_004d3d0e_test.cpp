// Behaviour test for 004d3d0e (CTCCarryable::CTCCarryable): the base takes the thing; not fragile, no carrier, zero attach offset and offset amount.
#include <cstdio>

class CThing { public: long m_id; };
class CTCCarrying;

class C3DVector { public:
    float X;
    float Y;
    float Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

struct CTCBase_Calls {
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase_Calls(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
    virtual ~CTCBase_Calls() { }
};

class CTCCarryable : public CTCBase_Calls { public:
    bool         ForceFragility;
    CTCCarrying* PCarryingTC;
    C3DVector    OffsetCoordRelativeToAttachToDummy;
    float        OffsetAmount;
    CTCCarryable(CThing* thing)
        : CTCBase_Calls(thing)
        , ForceFragility(false)
        , PCarryingTC(0)
        , OffsetCoordRelativeToAttachToDummy(0.0f, 0.0f, 0.0f)
        , OffsetAmount(0.0f)
    {
    }
};

int main()
{
    CThing thing = { 1 };
    CTCCarryable c(&thing);
    if (c.ParentThing != &thing || c.ForceFragility || c.PCarryingTC != 0 || c.OffsetCoordRelativeToAttachToDummy.X != 0.0f
        || c.OffsetCoordRelativeToAttachToDummy.Y != 0.0f || c.OffsetCoordRelativeToAttachToDummy.Z != 0.0f || c.OffsetAmount != 0.0f) {
        std::printf("004d3d0e_TEST FAIL\n"); return 1;
    }
    std::printf("004d3d0e_TEST PASS\n");
    return 0;
}