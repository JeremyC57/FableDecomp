#include <cstdio>
// Behaviour model of CTCPhysicsBase::GetBoundingInfo (0x006AF640)
struct C3DVector { float X, Y, Z; };
struct CThingPhysicsMeshInfo { C3DVector ToCentre; float BoundingRadius; };
struct CTCPhysicsBase {
    const CThingPhysicsMeshInfo* mesh; C3DVector Position; float Radius;
    const CThingPhysicsMeshInfo* GetPhysicsMeshInfo() { return mesh; }
    void GetBoundingInfo(C3DVector& centre, float& radius) {
        const CThingPhysicsMeshInfo* s = GetPhysicsMeshInfo();
        if (s) { centre.X = s->ToCentre.X + Position.X; centre.Y = s->ToCentre.Y + Position.Y; centre.Z = s->ToCentre.Z + Position.Z; radius = s->BoundingRadius; return; }
        radius = Radius; centre = Position; centre.Z += radius;
    } };
int main()
{
    CThingPhysicsMeshInfo m = { { 1, 2, 3 }, 5 };
    CTCPhysicsBase p = { &m, { 10, 20, 30 }, 2 };
    C3DVector c; float r;
    p.GetBoundingInfo(c, r);
    if (c.X != 11 || c.Y != 22 || c.Z != 33 || r != 5) { std::printf("006af640_TEST FAIL\n"); return 1; }
    p.mesh = 0; p.GetBoundingInfo(c, r);
    if (c.X != 10 || c.Y != 20 || c.Z != 32 || r != 2) { std::printf("006af640_TEST FAIL\n"); return 1; }
    std::printf("006af640_TEST PASS\n");
    return 0;
}