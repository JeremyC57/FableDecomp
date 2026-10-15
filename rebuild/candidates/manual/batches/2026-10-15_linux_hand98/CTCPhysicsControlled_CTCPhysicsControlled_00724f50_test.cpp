#include <cstdio>
struct CThing;
static CThing* g_baseArg = 0;
struct C3DVector { float X, Y, Z; C3DVector() { } };
struct CTCPhysicsStandard {
    virtual ~CTCPhysicsStandard() { }
    unsigned char _base[0x84];
    CTCPhysicsStandard(CThing* thing) { g_baseArg = thing; }
};
struct CTCPhysicsControlled : CTCPhysicsStandard {
    C3DVector ImpulseVelocity;
    C3DVector ForcedMoveToPos;
    bool      ForcedMoveToPosSet : 1;
    bool      ForcedMoveToPosUsePhysics : 1;
    float     RenderZOffset;
    float     LastRenderZOffset;
    CTCPhysicsControlled(CThing* thing) : CTCPhysicsStandard(thing)
    {
        ForcedMoveToPosSet = false;
        ForcedMoveToPosUsePhysics = false;
        RenderZOffset = 0.0f;
        LastRenderZOffset = 0.0f;
    }
};
int main()
{
    CThing* t = (CThing*)0x5678;
    CTCPhysicsControlled c(t);
    if (g_baseArg != t || c.ForcedMoveToPosSet || c.ForcedMoveToPosUsePhysics || c.RenderZOffset != 0.0f || c.LastRenderZOffset != 0.0f) { std::printf("00724f50_TEST FAIL\n"); return 1; }
    std::printf("00724f50_TEST PASS\n");
    return 0;
}
