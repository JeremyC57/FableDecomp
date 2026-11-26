// Behaviour test for 0x009143C0 CAIStateGroup_BirdGroundWander::Cleanup(): stores the world's current frame in FrameLastFinishedWalking.
#include <cstdio>
static long g_frame = 0;
struct CWorldModel { static long GetFrame() { return g_frame; } };
struct CBirdModel {
    CWorldModel world; unsigned long FrameLastFinishedWalking;
    const CWorldModel& PeekWorld() const { return world; }
    void Cleanup() { FrameLastFinishedWalking = PeekWorld().GetFrame(); }
};
int main()
{
    CBirdModel b; b.FrameLastFinishedWalking = 7;
    g_frame = 88; b.Cleanup(); if (b.FrameLastFinishedWalking != 88) { std::printf("009143c0_TEST FAIL\n"); return 1; }
    g_frame = 1234; b.Cleanup(); if (b.FrameLastFinishedWalking != 1234) { std::printf("009143c0_TEST FAIL\n"); return 1; }
    std::printf("009143c0_TEST PASS\n"); return 0;
}
