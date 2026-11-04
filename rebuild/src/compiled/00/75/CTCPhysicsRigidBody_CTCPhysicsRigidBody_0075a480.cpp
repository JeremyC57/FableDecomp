// CTCPhysicsRigidBody::~CTCPhysicsRigidBody() -- free the physics mesh info, then the member vectors (the rigid body's two configuration vertex lists and its bounding vertices, Collidors, CollidingVertexIndex, CollisionNormal), the CTCPhysicsBase PhysicsDef release, and ~CTCBase
// Leaf: ??1CTCPhysicsRigidBody
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075A480, 162 bytes. Sole caller: the ??_G 0x0075C020 (slot 0 of
//   the CTCPhysicsRigidBody vtable 0x0126996C).
// Identity: the first store is vtable 0x0126996C (RTTI .?AVCTCPhysicsRigidBody@@ : CTCPhysicsBase : CTCBase :
//   CBaseClassNonCopyable : CBaseClass); its slot 0 ??_G is the only caller. A user-declared virtual dtor (the vtable store
//   stays); CTCPhysicsBase's is implicit (no CTCPhysicsBase vtable store: VC7.1 drops it in a compiler-generated dtor), so
//   its PhysicsDef release (+0x38) is expanded here before the tail jump to ~CTCBase 0x00686830 (??1CTCBase@@UAE@XZ).
// Callees: operator delete 0x00BFE9BC; free 0x00BFEA14 (STLport allocator<T>::deallocate, six vectors); ~CTCBase 0x00686830;
//   the PhysicsDef pointee's virtual Delete (slot 1, `call [eax+4]`).
// Layout (Ego_r.pdb, retail STLport 12-byte vectors; engine/CTCPhysicsRigidBody.h agrees): CollisionNormal +0x40,
//   CollidingVertexIndex +0x4C, Collidors +0x58, PPhysicsMeshInfo +0x64, RHSet +0x68, OldRHSet +0x80, OffsetVector +0x98,
//   Body (CRigidBody, 0x1B8) +0xA0 = BodyBoundingVertices +0x44, CurrentConfig +0x50, LastConfig +0x104 (CRigidBodyConfig
//   0xB4, BoundingVertices +0xA8) -> free order +0x24C, +0x198, +0xE4. CTCPhysicsBase (0x40): PhysicsDef +0x38.
// Shape: the body is `if (PPhysicsMeshInfo) delete PPhysicsMeshInfo;` (a bare `delete` drops retail's null test: 158 v 162);
//   then the member vectors in reverse order (trivial element dtors, _Vector_base frees _M_start when non-null).
// Header debt: engine/CTCBase.h, engine/CTCPhysicsBase.h and engine/CTCPhysicsRigidBody.h are flattened PODs (vptr word,
//   opaque vectors), so the polymorphic chain is modelled locally with the PDB member names; C3DVector / C2DVector /
//   CMatrix3x4 / CRightHandedSet / CRigidBody / CRigidBodyConfig have no headers (Ego_r layouts).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CBaseClassNonCopyable.h"
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first) _Destroy(&*__first);
}
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
    ~_Vector_base()
    {
        if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
} // namespace std

class CThing;
class CPhysicsDef;
class CThingPhysicsMeshInfo;
#include "engine/C3DVector.h"
class C2DVector { public: float X, Y; };
class CMatrix3x4 { public: float Elements[12]; };                     // 0x30 bytes (Ego_r.pdb; members not touched)
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
class CRigidBodyConfig {
public:
    C3DVector CMPosition; CMatrix3x4 Orientation; C3DVector CMVelocity; C3DVector AngularMomentum; C3DVector CMForce;
    C3DVector Torque; CMatrix3x4 InverseWorldInertiaTensor; C3DVector AngularVelocity;
    std::vector<C3DVector> BoundingVertices;                             // +0xa8
};
class CRigidBody {
public:
    float OneOverMass; float CoefficientOfRestitution; CMatrix3x4 InverseBodyInertiaTensor; C3DVector CMPosition;
    std::vector<C3DVector> BodyBoundingVertices;                         // +0x44
    CRigidBodyConfig CurrentConfig;                                      // +0x50
    CRigidBodyConfig LastConfig;                                         // +0x104
};
class CTCBase : public CBaseClassNonCopyable {
public:
    CTCBase(CThing& owner);
    virtual ~CTCBase();                                                  // 0x00686830
    CThing* ParentThing;                                                 // +0x04
    bool    ToBeRemoved;                                                 // +0x08
    bool    IsInGlobalUpdateTCs;                                         // +0x09
};
class CTCPhysicsBase : public CTCBase {
public:
    CTCPhysicsBase(CThing& owner);
    C3DVector Position;                                                  // +0x0c
    C3DVector OldPosition;                                               // +0x18
    long      OldPositionLastFrameSet;                                   // +0x24
    C3DVector Velocity;                                                  // +0x28
    float     Radius;                                                    // +0x34
    CDefPointer<const CPhysicsDef> PhysicsDef;                           // +0x38
    bool      Moving : 1;                                                // +0x3c
    bool      OldMoving : 1;
};
class CTCPhysicsRigidBody : public CTCPhysicsBase {
public:
    CTCPhysicsRigidBody(CThing& owner);
    virtual ~CTCPhysicsRigidBody();
    std::vector<C3DVector> CollisionNormal;                              // +0x40
    std::vector<long>      CollidingVertexIndex;                         // +0x4c
    std::vector<CThing*>   Collidors;                                    // +0x58
    CThingPhysicsMeshInfo* PPhysicsMeshInfo;                             // +0x64
    CRightHandedSet        RHSet;                                        // +0x68
    CRightHandedSet        OldRHSet;                                     // +0x80
    C2DVector              OffsetVector;                                 // +0x98
    CRigidBody             Body;                                         // +0xa0
};
CTCPhysicsRigidBody::~CTCPhysicsRigidBody()
{
    if (PPhysicsMeshInfo)
        delete PPhysicsMeshInfo;
}