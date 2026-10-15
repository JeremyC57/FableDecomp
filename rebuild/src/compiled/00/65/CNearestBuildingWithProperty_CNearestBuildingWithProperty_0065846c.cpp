// CNearestBuildingWithProperty::CNearestBuildingWithProperty(const C3DVector& pos, ESimBuildingProperty property) -- search functor for the nearest building carrying a property: property, search position, no building found yet, best distance squared -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065846C, length 43 bytes (`ret 8`).
// Layout from the PDB (CNearestBuildingWithProperty, 24 bytes): BuildingProperty +0x00, Pos +0x04 (C3DVector),
// NearestBuilding +0x10, BestDistanceSquared +0x14. The initialiser list stores in declaration order: the
// property (second argument, read first), the position copied word by word through edx, then the null building
// pointer and the -1.0f sentinel (0xBF800000) as immediates. The `and dword ptr [eax+10h],0` zero store marks a
// size-optimised translation unit: #pragma optimize("s", on). Under it a POD C3DVector copy becomes three `movsd`;
// the retail word-by-word edx copy is C3DVector's memberwise user copy constructor (raw words, not floats:
// a float member would be copied through the x87).
// Leaf: ??0CNearestBuildingWithProperty
#pragma optimize("s", on)

struct C3DVector {                       // three float words; the copy moves them as raw dwords
    unsigned long x;
    unsigned long y;
    unsigned long z;
    C3DVector(const C3DVector& other) : x(other.x), y(other.y), z(other.z) {}
};

class CThingBuilding;
enum ESimBuildingProperty { SIM_BUILDING_PROPERTY_NONE = 0 };

struct CNearestBuildingWithProperty {
    ESimBuildingProperty BuildingProperty;   // +0x00
    C3DVector            Pos;                // +0x04
    CThingBuilding*      NearestBuilding;    // +0x10
    float                BestDistanceSquared; // +0x14

    CNearestBuildingWithProperty(const C3DVector& pos, ESimBuildingProperty property);
};

CNearestBuildingWithProperty::CNearestBuildingWithProperty(const C3DVector& pos, ESimBuildingProperty property)
    : BuildingProperty(property), Pos(pos), NearestBuilding(0), BestDistanceSquared(-1.0f)
{
}