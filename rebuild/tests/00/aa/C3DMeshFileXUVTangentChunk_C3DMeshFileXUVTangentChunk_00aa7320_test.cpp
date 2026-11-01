// Behaviour test for 0x00AA7320 C3DMeshFileXUVTangentChunk::~C3DMeshFileXUVTangentChunk (implicit): VTangents' block, then
// UTangents' block, then ~CChunkedFileChunk.
#include <cstdio>
#include <cstdlib>
static const void* g_log[8]; static int g_n = 0; static int g_baseAt = -1;
static void Free(void* p) { if (p) { g_log[g_n++] = p; std::free(p); } }
class C3DVector { public: float X, Y, Z; };
struct CArray { C3DVector* start; C3DVector* finish; C3DVector* eos; CArray() : start(0), finish(0), eos(0) { } ~CArray() { if (start != 0) Free(start); } };
class CChunkedFileChunk { public: virtual ~CChunkedFileChunk() { g_baseAt = g_n; } char ChunkType[5]; void* Parent; };
class C3DMeshFileXUVTangentChunk : public CChunkedFileChunk { public: CArray UTangents; CArray VTangents; };
int main()
{
    C3DMeshFileXUVTangentChunk* c = new C3DMeshFileXUVTangentChunk;
    c->UTangents.start = (C3DVector*)std::malloc(sizeof(C3DVector));
    c->VTangents.start = (C3DVector*)std::malloc(sizeof(C3DVector));
    const void* u = c->UTangents.start; const void* v = c->VTangents.start;
    CChunkedFileChunk* b = c;
    delete b;
    if (g_n != 2 || g_log[0] != v || g_log[1] != u || g_baseAt != 2) { std::printf("00aa7320_TEST FAIL\n"); return 1; }
    std::printf("00aa7320_TEST PASS\n");
    return 0;
}