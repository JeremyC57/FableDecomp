#include <cstdio>
// Behaviour test for CNearestBuildingWithProperty::CNearestBuildingWithProperty(const CNearestBuildingWithProperty&) @ 0x0065D750
struct CThingBuilding { int m_id; };
struct CNearestBuildingWithProperty {
    long            BuildingProperty;
    unsigned long   PosX, PosY, PosZ;
    CThingBuilding* NearestBuilding;
    unsigned long   BestDistanceSquared;
    CNearestBuildingWithProperty() {}
    CNearestBuildingWithProperty(const CNearestBuildingWithProperty& other)
    {
        BuildingProperty = other.BuildingProperty; PosX = other.PosX; PosY = other.PosY; PosZ = other.PosZ;
        NearestBuilding = other.NearestBuilding; BestDistanceSquared = other.BestDistanceSquared;
    }
};
int main()
{
    CThingBuilding b; b.m_id = 5;
    CNearestBuildingWithProperty a;
    a.BuildingProperty = 3; a.PosX = 1; a.PosY = 2; a.PosZ = 0xdeadbeef; a.NearestBuilding = &b; a.BestDistanceSquared = 0x3f800000;
    CNearestBuildingWithProperty c(a);
    if (c.BuildingProperty != 3 || c.PosX != 1 || c.PosY != 2 || c.PosZ != 0xdeadbeef || c.NearestBuilding != &b ||
        c.BestDistanceSquared != 0x3f800000) { std::printf("0065d750_TEST FAIL\n"); return 1; }
    std::printf("0065d750_TEST PASS\n");
    return 0;
}