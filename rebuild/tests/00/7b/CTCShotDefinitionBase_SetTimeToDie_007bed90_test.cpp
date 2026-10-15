#include <cstdio>
// Behaviour of CTCShotDefinitionBase::SetTimeToDie (0x007BED90): DieOnFrame = now + seconds * fps, the rate read before the frame query.
long g_FramesPerSecond = 15;
static long g_frame = 1000;
long GetWorldFrame() { g_FramesPerSecond = 30; return g_frame; }   // changing the rate proves it was read first
struct CTCShotDefinitionBase_Retail {
    long DieOnFrame;
    void SetTimeToDie(float seconds)
    {
        long framesPerSecond = g_FramesPerSecond;
        DieOnFrame = (long)(framesPerSecond * seconds + GetWorldFrame());
    }
};
int main()
{
    CTCShotDefinitionBase_Retail s;
    s.DieOnFrame = 0;
    s.SetTimeToDie(2.0f);
    if (s.DieOnFrame != 1030) { std::printf("007bed90_TEST FAIL\n"); return 1; }
    std::printf("007bed90_TEST PASS\n");
    return 0;
}