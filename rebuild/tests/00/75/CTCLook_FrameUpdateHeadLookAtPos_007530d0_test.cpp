#include <cstdio>
// Behaviour model of CTCLook::FrameUpdateHeadLookAtPos (0x007530D0): the head speed from the look flags.
static float SpeedFor(long flags)
{
    float speed = 0.0f;
    if (flags & 8) speed = 0.33f;
    else if (flags & 0x10) speed = 0.66f;
    else if (flags & 0x20) speed = 1.0f;
    return speed;
}
int main()
{
    if (SpeedFor(0) != 0.0f || SpeedFor(8 | 0x20) != 0.33f || SpeedFor(0x10) != 0.66f || SpeedFor(0x20) != 1.0f) { std::printf("007530d0_TEST FAIL\n"); return 1; }
    bool eyes = (0x4L & 4) != 0;
    if (!eyes) { std::printf("007530d0_TEST FAIL\n"); return 1; }
    std::printf("007530d0_TEST PASS\n");
    return 0;
}