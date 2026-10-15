// Behaviour test for CTCTargetingPlayer::PostProcessTargetedThing @ 0x00803B20: refresh threatening when the target changed, return the parent check.
#include <cstdio>
struct CThing { bool controlled; bool IsControlledCreature() { return controlled; } };
struct CIntelligentPointer { CThing* p; CThing* Get() const { return p; } };
struct CTCTargetingPlayer {
    CThing* ParentThing; CIntelligentPointer PLastThreatenedThing; CThing* targeted; int updates;
    virtual ~CTCTargetingPlayer() {}
    virtual CThing* GetPTargetedThing() { return targeted; }
    void UpdateTargetThreatening() { ++updates; PLastThreatenedThing.p = targeted; }
    bool PostProcessTargetedThing()
    {
        CThing* last = PLastThreatenedThing.Get();
        if (GetPTargetedThing() != last) UpdateTargetThreatening();
        return ParentThing->IsControlledCreature();
    }
};
int main()
{
    CThing parent, a; parent.controlled = true; CTCTargetingPlayer p; p.ParentThing = &parent; p.PLastThreatenedThing.p = 0; p.targeted = &a; p.updates = 0;
    if (!p.PostProcessTargetedThing() || p.updates != 1 || p.PLastThreatenedThing.p != &a) { std::printf("00803b20_TEST FAIL\n"); return 1; }
    parent.controlled = false;
    if (p.PostProcessTargetedThing() || p.updates != 1) { std::printf("00803b20_TEST FAIL\n"); return 1; }
    std::printf("00803b20_TEST PASS\n");
    return 0;
}
