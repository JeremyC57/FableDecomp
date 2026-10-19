// CNearestBuildingWithProperty::operator()(const CIntelligentPointer<CThing>&) -- building search functor: a building whose sim definition has the wanted property bit and lies nearer (squared distance) than the best so far becomes the nearest
// Leaf: ??RCNearestBuildingWithProperty
// Grade: DIFFER(164v164) -- float sum order only: retail adds the squares as z*z + x*x + y*y, the lane always emits z*z + y*y + x*x (same for X*X+Y*Y+Z*Z and Y*Y+X*X+Z*Z; the GOTCHAS four-term reorder gap); everything else matches under optimize(y off, s on) with __forceinline vector helpers
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065F056, 164 bytes (`ret 4`). Size-optimised TU with a frame pointer: `#pragma optimize("y",
//   off)` + `#pragma optimize("s", on)` (GOTCHAS: a frame-pointer body ending in `leave` that still uses esi freely).
// Callees: CBaseIntelligentPointer::GetPItem() 0x00A01B50 (three times), CThingBuilding::PeekPSimDefinition() 0x0082D2D0
//   (CDefPointer returned through the hidden pointer; GOTCHAS: not PeekPMaterial), CThing::GetPosition() 0x004C73D0; the def's
//   virtual slot 1 (+4) when the temporary pointer drops the last reference.
// Layout = Ego_r.pdb CNearestBuildingWithProperty (24 bytes): BuildingProperty +0 (ESimBuildingProperty bit mask), Pos +4,
//   NearestBuilding +0x10, BestDistanceSquared +0x14; the sim def's property bits at +0x3c, its reference count at +4.
// Shape: the vector helpers are __forceinline (the TU is /Os); the def pointer is a scoped CDefPointer local (inline release at the end, skipped when null); the squared distance
//   is a named float (`fst` into the frame) compared only when a building is already held (`test ah,5 ; jp`: strictly nearer).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct C3DVector {
    float X, Y, Z;
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
struct CRefCounted {                               // reference count base at +4 of a def
    long Count;
    void DecRef() { --Count; }
    long GetRefCount() const { return Count; }
};
struct CBuildingSimDefinition {
    virtual void v00();
    virtual void Destroy();                        // slot 1 (+0x04)
    CRefCounted   Ref;                             // +0x04
    unsigned char _pad08[0x3c - 0x08];
    unsigned long BuildingProperties;              // +0x3c
    void ReleaseTemporary() { Ref.DecRef(); if (Ref.GetRefCount() == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    T* operator->() const { return Object; }
};
class CThing {
public:
    const C3DVector& GetPosition() const;          // 0x004C73D0
};
class CThingBuilding : public CThing {
public:
    CDefPointer<CBuildingSimDefinition> PeekPSimDefinition() const;   // 0x0082D2D0
};
class CBaseIntelligentPointer {
public:
    void* m_pItem;
    CThing* GetPItem() const;                      // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
struct CNearestBuildingWithProperty {
    unsigned long   BuildingProperty;              // +0x00 ESimBuildingProperty
    C3DVector       Pos;                           // +0x04
    CThingBuilding* NearestBuilding;               // +0x10
    float           BestDistanceSquared;           // +0x14
    void operator()(const CIntelligentPointer<CThing>& thing);
};

void CNearestBuildingWithProperty::operator()(const CIntelligentPointer<CThing>& thing)
{
    CDefPointer<CBuildingSimDefinition> def = ((CThingBuilding*)thing.GetPItem())->PeekPSimDefinition();
    if (def.Object) {
        if (def->BuildingProperties & BuildingProperty) {
            float distanceSquared = (thing.GetPItem()->GetPosition() - Pos).GetLengthSquared();
            if (NearestBuilding == 0 || distanceSquared < BestDistanceSquared) {
                NearestBuilding = (CThingBuilding*)thing.GetPItem();
                BestDistanceSquared = distanceSquared;
            }
        }
    }
}
