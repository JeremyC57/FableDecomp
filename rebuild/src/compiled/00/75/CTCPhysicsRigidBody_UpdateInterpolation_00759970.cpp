// CTCPhysicsRigidBody::UpdateInterpolation() -- snapshot the rigid body's right-handed set as the old one, then the base class's interpolation update
// Leaf: ?UpdateInterpolation@CTCPhysicsRigidBody@@UAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00759970, 50 bytes (int3 padding 0x0075996A..F before, 0x007599A2..AF after), thiscall,
//   a 24-byte copy +0x68 -> +0x80 through esi, tail `jmp CTCPhysicsBase::UpdateInterpolation` 0x006AF780.
// Identity (proven, two sources): (1) the only reference is slot 94 of the CTCPhysicsRigidBody vtable 0x0126996C (RTTI
//   .?AVCTCPhysicsRigidBody@@ : CTCPhysicsBase : CTCBase); slot 94 of CTCPhysicsBase 0x0125D2C4 is 0x006AF780 `jmp [vtbl+180h]`
//   (= `{ UpdateOldPos(); }`, slot 96) and slot 94 of CTCPhysicsStandard 0x01265F84 is 0x00723730 (`call [vtbl+17Ch]` = slot 95
//   UpdateOldRHSet, then `jmp 0x006AF780`) -- exactly the PDB bodies of CTCPhysicsBase / CTCPhysicsStandard::UpdateInterpolation;
//   (2) Ego_r.pdb / FableWin.pdb ?UpdateInterpolation@CTCPhysicsRigidBody@@UAEXXZ (public virtual over the protected base
//   ?UpdateInterpolation@CTCPhysicsBase@@MAEXXZ), and the Ego_r donor order ConstructFromParams / UpdateInterpolation /
//   GetFacingAngleXZ / SetFacingAnglesFromRHSet / SetRHSet = retail 0x00759900 / 0x00759970 / 0x007599B0 / 0x007599C0 / 0x00759A00
//   (all landed but this row). The forge label vfunc_94 was a placeholder.
// Callee: CTCPhysicsBase::UpdateInterpolation() 0x006AF780 (protected virtual, called qualified = direct tail jump; its current
//   binding ?InitialiseBase@CCreatureAction_TrollWhackGroundBase_Methods@@QAEXXZ is wrong -> NAME_FIXES).
// Layout: Ego_r CTCPhysicsRigidBody RHSet +116 / OldRHSet +140 after three Dinkumware vectors (16 bytes) at +64; retail STLport
//   vectors are 12 bytes, so RHSet +0x68 / OldRHSet +0x80 (the landed SetRHSet 0x00759A00 / GetRHSet 0x00759B40 read the same
//   displacements). CRightHandedSet = Ego_r {C3DVector Up +0, C3DVector Forward +0xC} (24 bytes, no engine header). engine/CTCPhysicsBase.h
//   and engine/CTCPhysicsRigidBody.h are flattened (vptr as data, PDB offsets), so the classes are restated here over the
//   engine/CTCBase.h retail opt-in (header debt).
//   Every retail member is present: OffsetVector C2DVector +0x98 and Body (CRigidBody, opaque Body_[0x1B8]) +0xA0 to the retail
//   sizeof 0x258 (operator new size at 0x004D399D; the ctor 0x0075AA80 zeroes the three vectors and Body's vectors at +0xE4/+0x198/+0x24C).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"

namespace std {
template <class _Tp> class allocator { public: };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> { };
}

class CPhysicsDef;
class CThingPhysicsMeshInfo;

#include "engine/C3DVector.h"

class C2DVector {
public:
    float X;
    float Y;
};

class CRightHandedSet {
public:
    C3DVector Up;                                           // +0x00
    C3DVector Forward;                                      // +0x0C
};

class CTCPhysicsBase : public CTCBase {                      // RTTI .?AVCTCPhysicsBase@@, vtable 0x0125D2C4, 0x40 bytes
protected:
    virtual void UpdateInterpolation();                     // slot 94: 0x006AF780
public:
    C3DVector     Position;                                 // +0x0C
    C3DVector     OldPosition;                              // +0x18
    long          OldPositionLastFrameSet;                  // +0x24
    C3DVector     Velocity;                                 // +0x28
    float         Radius;                                   // +0x34
    CDefPointer<const CPhysicsDef> PhysicsDef;              // +0x38 (engine/CDefPointer.h)
    bool          Moving : 1;                               // +0x3C
    bool          OldMoving : 1;
};

class CTCPhysicsRigidBody : public CTCPhysicsBase {          // vtable 0x0126996C
public:
    virtual void UpdateInterpolation();
    std::vector<C3DVector> CollisionNormal;                 // +0x40
    std::vector<long>      CollidingVertexIndex;            // +0x4C
    std::vector<CThing*>   Collidors;                       // +0x58
    CThingPhysicsMeshInfo* PPhysicsMeshInfo;                // +0x64
    CRightHandedSet        RHSet;                           // +0x68
    CRightHandedSet        OldRHSet;                        // +0x80
    C2DVector              OffsetVector;                    // +0x98
    unsigned char          Body_[0x1B8];                    // CRigidBody Body +0xA0 (opaque: no CRigidBody model yet; Ego_r 0x1C4 less
                                                            //   three STLport vectors, ctor 0x0075AA80 zeroes them at +0xE4/+0x198/+0x24C)
};

FABLE_STATIC_ASSERT(sizeof(CTCPhysicsRigidBody) == 0x258);  // retail `push 258h ; call operator new` at 0x004D399D
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsRigidBody, RHSet) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CTCPhysicsRigidBody, OldRHSet) == 0x80);

void CTCPhysicsRigidBody::UpdateInterpolation()
{
    OldRHSet = RHSet;
    CTCPhysicsBase::UpdateInterpolation();
}