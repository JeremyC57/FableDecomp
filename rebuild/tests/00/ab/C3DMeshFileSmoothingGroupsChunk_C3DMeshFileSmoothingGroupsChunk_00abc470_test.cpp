// behaviour test for 0x00ABC470 C3DMeshFileSmoothingGroupsChunk::C3DMeshFileSmoothingGroupsChunk() (model: the base ctor copies the type; the array starts empty)
#include <cstdio>
#include <cstring>
#include <new>
struct CBaseChunk {
    char ChunkType[5]; void* Parent;
    CBaseChunk(const char* t) : Parent(0) { std::strncpy(ChunkType, t, 4); ChunkType[4] = 0; }
};
struct CArr { void* s; void* f; void* e; CArr() : s(0), f(0), e(0) { } };
struct CChunkX : CBaseChunk { CChunkX() : CBaseChunk("SMTH") { } CArr FaceGroupsList; };
int main()
{
    static double raw[8];
    std::memset(raw, 0xCD, sizeof(raw));
    CChunkX* c = new (raw) CChunkX;
    int ok = std::strcmp(c->ChunkType, "SMTH") == 0 && c->FaceGroupsList.s == 0 && c->FaceGroupsList.f == 0 && c->FaceGroupsList.e == 0;
    if (!ok) { std::printf("00abc470_TEST FAIL\n"); return 1; }
    std::printf("00abc470_TEST PASS\n");
    return 0;
}