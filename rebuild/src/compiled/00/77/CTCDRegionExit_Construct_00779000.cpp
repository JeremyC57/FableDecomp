// CTCDRegionExit::Construct(const C3DVector& position) -- place the owning thing's physics component at the position with the identity axes: build a CTCParamsPhysicsBase(position, CRightHandedSet(Up (0,0,1), Forward (0,1,0))) on the stack and hand it to the physics component's virtual ConstructFromParams
// Leaf: ?Construct@CTCDRegionExit@@AAEXABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00779000, 127 bytes, thiscall, `ret 4`.
// Identity (proven): CTCDRegionExit::Construct(const C3DVector&), private (Ego_r.pdb and FableWin.pdb both hold
//   ?Construct@CTCDRegionExit@@AAEXABVC3DVector@@@Z); evidence: it follows the CTCDRegionExit slot-29 override 0x00778F90 (vtable 0x0126AB54, RTTI .?AVCTCDRegionExit@@) and the constructor 0x00778FA0 and precedes CTCDRegionExit::MoveHeroThroughExit 0x00779080; its one caller is CTCDRegionExit::Create(long, const C3DVector&) 0x00779650 (Ego_r.pdb Construct 0x007364A0, Create 0x00736910)
// Label: the manifest's bare "Construct" is kept as the leaf; the manifest propagates "CTCDCameraPoint::Construct" across this byte-identical family; this address is the region exit's.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable() 0x0099A2F0 (??0CBaseClassNonCopyable@@IAE@XZ) and
//   CBaseClassNonCopyable::~CBaseClassNonCopyable() 0x0099A300 (??1CBaseClassNonCopyable@@MAE@XZ), both catalog-bound and
//   PDB-confirmed; the physics component's slot 3 (+0x0C) is CTCPhysicsBase::ConstructFromParams(const CTCParamsBase&)
//   (?ConstructFromParams@CTCPhysicsBase@@UAEXABVCTCParamsBase@@@Z, Ego_r.pdb), called through the vtable.
// Body: the CTCParamsPhysicsBase constructor is inline here (base ctor call + vtable 0x0125C818 store + the 36-byte
//   Position/Rhs copy), its implicit destructor collapses to the base destructor call.
// Layout: `this` is the engine CTCBase header (ParentThing +0x04); retail CThing PhysicsTC +0x60 (Ego_r.pdb +0x70; the
//   landed CThingVillage::Create 0x0083B7F0 and CThingAICreature::Create 0x00833800 read it there); the params object is
//   vptr + Position + CRightHandedSet (0x28 bytes). The physics component is the engine CTCPhysicsBase header dispatched
//   through a method-only adapter (the landed CThingVillage::Create model).
#include "engine/CTCBase.h"
#include "engine/CTCPhysicsBase.h"

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class CRightHandedSet { public:
    C3DVector Up;                               // +0x00
    C3DVector Forward;                          // +0x0c
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) {}
    CRightHandedSet(const CRightHandedSet& o) : Up(o.Up), Forward(o.Forward) {}
};
class CBaseClass {
public:
    CBaseClass();                               // 0x0099A2D0
    virtual ~CBaseClass();                      // 0x0099A2E0
};
class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                    // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();           // 0x0099A300
};
class CTCParamsBase : public CBaseClassNonCopyable {
};
class CTCParamsPhysicsBase : public CTCParamsBase {   // vtable 0x0125C818, 0x28 bytes
public:
    C3DVector       Position;                   // +0x04
    CRightHandedSet Rhs;                        // +0x10
    CTCParamsPhysicsBase(const C3DVector& position, const CRightHandedSet& rhs) : Position(position), Rhs(rhs) {}
};
struct CTCPhysicsMethods {                      // method-only adapter over the engine CTCPhysicsBase
    virtual void Slot0(); virtual void Slot1(); virtual void Slot2();
    virtual void ConstructFromParams(const CTCParamsBase& params);    // slot 3 (+0x0C)
};
class CThing { public:
    unsigned char     _pad00[0x60];
    ::CTCPhysicsBase* PhysicsTC;                // +0x60 (Ego_r.pdb +0x70)
};
class CTCDRegionExit : public CTCBase {
private:
    void Construct(const C3DVector& position);
};

void CTCDRegionExit::Construct(const C3DVector& position)
{
    ::CTCPhysicsBase* physics = ParentThing->PhysicsTC;
    CTCParamsPhysicsBase params(position, CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f)));
    reinterpret_cast<CTCPhysicsMethods*>(physics)->ConstructFromParams(params);
}