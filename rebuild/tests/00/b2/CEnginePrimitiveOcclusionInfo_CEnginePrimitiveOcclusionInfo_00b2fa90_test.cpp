// Behaviour test for CEnginePrimitiveOcclusionInfo::CEnginePrimitiveOcclusionInfo (0x00B2FA90): test frame = the occlusion manager's current frame, visible, not seen.
#include <cstdio>
struct OcclusionManager { unsigned int CurrentFrame; };
static OcclusionManager g_manager;
struct Info { int InvisibleFrames; bool Seen; unsigned int TestFrame;
    Info() : TestFrame(g_manager.CurrentFrame) { InvisibleFrames = 0; Seen = false; } };
int main()
{
    g_manager.CurrentFrame = 99; Info a; g_manager.CurrentFrame = 100; Info b;
    if (a.TestFrame != 99 || b.TestFrame != 100 || a.InvisibleFrames != 0 || a.Seen) { std::printf("00b2fa90_TEST FAIL\n"); return 1; }
    std::printf("00b2fa90_TEST PASS\n");
    return 0;
}