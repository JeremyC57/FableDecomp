#include <cstdio>

struct CPixelFormatDescriptor { long ChannelType; unsigned char rest[0x20]; };
CPixelFormatDescriptor g_PixelFormatDescriptors[4];
class CPixelFormat { public: long FormatIndex; bool IsSigned() { return g_PixelFormatDescriptors[FormatIndex].ChannelType == 6; } };
int main()
{
    g_PixelFormatDescriptors[2].ChannelType = 6; g_PixelFormatDescriptors[3].ChannelType = 5;
    CPixelFormat f; f.FormatIndex = 2; if (!f.IsSigned()) { std::printf("009e3970_TEST FAIL\n"); return 1; }
    f.FormatIndex = 3; if (f.IsSigned()) { std::printf("009e3970_TEST FAIL\n"); return 1; }
    std::printf("009e3970_TEST PASS\n");
    return 0;
}