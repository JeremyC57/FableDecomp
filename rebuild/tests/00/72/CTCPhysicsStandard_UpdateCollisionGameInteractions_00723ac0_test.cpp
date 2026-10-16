#include <cstdio>
// Behaviour model of CTCPhysicsStandard::UpdateCollisionGameInteractions (0x00723AC0)
struct C3DVector { float X, Y, Z; };
struct CPhysicsCollideResponse { C3DVector CollidePos, CollideNormal; };
struct CThing;
struct CTCPhysicsBase { bool interacts; CThing* ParentThing; bool GetInteractsWithThings() { return interacts; } };
struct CThing { CTCPhysicsBase* PPhysics; unsigned char Flags91; int hits; CThing* lastOther; const C3DVector* lastNormal;
    void OnCollision(long, const C3DVector*, const C3DVector* n, CThing* o) { ++hits; lastOther = o; lastNormal = n; } };
static void Update(CTCPhysicsBase* self, CThing* other, CPhysicsCollideResponse* r)
{
    if (self->GetInteractsWithThings()) self->ParentThing->OnCollision(1, &r->CollidePos, &r->CollideNormal, other);
    if (other->PPhysics->GetInteractsWithThings() && ((unsigned char)~other->Flags91 & 1))
        other->OnCollision(1, &r->CollidePos, &r->CollideNormal, self->ParentThing);
}
int main()
{
    CTCPhysicsBase pa = { true, 0 }, pb = { true, 0 };
    CThing a = { &pa, 0, 0, 0, 0 }, b = { &pb, 0, 0, 0, 0 }; pa.ParentThing = &a; pb.ParentThing = &b;
    CPhysicsCollideResponse r;
    Update(&pa, &b, &r);
    if (a.hits != 1 || a.lastOther != &b || b.hits != 1 || b.lastOther != &a || a.lastNormal != &r.CollideNormal) { std::printf("00723ac0_TEST FAIL\n"); return 1; }
    b.Flags91 = 1; pa.interacts = false; Update(&pa, &b, &r);
    if (a.hits != 1 || b.hits != 1) { std::printf("00723ac0_TEST FAIL\n"); return 1; }
    std::printf("00723ac0_TEST PASS\n");
    return 0;
}