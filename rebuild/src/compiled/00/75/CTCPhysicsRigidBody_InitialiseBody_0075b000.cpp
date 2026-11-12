// CTCPhysicsRigidBody::InitialiseBody() -- set up the rigid body from the thing's physics mesh: the three bounding-box edge
//   lengths give the mass (3.2 per unit volume) and its inverse, the body inertia tensor is the identity with the box's inverse
//   inertia diagonal (3 / ((b^2 + c^2) * mass) per axis), restitution 1; then the body-space bounding points and the centre of
//   gravity, both configs' vertex buffers sized to match, and the current config reset (motion zeroed, position = centre of
//   gravity + thing position, orientation from the right-handed set), its vertices calculated, and copied to the last config.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0075B000, 709 bytes, thiscall, plain `ret`. FableWin.pdb ?InitialiseBody@CTCPhysicsRigidBody@@AAEXXZ
//   (private); sole caller CTCPhysicsRigidBody::OnCreate 0x0075B2D0 (tail jump).
// Callees: CTCPhysicsBase::GetPhysicsMeshObject 0x006B0850; CPhysicsMeshObject::GetBoundingBoxXEdge 0x00A11C20 / YEdge 0x00A11C90 /
//   ZEdge 0x00A11BA0 / GetRigidBodyBoundingPoints 0x00A12840 / GetCentreOfGravity 0x00A11EC0 / ~CPhysicsMeshObject 0x00A127D0;
//   std::vector<C3DVector>::resize 0x0075C040 (STLport, out of line); virtual GetRHSet (slot 72, +0x120);
//   C3DMeshObject::GetPositionMatrixFromRHSet 0x00A358F0 (static __fastcall); CalculateVertices 0x00759EC0;
//   CRigidBody::CRigidBodyConfig::operator= 0x0075BA30 (implicit, called). Inline: C3DVector::GetMagnitude / Clear / operator+,
//   CMatrix3x4::SetIdentity, CRigidBody::CRigidBodyConfig::Reset (FableWin.pdb ?Reset@CRigidBodyConfig@CRigidBody@@QAEXXZ, inline here).
// Shape: the parentheses round the Z edge length and the E33 expression generate no code; they move VC7.1's 81-tuple scheduling
//   cuts so the integer setup (zero/one registers, the bounding-points arguments) interleaves with the mass multiplies and the
//   second resize reads the size before taking the value's address, as in retail (GOTCHAS wf1/wf2 parentheses rule).
//   The resize value is one named, never-written C3DVector local because retail passes the same uninitialised frame slot to
//   both resize calls (`lea ecx,[esp+0x30] ; push ecx` at 0x0075B1AD and 0x0075B1D5, nothing stores to it); the other value
//   spellings (temporaries C3DVector() / a value per call) diverge by 23 hunks (ATTEMPTS v65/v66).
// Leaf: ?InitialiseBody@CTCPhysicsRigidBody@@AAEXXZ
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSRIGIDBODY
#define FABLE_RETAIL_CTCPHYSICSRIGIDBODY_BODY
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_EDGES
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_RIGIDBODY
#define FABLE_RETAIL_C3DMESHOBJECT_POSITIONMATRIXFROMRHSET
#define FABLE_RETAIL_CMATRIX3X4_SETIDENTITY
#define FABLE_RETAIL_C3DVECTOR_CLEAR
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_CRIGIDBODYCONFIG_ASSIGN_OUTOFLINE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsRigidBody.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/C3DMeshObject.h"
#include "engine/CRightHandedSet.h"
void CTCPhysicsRigidBody::InitialiseBody()
{
    CPhysicsMeshObject mesh = GetPhysicsMeshObject();
    float x = mesh.GetBoundingBoxXEdge().GetMagnitude();
    float y = mesh.GetBoundingBoxYEdge().GetMagnitude();
    float z = ((((mesh.GetBoundingBoxZEdge().GetMagnitude()))));
    float mass = y * z * x * 3.2f;
    Body.OneOverMass = 1.0f / mass;
    Body.InverseBodyInertiaTensor.SetIdentity();
    Body.CoefficientOfRestitution = 1.0f;
    Body.InverseBodyInertiaTensor.E11 = 3.0f / ((y * y + z * z) * mass);
    Body.InverseBodyInertiaTensor.E22 = 3.0f / ((x * x + z * z) * mass);
    Body.InverseBodyInertiaTensor.E33 = (3.0f / ((x * x + y * y) * mass));
    mesh.GetRigidBodyBoundingPoints(Body.BodyBoundingVertices);
    C3DVector centre = mesh.GetCentreOfGravity();
    Body.CMPosition = centre;
    C3DVector value;
    Body.CurrentConfig.BoundingVertices.resize(Body.BodyBoundingVertices.size(), value);
    Body.LastConfig.BoundingVertices.resize(Body.BodyBoundingVertices.size(), value);
    Body.CurrentConfig.Reset();
    Body.CurrentConfig.CMPosition = centre + Position;
    CMatrix3x4 orientation = C3DMeshObject::GetPositionMatrixFromRHSet(GetRHSet());
    Body.CurrentConfig.Orientation = orientation;
    CalculateVertices(Body.CurrentConfig);
    Body.LastConfig = Body.CurrentConfig;
}