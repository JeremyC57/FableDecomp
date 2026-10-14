#include <cstdio>
#include <cstring>
static int g_baseCtor = 0;
struct CBase { virtual void VSlot0() { } CBase() { ++g_baseCtor; } };
struct CChunkedFileSuperChunk;
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
    CChunkedFileRawDataChunk_Retail proto("MESH"); proto.Data = &proto; proto.ByteSize = 77; proto.Parent = (CChunkedFileSuperChunk*)&proto;
    CChunkedFileChunk_Retail* made = proto.CreateNewInstance();
    CChunkedFileRawDataChunk_Retail* raw = static_cast<CChunkedFileRawDataChunk_Retail*>(made);
    if (made == 0 || made == &proto || g_baseCtor != 2 || made->Parent != 0 || raw->Data != 0 || raw->ByteSize != 0 || std::memcmp(made->ChunkType, "MESH\0", 5) != 0) { std::printf("00aa7850_TEST FAIL\n"); return 1; }
    delete raw;
    std::printf("00aa7850_TEST PASS\n"); return 0;
}