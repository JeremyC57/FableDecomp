#include <cstdio>
#include <cstring>
static int g_baseCtor = 0;
struct CBase { virtual void VSlot0() { } CBase() { ++g_baseCtor; } };
class CChunkedFileSuperChunk;
struct CChunkedFileChunk_Retail : CBase {
    char ChunkType[5]; CChunkedFileSuperChunk* Parent;
    CChunkedFileChunk_Retail(const char* type) : Parent(0) { std::memset(ChunkType, 0, sizeof(ChunkType)); std::memcpy(ChunkType, type, 4); }
};
struct CChunkedFileRawDataChunk_Retail : CChunkedFileChunk_Retail {
    void* Data; unsigned long ByteSize;
    CChunkedFileRawDataChunk_Retail(const char* type) : CChunkedFileChunk_Retail(type), Data(0), ByteSize(0) { }
    virtual CChunkedFileChunk_Retail* CreateNewInstance() { return new CChunkedFileRawDataChunk_Retail(ChunkType); }
};
int main()
{
    CChunkedFileRawDataChunk_Retail c("RAWDxx");
    if (g_baseCtor != 1 || c.Parent != 0 || c.Data != 0 || c.ByteSize != 0 || std::memcmp(c.ChunkType, "RAWD\0", 5) != 0) { std::printf("00aa76d0_TEST FAIL\n"); return 1; }
    std::printf("00aa76d0_TEST PASS\n"); return 0;
}