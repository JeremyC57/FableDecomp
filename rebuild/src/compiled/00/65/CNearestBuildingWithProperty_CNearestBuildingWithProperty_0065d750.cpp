// CNearestBuildingWithProperty::CNearestBuildingWithProperty(const CNearestBuildingWithProperty& other) -- memberwise copy constructor of the 24-byte nearest-building search functor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065D750, length 43 bytes (`ret 4`, eax = this).
// Byte-identical to the landed copy constructor @ 0x0049CF80. Layout from the PDB: BuildingProperty +0x00,
// Pos +0x04 (three words), NearestBuilding +0x10, BestDistanceSquared +0x14. Every word goes through edx in
// declaration order, so the copy is an explicit memberwise copy constructor over six dword members (a float
// member would be copied through the x87, a struct member through `lea`).
// Leaf: ??0CNearestBuildingWithProperty
struct CThingBuilding;

struct CNearestBuildingWithProperty {
    long            BuildingProperty;     // +0x00 ESimBuildingProperty
    unsigned long   PosX;                 // +0x04 Pos.x (moved as a raw word)
    unsigned long   PosY;                 // +0x08 Pos.y
    unsigned long   PosZ;                 // +0x0C Pos.z
    CThingBuilding* NearestBuilding;      // +0x10
    unsigned long   BestDistanceSquared;  // +0x14 float, moved as a raw word

    CNearestBuildingWithProperty(const CNearestBuildingWithProperty& other);
};

CNearestBuildingWithProperty::CNearestBuildingWithProperty(const CNearestBuildingWithProperty& other)
{
    BuildingProperty    = other.BuildingProperty;
    PosX                = other.PosX;
    PosY                = other.PosY;
    PosZ                = other.PosZ;
    NearestBuilding     = other.NearestBuilding;
    BestDistanceSquared = other.BestDistanceSquared;
}