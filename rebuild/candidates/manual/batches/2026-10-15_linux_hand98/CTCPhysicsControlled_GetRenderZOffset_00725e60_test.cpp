#include <cstdio>
struct CTCPhysicsControlled {
    unsigned char _base[0x88];
    float ImpulseVelocity[3];
    float ForcedMoveToPos[3];
    unsigned char Flags;
    float RenderZOffset;
    float LastRenderZOffset;
    float GetRenderZOffset(float t) const { return (RenderZOffset - LastRenderZOffset) * t + LastRenderZOffset; }
};
int main()
{
    CTCPhysicsControlled c;
    c.RenderZOffset = 3.0f; c.LastRenderZOffset = 1.0f;
    if (c.GetRenderZOffset(0.0f) != 1.0f || c.GetRenderZOffset(1.0f) != 3.0f || c.GetRenderZOffset(0.5f) != 2.0f) { std::printf("00725e60_TEST FAIL\n"); return 1; }
    std::printf("00725e60_TEST PASS\n");
    return 0;
}
