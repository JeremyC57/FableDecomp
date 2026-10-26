#include <cstdio>
// Behaviour model of CTCPhysicsRigidBody::ConstructFromParams (0x00759900): base position copy, then RHSet / OldRHSet from the params
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
struct CTCParamsPhysics { C3DVector Position; CRightHandedSet RHSet; };
class CTCPhysicsBase { public: C3DVector Position, OldPosition;
    void ConstructFromParams(const CTCParamsPhysics* p) { OldPosition = p->Position; Position = p->Position; } };
struct CTCPhysicsRigidBody : CTCPhysicsBase { CRightHandedSet RHSet, OldRHSet;
    void ConstructFromParams(const CTCParamsPhysics* p) { CTCPhysicsBase::ConstructFromParams(p); RHSet = p->RHSet; OldRHSet = p->RHSet; } };
int main()
{
    CTCParamsPhysics prm = { { 1, 2, 3 }, { { 0, 0, 1 }, { 1, 0, 0 } } };
    CTCPhysicsRigidBody s;
    s.ConstructFromParams(&prm);
    if (s.Position.Z != 3 || s.OldPosition.X != 1 || s.RHSet.Up.Z != 1 || s.OldRHSet.Forward.X != 1) { std::printf("00759900_TEST FAIL\n"); return 1; }
    std::printf("00759900_TEST PASS\n");
    return 0;
}