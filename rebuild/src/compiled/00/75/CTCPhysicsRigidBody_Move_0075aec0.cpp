// CTCPhysicsRigidBody::Move() -- advance the rigid body one logic frame: simulate 1/fps seconds, take the facing set from the orientation rows, then place the thing so the body-space centre of mass lands on the simulated one
// Leaf: ?Move@CTCPhysicsRigidBody@@UAEXXZ
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075AEC0, 306 bytes, thiscall, no callers: CTCPhysicsRigidBody vtable 0x0126996C slot 87
//   (rtti-vtables.tsv), the slot of CTCPhysicsBase::Move 0x006B1360 / CTCPhysicsLight::Move 0x00723360 (landed). Ego_r.pdb
//   ?Move@CTCPhysicsRigidBody@@UAEXXZ (virtual, public, void()).
// Callees: CTCPhysicsRigidBody::Simulate(float) 0x0075AD50 (landed), virtual SetRHSet slot 75 (+0x12C, landed 0x00759A00),
//   C3DMeshObject::GetPositionMatrixFromRHSet 0x00A358F0 (static __fastcall, CMatrix3x4 by value: hidden result in ecx, the
//   set in edx; Ego_r + FableWin ?GetPositionMatrixFromRHSet@C3DMeshObject@@SI?AVCMatrix3x4@@ABVCRightHandedSet@@@Z), virtual
//   SetPosition slot 30 (+0x78, CTCPhysicsBase::SetPosition 0x006B0C10). Globals: g_framesPerSecond 0x01375550 (long), 1.0f
//   0x0122DED8.
// Layout: Body (CRigidBody) +0xA0: CMPosition +0xD8, CurrentConfig +0xF0 (CMPosition +0xF0, Orientation +0xFC: GetRowY reads
//   E21..E23 +0x108, GetRowZ E31..E33 +0x114) -- Ego_r CRigidBody / CRigidBodyConfig / CMatrix3x4 offsets, the PDB's 16-byte
//   vectors 12 bytes in retail (as the landed Simulate 0x0075AD50 and SetRHSet 0x00759A00). engine/CTCPhysicsRigidBody.h
//   flattens the bases and has no virtuals, so the class is modelled locally (NOTES.md "Header debt").
// Shape: CMatrix3x4::GetRowY / GetRowZ and TransformVector(C3DVector&) inline (Ego_r QBE members; the in-place transform
//   copies X and Y to locals and reads Z from the vector, hence retail's Z-first products and the [esp+4]/[esp+8] spills);
//   C3DVector has its user copy ctor (Ego_r ??0C3DVector@@QAE@ABV0@@Z: fld X, fld Y, mov Z copies) and CMatrix3x4 its user
//   default ctor (non-POD: the __fastcall hidden result pointer goes in ecx, the set in edx).
extern long g_framesPerSecond;                              // 0x01375550

#include "engine/C3DVector.h"
class CRightHandedSet { public:
    C3DVector Up;                                           // +0x00
    C3DVector Forward;                                      // +0x0C
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) { }
};
class CMatrix3x4 { public:
    CMatrix3x4() { }
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    C3DVector GetRowY() const { return C3DVector(E21, E22, E23); }
    C3DVector GetRowZ() const { return C3DVector(E31, E32, E33); }
    void TransformVector(C3DVector& v) const
    {
        float x = v.X;
        float y = v.Y;
        v.X = x * E11 + y * E21 + v.Z * E31;
        v.Y = x * E12 + y * E22 + v.Z * E32;
        v.Z = x * E13 + y * E23 + v.Z * E33;
    }
};
class C3DMeshObject { public:
    static CMatrix3x4 __fastcall GetPositionMatrixFromRHSet(const CRightHandedSet& rhset);   // 0x00A358F0
};
template <class T> class CRetailVector { public: T* _M_start; T* _M_finish; T* _M_end_of_storage; };   // STLport vector, 12 bytes
class CRigidBodyConfig { public:                            // 0xB4 bytes in retail
    C3DVector  CMPosition;                                  // +0x00
    CMatrix3x4 Orientation;                                 // +0x0C
    C3DVector  CMVelocity;                                  // +0x3C
    C3DVector  AngularMomentum;                             // +0x48
    C3DVector  CMForce;                                     // +0x54
    C3DVector  Torque;                                      // +0x60
    CMatrix3x4 InverseWorldInertiaTensor;                   // +0x6C
    C3DVector  AngularVelocity;                             // +0x9C
    CRetailVector<C3DVector> BoundingVertices;              // +0xA8
};
class CRigidBody { public:
    float      OneOverMass;                                 // +0x00
    float      CoefficientOfRestitution;                    // +0x04
    CMatrix3x4 InverseBodyInertiaTensor;                    // +0x08
    C3DVector  CMPosition;                                  // +0x38
    CRetailVector<C3DVector> BodyBoundingVertices;          // +0x44
    CRigidBodyConfig CurrentConfig;                         // +0x50
    CRigidBodyConfig LastConfig;                            // +0x104
};

#define V6(n) virtual void v##n##a(); virtual void v##n##b(); virtual void v##n##c(); virtual void v##n##d(); virtual void v##n##e(); virtual void v##n##f();
class CTCPhysicsBase { public:                              // vtable-shaped base: only the used slots are named
    V6(0) V6(1) V6(2) V6(3) V6(4)                           // slots 0-29
    virtual void SetPosition(const C3DVector& position);    // slot 30 (+0x78)
    V6(5) V6(6) V6(7) V6(8) V6(9) V6(10) V6(11)             // slots 31-72
    virtual void v73(); virtual void v74();
    virtual void SetRHSet(const CRightHandedSet& rhset);    // slot 75 (+0x12C)
    V6(12) V6(13)                                           // slots 76-87
    unsigned char _pad04[0x40 - 0x04];
};
class CTCPhysicsRigidBody : public CTCPhysicsBase { public:
    virtual void Move();                                    // slot 87 (overrides CTCPhysicsBase::Move)
    unsigned char _pad40[0xA0 - 0x40];                      // CollisionNormal .. OffsetVector
    CRigidBody Body;                                        // +0xA0
private:
    // Private in Ego_r.pdb 0x00769E60 + FableWin.pdb 0x025C0C40 (?Simulate@CTCPhysicsRigidBody@@AAEXM@Z; agentNF32 access respelling).
    void Simulate(float deltaTime);                         // 0x0075AD50
};

void CTCPhysicsRigidBody::Move()
{
    Simulate(1.0f / (float)g_framesPerSecond);
    CRightHandedSet rhset(Body.CurrentConfig.Orientation.GetRowZ(), Body.CurrentConfig.Orientation.GetRowY());
    SetRHSet(rhset);
    CMatrix3x4 matrix = C3DMeshObject::GetPositionMatrixFromRHSet(rhset);
    C3DVector offset = Body.CMPosition;
    matrix.TransformVector(offset);
    SetPosition(Body.CurrentConfig.CMPosition - offset);
}