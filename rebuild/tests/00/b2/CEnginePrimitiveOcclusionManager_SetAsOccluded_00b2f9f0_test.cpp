// Behaviour test for CEnginePrimitiveOcclusionManager::SetAsOccluded (0x00B2F9F0): invisible for delay+1 frames, pending handles cleared, test frame stamped.
#include <cstdio>
struct OcclusionManager { unsigned int CurrentFrame; };
static OcclusionManager g_manager;
struct Info { int InvisibleFrames; unsigned int handles[4]; int handleCount; unsigned int TestFrame; };
struct PrimitiveManager { int PrimitiveOccludeDelay;
    void SetAsOccluded(Info& info) const { info.InvisibleFrames = PrimitiveOccludeDelay + 1; info.handleCount = 0; info.TestFrame = g_manager.CurrentFrame; } };
int main()
{
    g_manager.CurrentFrame = 1234; PrimitiveManager m; m.PrimitiveOccludeDelay = 4;
    Info i; i.InvisibleFrames = 0; i.handleCount = 3; i.TestFrame = 0;
    m.SetAsOccluded(i);
    if (i.InvisibleFrames != 5 || i.handleCount != 0 || i.TestFrame != 1234) { std::printf("00b2f9f0_TEST FAIL\n"); return 1; }
    std::printf("00b2f9f0_TEST PASS\n");
    return 0;
}