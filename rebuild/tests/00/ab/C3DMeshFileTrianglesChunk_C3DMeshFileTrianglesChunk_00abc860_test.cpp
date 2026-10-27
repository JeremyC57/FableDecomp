// behaviour test for 0x00ABC860 C3DMeshFileTrianglesChunk::C3DMeshFileTrianglesChunk() (model: the base ctor copies the type; the array starts empty)
#include <cstdio>
#include <cstring>
#include <new>
struct CBaseChunk {
    char ChunkType[5]; void* Parent;
    CBaseChunk(const char* t) : Parent(0) { std::strncpy(ChunkType, t, 4); ChunkType[4] = 0; }
};
struct CArr { void* s; void* f; void* e; CArr() : s(0), f(0), e(0) { } };
struct CChunkX : CBaseChunk { CChunkX() : CBaseChunk("TRIS") { } CArr TriangleList; };
int main()
{
    static double raw[8];
    std::memset(raw, 0xCD, sizeof(raw));
    CChunkX* c = new (raw) CChunkX;
    int ok = std::strcmp(c->ChunkType, "TRIS") == 0 && c->TriangleList.s == 0 && c->TriangleList.f == 0 && c->TriangleList.e == 0;
    if (!ok) { std::printf("00abc860_TEST FAIL\n"); return 1; }
    std::printf("00abc860_TEST PASS\n");
    return 0;
}