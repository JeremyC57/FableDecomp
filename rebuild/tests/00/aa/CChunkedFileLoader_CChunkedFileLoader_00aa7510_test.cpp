#include <cstdio>
#include <cstring>
class CChunkedFileLoader { public:
    virtual void VSlot0() { }
    long ReadUnknownChunkTypes; char SupportedFileType[5];
    CChunkedFileLoader(const char* type) { std::memset(SupportedFileType, 0, sizeof(SupportedFileType)); std::memcpy(SupportedFileType, type, 4); ReadUnknownChunkTypes = 0; }
};
int main()
{
    CChunkedFileLoader l("3DAFxyz");
    if (l.ReadUnknownChunkTypes != 0 || std::memcmp(l.SupportedFileType, "3DAF\0", 5) != 0) { std::printf("00aa7510_TEST FAIL\n"); return 1; }
    std::printf("00aa7510_TEST PASS\n"); return 0;
}