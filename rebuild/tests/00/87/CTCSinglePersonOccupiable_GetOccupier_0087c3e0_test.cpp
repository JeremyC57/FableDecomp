// Behaviour test for CTCSinglePersonOccupiable::GetOccupier 0x0087C3E0: the occupier is whatever SetOccupier last stored in the
// POccupier pointer (+0x0C); a cleared occupiable reports no occupier.
#include <cstdio>
struct CThingCreatureBase { int id; };
struct CIntelligentPointer { CThingCreatureBase* Item; CThingCreatureBase* Get() const { return Item; } void Set(CThingCreatureBase* c) { Item = c; } };
struct CTCSinglePersonOccupiable {
    void* VPtr; void* ParentThing; int Flags; CIntelligentPointer POccupier;
    void SetOccupier(CThingCreatureBase* c) { POccupier.Set(c); }
    CThingCreatureBase* GetOccupier() const { return POccupier.Get(); }
};
int main()
{
    CThingCreatureBase sleeper = { 4 };
    CTCSinglePersonOccupiable bed; bed.POccupier.Item = 0;
    if (bed.GetOccupier() != 0) { std::printf("0087c3e0_TEST FAIL\n"); return 1; }
    bed.SetOccupier(&sleeper);
    if (bed.GetOccupier() != &sleeper) { std::printf("0087c3e0_TEST FAIL\n"); return 1; }
    bed.SetOccupier(0);
    if (bed.GetOccupier() != 0) { std::printf("0087c3e0_TEST FAIL\n"); return 1; }
    std::printf("0087c3e0_TEST PASS\n");
    return 0;
}