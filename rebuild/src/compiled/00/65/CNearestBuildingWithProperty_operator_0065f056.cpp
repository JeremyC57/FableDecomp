// CNearestBuildingWithProperty::operator()(const CIntelligentPointer<CThing>&) -- building search functor: a building whose sim definition has the wanted property bit and lies nearer (squared distance) than the best so far becomes the nearest
// Grade: RELOCATION_MATCH -- GetLengthSquared written as a `s = Z*Z; s += X*X; s += Y*Y;` chain in retail's term order (the single X*X + Y*Y + Z*Z expression is reordered z,y,x by VC7.1)
// Leaf: ??RCNearestBuildingWithProperty
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
class C3DVector { public:
    float X, Y, Z;
    __forceinline C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline float GetLengthSquared() const { float s = Z * Z; s += X * X; s += Y * Y; return s; }
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
template <class T> class CDefPointer { public:
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
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { 
public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
class CNearestBuildingWithProperty { public:
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