#include <cstdio>
#include <cstring>
static int g_baseCtor = 0;
struct CBase { virtual void VSlot0() { } CBase() { ++g_baseCtor; } };
class CChunkedFileSuperChunk;
struct CChunkedFileChunk_Retail : CBase {
    char ChunkType[5]; CChunkedFileSuperChunk* Parent;
    CChunkedFileChunk_Retail(const char* type) : Parent(0) { std::memset(ChunkType, 0, sizeof(ChunkType)); std::memcpy(ChunkType, type, 4); }
};
int main()
{
    CChunkedFileChunk_Retail c("ANIMX");
    if (g_baseCtor != 1 || c.Parent != 0 || std::memcmp(c.ChunkType, "ANIM\0", 5) != 0 || std::strlen(c.ChunkType) != 4) { std::printf("00aa7590_TEST FAIL\n"); return 1; }
    std::printf("00aa7590_TEST PASS\n"); return 0;
}