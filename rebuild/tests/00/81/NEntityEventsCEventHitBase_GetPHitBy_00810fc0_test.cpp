// Behaviour test for NEntityEvents::CEventHitBase::GetPHitBy 0x00810FC0: the result is the thing held by the event's hit
// parameters' PThingHitting pointer (the counted pointer at +8 is followed, then the intelligent pointer at 0x50 is read).
#include <cstdio>
struct CThing { int id; };
struct CIntelligentPointer { CThing* Item; CThing* Get() const { return Item; } };
struct CHitParameters { unsigned char Before[0x50]; CIntelligentPointer PThingHitting, PWeapon; };
struct CCountedPointer { CHitParameters* Data; void* Info; CHitParameters* operator->() const { return Data; } };
struct CEventHitBase {
    void* VPtr; long WorldFrame; CCountedPointer PHitParams;
    CThing* GetPHitBy() const { return PHitParams->PThingHitting.Get(); }
    CThing* GetPWeaponUsed() const { return PHitParams->PWeapon.Get(); }
};
int main()
{
    CThing hitter = { 1 }, weapon = { 2 };
    CHitParameters params; params.PThingHitting.Item = &hitter; params.PWeapon.Item = &weapon;
    CEventHitBase e; e.PHitParams.Data = &params; e.PHitParams.Info = 0;
    CThing* expect = &hitter;
    if (e.GetPHitBy() != expect) { std::printf("00810fc0_TEST FAIL\n"); return 1; }
    params.PThingHitting.Item = 0;
    if (e.GetPHitBy() != 0) { std::printf("00810fc0_TEST FAIL\n"); return 1; }
    std::printf("00810fc0_TEST PASS\n");
    return 0;
}