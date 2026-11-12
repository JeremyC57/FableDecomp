// Behaviour test for CTCPhysicsRigidBody::ResolveCollisions (0x0075A7A0): the row body compiled over the same shared headers and
// opt-ins as the row (engine/CTCPhysicsRigidBody.h typed branch, engine/CRigidBody.h, STLport vectors), run on a real object and
// checked against hand-computed impulse results, plus the two list clears. The private member and the protected lists are reached
// with the explicit-instantiation access idiom (no local model of the class); the object lives in raw storage because the class is
// abstract (pure virtuals in CTCBase) and ResolveCollisions touches no virtual.
#include <cstdio>
#include <cmath>
#include <cstring>
#include <new>
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

// The real body of CMatrix3x4::operator*(const C3DVector&) const is the landed row 0x00A56530 (the three row dot products); the
// header opt-in only declares it, so the test links this same body.
C3DVector CMatrix3x4::operator*(const C3DVector& v) const
{
    C3DVector result;
    result.X = E11 * v.X + E12 * v.Y + E13 * v.Z;
    result.Y = E21 * v.X + E22 * v.Y + E23 * v.Z;
    result.Z = E31 * v.X + E32 * v.Y + E33 * v.Z;
    return result;
}

// The row body (0x0075A7A0), as in the row source.
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

// Explicit-instantiation access idiom: template arguments of an explicit instantiation are not access-checked.
template <class Tag, typename Tag::type M> struct AccessMember { friend typename Tag::type GetMember(Tag) { return M; } };
struct NormalsTag { typedef std::vector<C3DVector> CTCPhysicsRigidBody::*type; friend type GetMember(NormalsTag); };
struct IndicesTag { typedef std::vector<long> CTCPhysicsRigidBody::*type; friend type GetMember(IndicesTag); };
struct BodyTag { typedef CRigidBody CTCPhysicsRigidBody::*type; friend type GetMember(BodyTag); };
struct ResolveTag { typedef void (CTCPhysicsRigidBody::*type)(CRigidBody::CRigidBodyConfig&); friend type GetMember(ResolveTag); };
template struct AccessMember<NormalsTag, &CTCPhysicsRigidBody::CollisionNormal>;
template struct AccessMember<IndicesTag, &CTCPhysicsRigidBody::CollidingVertexIndex>;
template struct AccessMember<BodyTag, &CTCPhysicsRigidBody::Body>;
template struct AccessMember<ResolveTag, &CTCPhysicsRigidBody::ResolveCollisions>;

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
static double g_storage[sizeof(CTCPhysicsRigidBody) / sizeof(double) + 1];

static void ZeroConfig(CRigidBody::CRigidBodyConfig& c)
{
    c.CMPosition = C3DVector(0, 0, 0); c.CMVelocity = c.CMPosition; c.AngularMomentum = c.CMPosition;
    c.CMForce = c.CMPosition; c.Torque = c.CMPosition; c.AngularVelocity = c.CMPosition;
    std::memset(&c.Orientation, 0, sizeof c.Orientation);
    std::memset(&c.InverseWorldInertiaTensor, 0, sizeof c.InverseWorldInertiaTensor);
    c.InverseWorldInertiaTensor.E11 = 1; c.InverseWorldInertiaTensor.E22 = 1; c.InverseWorldInertiaTensor.E33 = 1;
}

int main()
{
    std::memset(g_storage, 0, sizeof g_storage);
    CTCPhysicsRigidBody* body = reinterpret_cast<CTCPhysicsRigidBody*>(g_storage);
    std::vector<C3DVector>& normals = body->*GetMember(NormalsTag());
    std::vector<long>& indices = body->*GetMember(IndicesTag());
    CRigidBody& rigid = body->*GetMember(BodyTag());
    void (CTCPhysicsRigidBody::*resolve)(CRigidBody::CRigidBodyConfig&) = GetMember(ResolveTag());
    new (&normals) std::vector<C3DVector>(1, C3DVector(0, 0, 1));
    new (&indices) std::vector<long>(1, 1L);

    // Vertex 1 straight below the CM: r = (0,0,-1), v.n = -2, e = 1 -> num = 4; r^n = 0 so den = 1/m = 0.5; j = 8; dv = 4.
    CRigidBody::CRigidBodyConfig c;
    ZeroConfig(c);
    c.BoundingVertices.push_back(C3DVector(5, 5, 5));
    c.BoundingVertices.push_back(C3DVector(0, 0, -1));
    c.CMVelocity = C3DVector(0, 0, -2);
    rigid.OneOverMass = 0.5f; rigid.CoefficientOfRestitution = 1.0f;
    (body->*resolve)(c);
    if (!Near(c.CMVelocity.Z, 2.0f) || !Near(c.CMVelocity.X, 0) || !Near(c.AngularMomentum.X, 0)
        || normals.size() != 0 || indices.size() != 0) {
        std::printf("0075a7a0_TEST FAIL\n"); return 1;
    }

    // Off-centre contact: r = (1,0,0), v = (0,0,-1), n = (0,0,1), e = 0, 1/m = 1, I^-1 = identity:
    // r^n = (0,-1,0); (r^n)^r = (0,0,1); den = 1 + 1 = 2; num = 1; j = 0.5; L += r ^ (0,0,0.5) = (0,-0.5,0); w = I^-1 L.
    normals.push_back(C3DVector(0, 0, 1));
    indices.push_back(0L);
    CRigidBody::CRigidBodyConfig d;
    ZeroConfig(d);
    d.BoundingVertices.push_back(C3DVector(1, 0, 0));
    d.CMVelocity = C3DVector(0, 0, -1);
    rigid.OneOverMass = 1.0f; rigid.CoefficientOfRestitution = 0.0f;
    (body->*resolve)(d);
    if (!Near(d.CMVelocity.Z, -0.5f) || !Near(d.AngularMomentum.Y, -0.5f) || !Near(d.AngularVelocity.Y, -0.5f)
        || normals.size() != 0 || indices.size() != 0) {
        std::printf("0075a7a0_TEST FAIL\n"); return 1;
    }
    normals.~vector();
    indices.~vector();
    std::printf("0075a7a0_TEST PASS\n");
    return 0;
}