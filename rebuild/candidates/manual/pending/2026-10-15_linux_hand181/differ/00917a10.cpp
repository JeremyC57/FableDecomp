// CAIStateGroup_Clean::IsRunnable() -- runnable when there is a building whose material is cleanable (flag 0x20 clear), which has physics, and whose bounding centre lies inside it
// Leaf: IsRunnable@CAIStateGroup_Clean
// Grade: DIFFER(184v184) -- scheduling only: in the early-exit release retail stores the decremented count before `mov edx,esi ; test` (ours after); the end-of-function release matches
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00917A10, length 184 bytes (+ int3 padding).
// Callees: own virtual slot 14 (+0x38, the building), CThingBuilding::PeekPMaterial @ 0x0082D2D0 (CDefPointer<CMaterialDef>
// by value), CTCPhysicsBase::GetBoundingInfo(C3DVector*, float*) @ 0x006AF640, CThingBuilding::IsPointInside @ 0x0082D000.
// Shape: the material CDefPointer local is released inline on every exit (null test, then the def's inline
// ReleaseTemporary() through the ref-count base at +4: `add eax,4 ; dec ; test ; jne ; call [vtbl+4]`); the inside test
// result is held in bl across that release.
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

bool CAIStateGroup_Clean::IsRunnable()
{
    if (!GetBuilding())
        return false;
    CDefPointer<CMaterialDef> material = GetBuilding()->PeekPMaterial();
    if (material.Object && (material->Flags & 0x20))
        return false;
    if (!GetBuilding()->Physics)
        return false;
    float radius;
    C3DVector pos;
    CThingBuilding* building = GetBuilding();
    building->Physics->GetBoundingInfo(&pos, &radius);
    return GetBuilding()->IsPointInside(pos);
}
