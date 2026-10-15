#include <cstdio>
// Behaviour test for CNearestBuildingWithProperty::CNearestBuildingWithProperty(const C3DVector&, ESimBuildingProperty) @ 0x0065846C
struct C3DVector {
    unsigned long x, y, z;
    C3DVector() : x(0), y(0), z(0) {}
    C3DVector(const C3DVector& other) : x(other.x), y(other.y), z(other.z) {}
};
class CThingBuilding;
enum ESimBuildingProperty { SIM_BUILDING_PROPERTY_NONE = 0, SIM_BUILDING_PROPERTY_SHOP = 7 };
struct CNearestBuildingWithProperty {
    ESimBuildingProperty BuildingProperty;
    C3DVector            Pos;
    CThingBuilding*      NearestBuilding;
    float                BestDistanceSquared;
    CNearestBuildingWithProperty(const C3DVector& pos, ESimBuildingProperty property)
        : BuildingProperty(property), Pos(pos), NearestBuilding(0), BestDistanceSquared(-1.0f) {}
};
int main()
{
    C3DVector pos; pos.x = 11; pos.y = 22; pos.z = 33;
    CNearestBuildingWithProperty f(pos, SIM_BUILDING_PROPERTY_SHOP);
    if (f.BuildingProperty != SIM_BUILDING_PROPERTY_SHOP || f.Pos.x != 11 || f.Pos.y != 22 || f.Pos.z != 33 ||
        f.NearestBuilding != 0 || f.BestDistanceSquared != -1.0f) { std::printf("0065846c_TEST FAIL\n"); return 1; }
    std::printf("0065846c_TEST PASS\n");
    return 0;
}
