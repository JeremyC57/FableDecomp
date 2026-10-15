#include <cstdio>
struct CTCPhysicsStandard {
    bool AlwaysFacingMovementDirection : 1;
    bool AngledWithGround : 1;
    bool FeelsGravity : 1;
    bool FeelsAirResistance : 1;
    bool OnGround : 1;
    bool OnMesh : 1;
    bool InteractsWithThings : 1;
    bool CollidesWithThings : 1;
    bool vfunc_53() { return OnGround | OnMesh; }
};
int main()
{
    CTCPhysicsStandard p;
    p.AlwaysFacingMovementDirection = true; p.AngledWithGround = true; p.FeelsGravity = true; p.FeelsAirResistance = true;
    p.InteractsWithThings = true; p.CollidesWithThings = true;
    int fails = 0;
    p.OnGround = false; p.OnMesh = false; if (p.vfunc_53()) ++fails;
    p.OnGround = true;  p.OnMesh = false; if (!p.vfunc_53()) ++fails;
    p.OnGround = false; p.OnMesh = true;  if (!p.vfunc_53()) ++fails;
    p.OnGround = true;  p.OnMesh = true;  if (!p.vfunc_53()) ++fails;
    if (fails) { std::printf("00724c50_TEST FAIL\n"); return 1; }
    std::printf("00724c50_TEST PASS\n");
    return 0;
}