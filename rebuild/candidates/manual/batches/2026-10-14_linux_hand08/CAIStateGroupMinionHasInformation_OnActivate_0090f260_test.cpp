#include <cstdio>
static long g_frame = 77;
long GFGetCurrentFrame() { return g_frame; }
struct CAIStateGroupBase { virtual ~CAIStateGroupBase() {} unsigned char _pad[0x10]; };
struct CAIStateGroup_MinionHasInformation : CAIStateGroupBase {
    long LastFrameWaved, LastFrameWandered, FrameInitialised, WavesWithoutCalling;
    virtual void OnActivate();
};
void CAIStateGroup_MinionHasInformation::OnActivate() { LastFrameWaved = 0; LastFrameWandered = 0; FrameInitialised = GFGetCurrentFrame(); WavesWithoutCalling = 0; }
int main()
{
    CAIStateGroup_MinionHasInformation g; g.LastFrameWaved = 5; g.LastFrameWandered = 6; g.FrameInitialised = 0; g.WavesWithoutCalling = 3;
    g.OnActivate();
    if (g.LastFrameWaved || g.LastFrameWandered || g.FrameInitialised != 77 || g.WavesWithoutCalling) { std::printf("0090f260_TEST FAIL\n"); return 1; }
    std::printf("0090f260_TEST PASS\n"); return 0;
}
