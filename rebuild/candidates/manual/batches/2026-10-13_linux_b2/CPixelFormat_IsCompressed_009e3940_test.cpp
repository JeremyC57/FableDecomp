#include <cstdio>

struct D { long ChannelType; unsigned char rest[0x20]; };
D g_PixelFormatDescriptors[8];
struct F { long FormatIndex; bool IsCompressed() { long t = g_PixelFormatDescriptors[FormatIndex].ChannelType; if (t < 4) return false; if (t <= 5) return true; if (t != 7) return false; return true; } };
int main()
{
    for (int i = 0; i < 8; ++i) g_PixelFormatDescriptors[i].ChannelType = i;
    F f; bool want[8] = { false, false, false, false, true, true, false, true };
    for (int k = 0; k < 8; ++k) { f.FormatIndex = k; if (f.IsCompressed() != want[k]) { std::printf("009e3940_TEST FAIL\n"); return 1; } }
    std::printf("009e3940_TEST PASS\n");
    return 0;
}
