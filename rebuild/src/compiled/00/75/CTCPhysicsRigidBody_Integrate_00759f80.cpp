// 00759f80 CTCPhysicsRigidBody::Integrate
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CLEAR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSPOSE
#define FABLE_RETAIL_CMATRIX3X4_OPERATOR_MUL_VECTOR
#define FABLE_RETAIL_CMATRIX3X4_ORTHONORMALISE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSRIGIDBODY
#define FABLE_RETAIL_CTCPHYSICSRIGIDBODY_BODY
#define FABLE_RETAIL_RIGID_INTEGRATE_PASS29
// Leaf: ?Integrate@CTCPhysicsRigidBody@@AAEXM@Z
// Flags: /O2 /Gr
#include "engine/CTCPhysicsRigidBody.h"
void CTCPhysicsRigidBody::Integrate(float delta_time)
{
    CRigidBody::CRigidBodyConfig& source = Body.LastConfig;
    CRigidBody::CRigidBodyConfig& target = Body.CurrentConfig;
    target.CMPosition = source.CMPosition + source.CMVelocity * delta_time;
    CMatrix3x4 skew_symmetric;
    skew_symmetric.InitialiseSkewedSymmetric(source.AngularVelocity);
    skew_symmetric = skew_symmetric * delta_time;
    CMatrix3x4 result = skew_symmetric * source.Orientation;
    target.Orientation = source.Orientation + result;
    target.CMVelocity = source.CMVelocity + source.CMForce * (delta_time * Body.OneOverMass);
    target.AngularMomentum = source.AngularMomentum + source.Torque * delta_time;
    target.Orientation.Orthonormalise();
    CMatrix3x4 inertia_orientation = target.Orientation * Body.InverseBodyInertiaTensor;
    CMatrix3x4 orientation_transpose = target.Orientation;
    orientation_transpose.Transpose();
    target.InverseWorldInertiaTensor = inertia_orientation * orientation_transpose;
    target.AngularVelocity = target.InverseWorldInertiaTensor * target.AngularMomentum;
}