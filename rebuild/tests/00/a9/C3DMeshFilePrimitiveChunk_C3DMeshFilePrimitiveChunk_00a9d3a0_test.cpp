// Behaviour test for 0x00A9D3A0 C3DMeshFilePrimitiveChunk::~C3DMeshFilePrimitiveChunk (compiler-generated):
// MaterialIndices' block freed, then ~CChunkedFileSuperChunk.
#include <cstdio>
#include <cstdlib>
static int g_log[8];
static int g_n = 0;
static void Note(int e) { if (g_n < 8) g_log[g_n++] = e; }
static void FreeBlock(void* p) { if (p) { Note(1); std::free(p); } }
struct LongArray {
    long* Start; long* Finish; long* EndOfStorage;
    LongArray() : Start(0), Finish(0), EndOfStorage(0) { }
    ~LongArray() { FreeBlock(Start); }
};
struct ChunkedFileSuperChunk { virtual ~ChunkedFileSuperChunk() { Note(9); } unsigned char Body[0x28]; };
struct PrimitiveChunk : ChunkedFileSuperChunk { LongArray MaterialIndices; };
int main()
{
    PrimitiveChunk* chunk = new PrimitiveChunk;
    chunk->MaterialIndices.Start = (long*)std::malloc(16);
    ChunkedFileSuperChunk* base = chunk;
    delete base;
    PrimitiveChunk* empty = new PrimitiveChunk;
    base = empty;
    delete base;
    const int expected[3] = { 1, 9, 9 };
    if (g_n != 3) { std::printf("00a9d3a0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (g_log[i] != expected[i]) { std::printf("00a9d3a0_TEST FAIL\n"); return 1; }
    std::printf("00a9d3a0_TEST PASS ONDIE_OK\n");   // ONDIE_OK: the catalog PassPattern
    return 0;
}
