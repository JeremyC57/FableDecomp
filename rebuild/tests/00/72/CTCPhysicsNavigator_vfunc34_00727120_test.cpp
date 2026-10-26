#include <cstdio>
class C3DVector { public: float X, Y, Z; };
struct CTCPhysicsNavigator {
    bool      Navigating;
    C3DVector PositionToNavigateTo;
    C3DVector AdditionalImpulse;
    C3DVector ForcedMoveToPos;
    bool ForcedMoveToPosSet : 1;
    bool ForcedMoveToPosUsePhysics : 1;
    bool _rest : 6;
    void vfunc_34(const C3DVector& pos, bool usePhysics)
    {
        ForcedMoveToPos = pos;
        ForcedMoveToPosUsePhysics = usePhysics;
        ForcedMoveToPosSet = true;
    }
};
int main()
{
    CTCPhysicsNavigator n;
    n.ForcedMoveToPosSet = false; n.ForcedMoveToPosUsePhysics = true; n._rest = 0;
    n.AdditionalImpulse.X = 4.0f;
    C3DVector p; p.X = 1.0f; p.Y = 2.0f; p.Z = 3.0f;
    n.vfunc_34(p, false);
    if (!n.ForcedMoveToPosSet || n.ForcedMoveToPosUsePhysics || n.ForcedMoveToPos.X != 1.0f || n.ForcedMoveToPos.Y != 2.0f ||
        n.ForcedMoveToPos.Z != 3.0f || n.AdditionalImpulse.X != 4.0f || n._rest != 0) { std::printf("00727120_TEST FAIL\n"); return 1; }
    n.vfunc_34(p, true);
    if (!n.ForcedMoveToPosSet || !n.ForcedMoveToPosUsePhysics) { std::printf("00727120_TEST FAIL\n"); return 1; }
    std::printf("00727120_TEST PASS\n");
    return 0;
}