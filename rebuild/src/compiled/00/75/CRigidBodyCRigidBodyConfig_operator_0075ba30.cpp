// CRigidBody::CRigidBodyConfig::operator=(const CRigidBodyConfig& other) -- the compiler-generated memberwise assignment of a rigid
// body's physical state.
// Leaf: ??4CRigidBodyConfig@CRigidBody@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075BA30, length 196 bytes (`ret 4`, returns `this`).
// Callees: std::vector<C3DVector>::operator=(const vector&) @ 0x0075B8D0 (BoundingVertices).
// Name (agentNF32, 2026-11-12; was the top-level struct ??4CRigidBodyConfig@@QAEAAU0@ABU0@@Z): Ego_r.pdb 0x0076AA50 and FableWin.pdb
//   0x025C0AF0 ??4CRigidBodyConfig@CRigidBody@@QAEAAV01@ABV01@@Z (nested class CRigidBody::CRigidBodyConfig); the landed 0x0075B000
//   CTCPhysicsRigidBody::InitialiseBody binds this address under that symbol through engine/CRigidBody.h. Now on that shared header.
// Layout: engine/CRigidBody.h (Ego_r CRigidBodyConfig at retail offsets, STLport 12-byte vector: 0xb4).
// Shape: no user operator= -- the implicit one is emitted for the one-line `a = b` helper below. C3DVector has a
// user copy constructor but no operator=, so each vector member is copied through a pair of re-based pointer
// registers; the plain 3x4 matrices are `rep movsd` blocks; the vector member goes through its out-of-line assignment
// last (FABLE_RETAIL_STLP_VECTOR_ASSIGN: declared, called out of line).
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_C3DVECTOR_CLEAR                // engine/CRigidBody.h Reset() needs C3DVector::Clear
#include "engine/CRigidBody.h"

void AssignRigidBodyConfig(CRigidBody::CRigidBodyConfig& to, const CRigidBody::CRigidBodyConfig& from)
{
    to = from;
}
