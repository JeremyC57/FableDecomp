// Behaviour test for 00c3d710: CAudioOutput::ApplySpeakerMode -- maps a speaker configuration to the global channel count and channel mask
#include <cstdio>

static long g_speakerChannels = -1, g_speakerMask = -1;

struct CAudioOutput {
    void ApplySpeakerMode(long mode)
    {
        switch (mode) {
        case 8:
            g_speakerChannels = 4;
            g_speakerMask = 0x33;
            break;
        case 0:
            g_speakerChannels = 0;
            g_speakerMask = 0;
            break;
        case 0x80:
            g_speakerChannels = 8;
            g_speakerMask = 0xff;
            break;
        default:
            g_speakerChannels = 2;
            g_speakerMask = 3;
            break;
        case 0x40:
        case 0x10000:
        case 0x20000:
            g_speakerChannels = 6;
            g_speakerMask = 0x3f;
            break;
        }
    }
};

int main()
{
    static const long modes[8] = { 8, 0, 0x80, 0x40, 0x10000, 0x20000, 2, 0x99 };
    static const long chans[8] = { 4, 0, 8, 6, 6, 6, 2, 2 };
    static const long masks[8] = { 0x33, 0, 0xff, 0x3f, 0x3f, 0x3f, 3, 3 };
    CAudioOutput out;
    for (int i = 0; i < 8; ++i) {
        out.ApplySpeakerMode(modes[i]);
        if (g_speakerChannels != chans[i] || g_speakerMask != masks[i]) { std::printf("00c3d710_TEST FAIL\n"); return 1; }
    }
    std::printf("00c3d710_TEST PASS\n");
    return 0;
}