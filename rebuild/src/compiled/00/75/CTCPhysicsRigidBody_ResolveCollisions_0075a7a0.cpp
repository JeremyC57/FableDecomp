// CTCPhysicsRigidBody::ResolveCollisions(CRigidBody::CRigidBodyConfig& config) -- impulse response for every colliding vertex
//   (Chris Hecker's rigid-body ResolveCollisions, per vertex): r = vertex - CM, v = vCM + w ^ r,
//   j = -(1 + e) (v . n) / (1/m + ((I^-1 (r ^ n)) ^ r) . n), impulse = n * j applied to the CM velocity and the angular
//   momentum, angular velocity recomputed as I^-1 * L; then both collision lists are cleared.
// Leaf: ?ResolveCollisions@CTCPhysicsRigidBody@@AAEXAAVCRigidBodyConfig@CRigidBody@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075A7A0, 726 bytes (`ret 4`; PDB Ego_r + FableWin same mangled name, private).
// Callees: CMatrix3x4::operator*(const C3DVector&) const @ 0x00A56530 (landed; engine/CMatrix3x4.h opt-in; called twice);
//          memmove 0x00BFEAE6 (STLport __copy_trivial inside CollidingVertexIndex.clear()).
// Model: the shared engine/CTCPhysicsRigidBody.h typed branch (CTCPhysicsBase base, STLport std::vector<C3DVector> CollisionNormal
//   +0x40 / std::vector<long> CollidingVertexIndex +0x4c, Body typed CRigidBody +0xa0 by the _BODY opt-in, engine/CRigidBody.h) and
//   the nested opt-in FABLE_RETAIL_CTCPHYSICSRIGIDBODY_RESOLVECOLLISIONS (declares this private member).
// Shape: count = size() taken once (signed `sar 2`, 0x0075A7AF); the vertex is a NAMED copy (its Z moves as an integer,
//   0x0075A7F6); CollisionNormal[i] is re-indexed at each use (strength-reduced byte offset [esp+10h] beside the index [esp+14h]);
//   the numerator is Hecker's `-(1 + e) * (v . n)`; the impulse is `n * (num / den)` with no named j. Both clear()s are STLport's
//   inline erase(begin(), end()).
// Header opt-ins: FABLE_RETAIL_C3DVECTOR (+ _CLEAR, needed by engine/CRigidBody.h's Reset), FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_ZYX,
//   FABLE_RETAIL_C3DVECTOR_CROSS_XZ_GROUP (overlay: keeps the first operator* call's argument set-up at 0x0075A8CF..0x0075A8D5,
//   after the r ^ n stores), FABLE_RETAIL_CMATRIX3X4_OPERATOR_MUL_VECTOR (overlay: declares 0x00A56530), and
//   FABLE_RETAIL_STLP_VECTOR_INDEX_START (overlay, stlport/fable_containers.h: vector::operator[] as `*(_M_start + n)`).
// Inferred (STLport operator[]): STLport 4.6 _vector.h spells operator[] `*(begin() + n)`; with that spelling (the default branch)
//   VC7.1 grades DIFFER(729 v 726): it drops retail's second induction slot (`mov [esp+14h],ecx` 0x0075A7BA / `mov ecx,[esp+14h]`
//   0x0075A7E0 index beside the byte offset `mov [esp+10h],ecx` 0x0075A7D4) and no longer holds &config.AngularMomentum /
//   &config.AngularVelocity in ebx / ebp (`lea ebx,[esi+48h]` 0x0075A7CB, `lea ebp,[esi+9Ch]` 0x0075A7CE). The extra inline level
//   of begin() is the single cause (isolated in repair 2: local operator[] via begin() 729 v 726, via _M_start RELOCATION_MATCH);
//   ~90 spellings with the begin() form (begin()[i], named pointer / reference / copy, const operator[], index types, every
//   CROSS_* x DOT_* opt-in, numerator / impulse spellings, inline_depth, /Ob1 /Oa /Ow) all stay at 729. Retail's TU therefore
//   indexed the vector without begin()'s inline level (a different STLport configuration of operator[], not proven).
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CLEAR
#define FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_ZYX
#define FABLE_RETAIL_C3DVECTOR_CROSS_XZ_GROUP
#define FABLE_RETAIL_CMATRIX3X4_OPERATOR_MUL_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_INDEX_START
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSRIGIDBODY
#define FABLE_RETAIL_CTCPHYSICSRIGIDBODY_BODY
#define FABLE_RETAIL_CTCPHYSICSRIGIDBODY_RESOLVECOLLISIONS
#include "engine/CTCPhysicsRigidBody.h"

void CTCPhysicsRigidBody::ResolveCollisions(CRigidBody::CRigidBodyConfig& config)
{
    long count = CollidingVertexIndex.size();
    for (long i = 0; i < count; ++i) {
        C3DVector position = config.BoundingVertices[CollidingVertexIndex[i]];
        C3DVector r = position - config.CMPosition;
        C3DVector velocity = config.CMVelocity + (config.AngularVelocity ^ r);
        float impulseNumerator = -(1.0f + Body.CoefficientOfRestitution) * velocity.Dot(CollisionNormal[i]);
        float impulseDenominator = Body.OneOverMass
            + ((config.InverseWorldInertiaTensor * (r ^ CollisionNormal[i])) ^ r).Dot(CollisionNormal[i]);
        C3DVector impulse = CollisionNormal[i] * (impulseNumerator / impulseDenominator);
        config.CMVelocity += impulse * Body.OneOverMass;
        config.AngularMomentum += r ^ impulse;
        config.AngularVelocity = config.InverseWorldInertiaTensor * config.AngularMomentum;
    }
    CollisionNormal.clear();
    CollidingVertexIndex.clear();
}