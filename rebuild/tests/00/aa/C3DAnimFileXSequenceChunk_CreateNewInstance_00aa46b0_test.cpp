// behaviour test for 0x00AA46B0 C3DAnimFileXSequenceChunk::CreateNewInstance() (model; InitNewInstance records the new chunk)
#include <cstdio>
#include <cstring>
static const void* g_init = 0;
struct CChunk {
    char ChunkType[5]; void* start; void* finish; void* end;
    CChunk(const char* t) : start(0), finish(0), end(0) { std::strncpy(ChunkType, t, 4); ChunkType[4] = 0; }
    CChunk* InitNewInstance(CChunk* c) const { g_init = c; return c; }
    CChunk* CreateNewInstance() const { return InitNewInstance(new CChunk("XSEQ")); }
};
int main()
{
    CChunk proto("XSEQ");
    CChunk* c = proto.CreateNewInstance();
    int ok = c != 0 && c == g_init && std::strcmp(c->ChunkType, "XSEQ") == 0 && c->start == 0 && c != &proto;
    delete c;
    if (!ok) { std::printf("00aa46b0_TEST FAIL\n"); return 1; }
    std::printf("00aa46b0_TEST PASS\n");
    return 0;
}