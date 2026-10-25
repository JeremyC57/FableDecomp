// CTCDExplosion::Construct(const C3DVector& position) -- construct the thing's physics from a CTCParamsPhysicsBase holding the position and the default (up/forward) right-handed set
// Leaf: ?Construct@CTCDExplosion@@AAEXABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080C250, length 127 bytes (`ret 4`); byte twin of 0x00778D60 (the real
// CTCDCameraPoint::Construct, called from the camera-point TU). Identity (FINDINGS chunk 399): its only callers are the two
// landed CTCDExplosion::Create overloads 0x0080C460 / 0x0080C500 (Ego_r-exact leaves `?Create@CTCDExplosion@@SIPAVCThing@@J
// ABVC3DVector@@@Z` / `...0@Z`), which call it on the thing's +0x5C def component, and Ego_r.pdb has the private
// `?Construct@CTCDExplosion@@AAEXABVC3DVector@@@Z`; the catalog's CTCDCameraPoint name was a byte-twin propagation (no
// identical-code folding in retail: two bodies, two classes).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0 and its dtor @ 0x0099A300 (the manifest's "CBase::CBase"
// labels), CTCPhysicsBase::ConstructFromParams(CTCParamsBase*) (slot 3, +0xC). Vtable 0x0125C818 = CTCParamsPhysicsBase.
// Layout: Ego_r.pdb CTCParamsPhysicsBase (0x28): Position +4, Rhs +0x10 (CRightHandedSet: two C3DVectors, here (0,0,1) and
// (0,1,0)). The owner thing is the def component's +4 word and its physics component sits at the retail CThing +0x60.
// The position member is copied by C3DVector's user copy ctor (the implicit copy picks other registers).
// Shape: CBaseClassNonCopyable is the polymorphic root (its ctor stores a vtable), so the vptr is shared at +0; the params object is a stack local built by an inline ctor over the out-of-line non-copyable base (position copied
// as three dwords, then the vtable store, then the set's six constant floats); its implicit dtor calls the base dtor only.
class C3DVector {                               // a class in Ego_r.pdb (mangled V)
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct CRightHandedSet {
    C3DVector Up;                               // +0x00
    C3DVector Forward;                          // +0x0c
    CRightHandedSet() : Up(0.0f, 0.0f, 1.0f), Forward(0.0f, 1.0f, 0.0f) {}
};
struct CBaseClassNonCopyable {                  // polymorphic: its out-of-line ctor/dtor store vtables 0x0129A7C4 / 0x01231710
    virtual void v00();
    CBaseClassNonCopyable();                    // 0x0099A2F0
    ~CBaseClassNonCopyable();                   // 0x0099A300
};
struct CTCParamsBase : CBaseClassNonCopyable {
};
struct CTCParamsPhysicsBase : CTCParamsBase {   // vtable 0x0125C818, 0x28 bytes
    C3DVector Position;                         // +0x04
    CRightHandedSet Rhs;                        // +0x10
    CTCParamsPhysicsBase(const C3DVector& position) : Position(position) {}
    virtual void v00();
};
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void ConstructFromParams(CTCParamsBase* params);    // slot 3 (+0xc)
};
struct CThing {
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
};
class CTCDExplosion {
public:
    virtual void v00();
    CThing* Owner;                              // +0x04 (CTCBase::ParentThing)
private:
    void Construct(const C3DVector& position);
};

void CTCDExplosion::Construct(const C3DVector& position)
{
    CTCPhysicsBase* physics = Owner->PhysicsTC;
    CTCParamsPhysicsBase params(position);
    physics->ConstructFromParams(&params);
}