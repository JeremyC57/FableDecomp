#include <cstdio>
// Behaviour model of CTCPhysicsBase::HasCollidedWithLandscapeAt (0x00880220)
struct C3DVector { float X, Y, Z; };
struct CPhysicsCollideResponse { C3DVector CollidePos, CollideNormal; float Friction; };
struct CWorldMap { float ground;
    float GetGroundSizeZAt(const C3DVector&) { return ground; }
    C3DVector GetGroundFaceNormalAt(const C3DVector&) { C3DVector n = { 0, 0, 1 }; return n; } };
static bool HasCollided(CWorldMap* map, const C3DVector& p, CPhysicsCollideResponse* r)
{
    float ground = map->GetGroundSizeZAt(p);
    if (ground > p.Z) {
        r->CollideNormal = map->GetGroundFaceNormalAt(p);
        r->CollidePos.X = p.X; r->CollidePos.Y = p.Y; r->CollidePos.Z = ground;
        return true;
    }
    return false;
}
int main()
{
    CWorldMap m = { 5.0f }; C3DVector p = { 1, 2, 3 }; CPhysicsCollideResponse r = { { 0, 0, 0 }, { 0, 0, 0 }, 0.5f };
    if (!HasCollided(&m, p, &r) || r.CollidePos.Z != 5 || r.CollidePos.X != 1 || r.CollideNormal.Z != 1 || r.Friction != 0.5f) { std::printf("00880220_TEST FAIL\n"); return 1; }
    m.ground = 3.0f;
    if (HasCollided(&m, p, &r)) { std::printf("00880220_TEST FAIL\n"); return 1; }
    std::printf("00880220_TEST PASS\n");
    return 0;
}
