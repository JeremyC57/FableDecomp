#include <cstdio>
struct CThing;
static CThing* g_baseArg = 0;
struct C3DVector { float X, Y, Z; C3DVector() { } C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { } };
struct CTCPhysicsStandard {
    virtual ~CTCPhysicsStandard() { }
    unsigned char _base[0x84];
    CTCPhysicsStandard(CThing* thing) { g_baseArg = thing; }
};
struct CTCPhysicsNavigator : CTCPhysicsStandard {
    bool      Navigating;
    C3DVector PositionToNavigateTo;
    C3DVector AdditionalImpulse;
    C3DVector ForcedMoveToPos;
    bool      ForcedMoveToPosSet : 1;
    bool      ForcedMoveToPosUsePhysics : 1;
    CTCPhysicsNavigator(CThing* thing) : CTCPhysicsStandard(thing), Navigating(false), AdditionalImpulse(0.0f, 0.0f, 0.0f)
    {
        ForcedMoveToPosSet = false;
        ForcedMoveToPosUsePhysics = false;
    }
};
int main()
{
    CThing* t = (CThing*)0x1234;
    CTCPhysicsNavigator n(t);
    if (g_baseArg != t || n.Navigating || n.AdditionalImpulse.X != 0.0f || n.AdditionalImpulse.Y != 0.0f || n.AdditionalImpulse.Z != 0.0f || n.ForcedMoveToPosSet || n.ForcedMoveToPosUsePhysics) { std::printf("007266c0_TEST FAIL\n"); return 1; }
    std::printf("007266c0_TEST PASS\n");
    return 0;
}