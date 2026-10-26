#include <cstdio>
class C3DVector { public: float X, Y, Z; };
struct CTCPhysicsControlled {
    C3DVector ImpulseVelocity;
    C3DVector ForcedMoveToPos;
    unsigned char ForcedMoveToPosSet : 1;
    unsigned char ForcedMoveToPosUsePhysics : 1;
    unsigned char Other : 6;
    void SetForcedMoveToPos(const C3DVector& pos, bool usePhysics)
    {
        ForcedMoveToPos = pos;
        ForcedMoveToPosUsePhysics = usePhysics;
        ForcedMoveToPosSet = true;
    }
};
int main()
{
    CTCPhysicsControlled c;
    c.ForcedMoveToPosSet = 0; c.ForcedMoveToPosUsePhysics = 1; c.Other = 0x2a;
    C3DVector p; p.X = 1.0f; p.Y = 2.0f; p.Z = 3.0f;
    c.SetForcedMoveToPos(p, false);
    if (c.ForcedMoveToPos.X != 1.0f || c.ForcedMoveToPos.Z != 3.0f || !c.ForcedMoveToPosSet || c.ForcedMoveToPosUsePhysics || c.Other != 0x2a) { std::printf("00725000_TEST FAIL\n"); return 1; }
    c.SetForcedMoveToPos(p, true);
    if (!c.ForcedMoveToPosSet || !c.ForcedMoveToPosUsePhysics || c.Other != 0x2a) { std::printf("00725000_TEST FAIL\n"); return 1; }
    std::printf("00725000_TEST PASS\n");
    return 0;
}