// Behaviour test for CTCTargetingPlayer::UpdateNearestInterestingThing @ 0x008023E0: cleared, then the targeted thing or the base scan.
#include <cstdio>
class CThing { public: int id; };
class CTCTargetingBase { public:
    CThing* nearest; CThing* scanResult; int sets;
    virtual ~CTCTargetingBase() {}
    virtual CThing* GetPTargetedThing() { return 0; }
    virtual void UpdateNearestInterestingThing() { SetPNearestInterestingThing(scanResult); }
    void SetPNearestInterestingThing(CThing* t) { nearest = t; ++sets; }
};
class CTCTargetingPlayer : public CTCTargetingBase { public:
    CThing* targeted;
    virtual CThing* GetPTargetedThing() { return targeted; }
    virtual void UpdateNearestInterestingThing()
    {
        SetPNearestInterestingThing(0);
        if (GetPTargetedThing()) SetPNearestInterestingThing(GetPTargetedThing()); else CTCTargetingBase::UpdateNearestInterestingThing();
    }
};
int main()
{
    CThing a, b; CTCTargetingPlayer p; p.sets = 0; p.nearest = &a; p.scanResult = &b; p.targeted = 0;
    p.UpdateNearestInterestingThing();
    if (p.nearest != &b || p.sets != 2) { std::printf("008023e0_TEST FAIL\n"); return 1; }
    p.targeted = &a; p.UpdateNearestInterestingThing();
    if (p.nearest != &a || p.sets != 4) { std::printf("008023e0_TEST FAIL\n"); return 1; }
    std::printf("008023e0_TEST PASS\n");
    return 0;
}