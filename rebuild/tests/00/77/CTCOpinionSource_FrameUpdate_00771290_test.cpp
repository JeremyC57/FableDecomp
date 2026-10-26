// Behaviour test for CTCOpinionSource::FrameUpdate @ 0x00771290: the deed clears at or after its frame only.
#include <cstdio>
static long g_frame = 0;
class CWorld { public: static long GetWorldFrame() { return g_frame; } };
class CTCOpinionSource { public:
    long VirtualDeed, VirtualDeedID;
    unsigned long FrameToClearVirtualDeed;
    void FrameUpdate()
    {
        if (VirtualDeed != 0 && (unsigned long)CWorld::GetWorldFrame() >= FrameToClearVirtualDeed)
            VirtualDeed = 0;
    }
};
int main()
{
    CTCOpinionSource s; s.VirtualDeed = 3; s.VirtualDeedID = 9; s.FrameToClearVirtualDeed = 100;
    g_frame = 99; s.FrameUpdate();
    if (s.VirtualDeed != 3) { std::printf("00771290_TEST FAIL\n"); return 1; }
    g_frame = 100; s.FrameUpdate();
    if (s.VirtualDeed != 0 || s.VirtualDeedID != 9) { std::printf("00771290_TEST FAIL\n"); return 1; }
    std::printf("00771290_TEST PASS\n");
    return 0;
}