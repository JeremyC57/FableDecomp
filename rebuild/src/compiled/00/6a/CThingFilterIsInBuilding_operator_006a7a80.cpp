// CThingFilter_IsInBuilding::operator()(CThing* thing) -- true when the thing's position lies inside the filter's Building (CThingBuilding::IsPointInside)
// Leaf: ??RCThingFilter_IsInBuilding
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A7A80, length 24 bytes.
// Callees: CThing::GetPosition @ 0x004C73D0 (`PhysicsTC + 0x0C`, out of line; the manifest's "DoSizeof" label is
// propagated), CThingBuilding::IsPointInside @ 0x0082D000.
// Layout: PDB CThingFilter_IsInBuilding { CThingBuilding* Building; } (+0).
// Shape: the argument is evaluated before the object pointer (`call GetPosition ; mov ecx,[esi] ; push eax`).
struct C3DVector {
    float x, y, z;
};

struct CThing {
    const C3DVector& GetPosition() const;           // 0x004C73D0
};

struct CThingBuilding {
    bool IsPointInside(const C3DVector& pos);       // 0x0082D000
};

struct CThingFilter_IsInBuilding {
    CThingBuilding* Building;                       // +0x00
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsInBuilding::operator()(CThing* thing) const
{
    return Building->IsPointInside(thing->GetPosition());
}