// Behaviour test for 0x0082D0C0 CThingBuilding::IsEntitySerialisable() const
#include <cstdio>

class CThingBuildingModel {
public:
    unsigned char Dead : 1;
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Rest : 5;
    bool IsEntitySerialisable() const { return !CreatedByOwnerThingCreation; }
};

int main()
{
    CThingBuildingModel b;
    b.Dead = 1; b.CreatedByOwnerThingCreation = 0; b.IncludeInThingScans = 1; b.Rest = 31;
    if (!b.IsEntitySerialisable()) { std::printf("0082d0c0_TEST FAIL\n"); return 1; }
    b.CreatedByOwnerThingCreation = 1; b.Dead = 0; b.IncludeInThingScans = 0; b.Rest = 0;
    if (b.IsEntitySerialisable()) { std::printf("0082d0c0_TEST FAIL\n"); return 1; }
    std::printf("0082d0c0_TEST PASS\n");
    return 0;
}