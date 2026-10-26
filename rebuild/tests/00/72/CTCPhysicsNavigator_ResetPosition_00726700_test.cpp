#include <cstdio>
class C3DVector { public: float X, Y, Z; };
static int g_baseCalls = 0;
static C3DVector g_basePos;
class CTCPhysicsBase { public:
    void ResetPosition(const C3DVector& pos) { ++g_baseCalls; g_basePos = pos; }
};
struct CTCPhysicsNavigator : CTCPhysicsBase {
    C3DVector PositionToNavigateTo;
    C3DVector AdditionalImpulse;
    C3DVector ForcedMoveToPos;
    bool ForcedMoveToPosSet : 1;
    bool ForcedMoveToPosUsePhysics : 1;
    void ResetPosition(const C3DVector& pos)
    {
        CTCPhysicsBase::ResetPosition(pos);
        PositionToNavigateTo = pos;
        ForcedMoveToPos = pos;
        ForcedMoveToPosSet = false;
    }
};
int main()
{
    CTCPhysicsNavigator n; n.ForcedMoveToPosSet = true; n.ForcedMoveToPosUsePhysics = true;
    n.AdditionalImpulse.X = 9.0f;
    C3DVector p; p.X = 1.0f; p.Y = 2.0f; p.Z = 3.0f;
    n.ResetPosition(p);
    if (g_baseCalls != 1 || g_basePos.Z != 3.0f || n.PositionToNavigateTo.X != 1.0f || n.ForcedMoveToPos.Y != 2.0f || n.ForcedMoveToPosSet || !n.ForcedMoveToPosUsePhysics || n.AdditionalImpulse.X != 9.0f) { std::printf("00726700_TEST FAIL\n"); return 1; }
    std::printf("00726700_TEST PASS\n");
    return 0;
}