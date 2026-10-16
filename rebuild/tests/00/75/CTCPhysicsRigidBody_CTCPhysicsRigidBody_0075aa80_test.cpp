#include <cstdio>
// Behaviour model of CTCPhysicsRigidBody::CTCPhysicsRigidBody (0x0075AA80)
struct V { void* a; void* b; void* c; V() : a(0), b(0), c(0) {} bool empty() const { return !a && !b && !c; } };
struct CRigidBodyConfig { float pad[42]; V BoundingVertices; };
struct CRigidBody { float pad[17]; V BodyBoundingVertices; CRigidBodyConfig CurrentConfig, LastConfig; };
struct CTCPhysicsBase { void* ParentThing; CTCPhysicsBase(void* p) : ParentThing(p) {} };
struct CTCPhysicsRigidBody : CTCPhysicsBase { V CollisionNormal, CollidingVertexIndex, Collidors; void* PPhysicsMeshInfo; CRigidBody Body;
    CTCPhysicsRigidBody(void* p) : CTCPhysicsBase(p) { PPhysicsMeshInfo = 0; } };
int main()
{
    int t;
    CTCPhysicsRigidBody* r = new CTCPhysicsRigidBody(&t);
    if (r->ParentThing != &t || r->PPhysicsMeshInfo || !r->CollisionNormal.empty() || !r->CollidingVertexIndex.empty() || !r->Collidors.empty()
        || !r->Body.BodyBoundingVertices.empty() || !r->Body.CurrentConfig.BoundingVertices.empty() || !r->Body.LastConfig.BoundingVertices.empty())
    { std::printf("0075aa80_TEST FAIL\n"); return 1; }
    delete r;
    std::printf("0075aa80_TEST PASS\n");
    return 0;
}