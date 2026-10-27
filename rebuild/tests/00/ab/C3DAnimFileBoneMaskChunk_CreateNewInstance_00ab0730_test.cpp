// behaviour test for 0x00AB0730 C3DAnimFileBoneMaskChunk::CreateNewInstance() (model; InitNewInstance records the new chunk)
#include <cstdio>
#include <cstring>
static const void* g_init = 0;
struct CChunk {
    char ChunkType[5]; void* start; void* finish; void* end;
    CChunk(const char* t) : start(0), finish(0), end(0) { std::strncpy(ChunkType, t, 4); ChunkType[4] = 0; }
    CChunk* InitNewInstance(CChunk* c) const { g_init = c; return c; }
    CChunk* CreateNewInstance() const { return InitNewInstance(new CChunk("AMSK")); }
};
int main()
{
    CChunk proto("AMSK");
    CChunk* c = proto.CreateNewInstance();
    int ok = c != 0 && c == g_init && std::strcmp(c->ChunkType, "AMSK") == 0 && c->start == 0 && c != &proto;
    delete c;
    if (!ok) { std::printf("00ab0730_TEST FAIL\n"); return 1; }
    std::printf("00ab0730_TEST PASS\n");
    return 0;
}