// CAIStateGroup_Clean::OnActivate() -- once: starting at the building physics' bounding centre, step down by a constant while the point stays inside the building and keep the last inside height
// Leaf: OnActivate@CAIStateGroup_Clean
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00917520, length 185 bytes (+ int3 padding).
// Callees: own virtual slot 14 (+0x38, the building; called again for every use), CTCPhysicsBase::GetBoundingInfo
// (C3DVector*, float*) @ 0x006AF640 on the building's physics (+0x60), CThingBuilding::IsPointInside(const C3DVector&)
// @ 0x0082D000. Constant: the step 0.5f @ 0x0122F59C.
// Shape: `if (inside) { C3DVector last; while (inside) { last = pos; pos.Z -= step; } FloorHeight = last.Z; }` -- the
// pre-test is repeated before the loop, so `last` is read uninitialised when the loop never runs (retail behaviour).
struct C3DVector { float X, Y, Z; };

struct CTCPhysicsBase
{
    void GetBoundingInfo(C3DVector* centre, float* radius);    // 0x006AF640
};

struct CRefCounted                                  // reference count base at +4 of a def
{
    long Count;
    void DecRef() { --Count; }
    long GetRefCount() const { return Count; }
};

struct CMaterialDef
{
    virtual void v00();
    virtual void Destroy();                         // slot 1 (+0x04)
    CRefCounted Ref;                                // +0x04
    unsigned char _pad08[0x3c - 0x08];
    unsigned long Flags;                            // +0x3c (bit 5: not cleanable)
    void ReleaseTemporary() { Ref.DecRef(); if (Ref.GetRefCount() == 0) Destroy(); }
};

template <class T>
struct CDefPointer
{
    T* Object;
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    T* operator->() const { return Object; }
};

struct CThingBuilding
{
    unsigned char _pad00[0x60];
    CTCPhysicsBase* Physics;                        // +0x60
    bool IsPointInside(const C3DVector& point);     // 0x0082D000
    CDefPointer<CMaterialDef> PeekPMaterial();      // 0x0082D2D0 (by value, hidden pointer)
};

struct CAIStateGroup_Clean
{
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10();
    virtual bool IsRunnable();                      // slot 11 (+0x2c)
    virtual void v12();
    virtual void OnActivate();                      // slot 13 (+0x34)
    virtual CThingBuilding* GetBuilding();          // slot 14 (+0x38)
    unsigned char _pad04[0x20 - 0x04];
    float FloorHeight;                              // +0x20
    bool Activated;                                 // +0x24
};

void CAIStateGroup_Clean::OnActivate()
{
    if (Activated)
        return;
    Activated = true;
    if (GetBuilding()->Physics)
    {
        float radius;
        C3DVector pos;
        CThingBuilding* building = GetBuilding();
        building->Physics->GetBoundingInfo(&pos, &radius);
        if (GetBuilding()->IsPointInside(pos))
        {
            C3DVector last;
            while (GetBuilding()->IsPointInside(pos))
            {
                last = pos;
                pos.Z -= 0.5f;
            }
            FloorHeight = last.Z;
        }
    }
}