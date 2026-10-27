// behaviour test for 0x00ABC560 C3DMeshFileUniqueVerticesChunk::C3DMeshFileUniqueVerticesChunk() (model: the base ctor copies the type; the array starts empty)
#include <cstdio>
#include <cstring>
#include <new>
struct CBaseChunk {
    char ChunkType[5]; void* Parent;
    CBaseChunk(const char* t) : Parent(0) { std::strncpy(ChunkType, t, 4); ChunkType[4] = 0; }
};
struct CArr { void* s; void* f; void* e; CArr() : s(0), f(0), e(0) { } };
struct CChunkX : CBaseChunk { CChunkX() : CBaseChunk("UNIV") { } CArr UniqueVerticesList; };
int main()
{
    static double raw[8];
    std::memset(raw, 0xCD, sizeof(raw));
    CChunkX* c = new (raw) CChunkX;
    int ok = std::strcmp(c->ChunkType, "UNIV") == 0 && c->UniqueVerticesList.s == 0 && c->UniqueVerticesList.f == 0 && c->UniqueVerticesList.e == 0;
    if (!ok) { std::printf("00abc560_TEST FAIL\n"); return 1; }
    std::printf("00abc560_TEST PASS\n");
    return 0;
}