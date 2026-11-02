// Behaviour test for 0x00A9D330 CMirroredTextureHighlightsChunk::~CMirroredTextureHighlightsChunk (user destructor, empty body):
// deleting through the base frees the TriangleVertexIndexList block (when allocated) and then runs the base destructor.
#include <cstdio>
#include <cstdlib>
static int g_frees = 0;
static int g_order[4];
static int g_n = 0;
static void Note(int e) { if (g_n < 4) g_order[g_n++] = e; }
static void FreeBlock(void* p) { if (p != 0) { ++g_frees; std::free(p); } }
struct IndexArray {
    unsigned int* Start; unsigned int* Finish; unsigned int* EndOfStorage;
    IndexArray() : Start(0), Finish(0), EndOfStorage(0) { }
    ~IndexArray() { if (Start != 0) Note(1); FreeBlock(Start); }
};
struct ChunkedFileChunk { virtual ~ChunkedFileChunk() { Note(2); } char ChunkType[5]; void* Parent; };
struct MirroredTextureHighlightsChunk : ChunkedFileChunk {
    virtual ~MirroredTextureHighlightsChunk() { }
    IndexArray TriangleVertexIndexList;
};
int main()
{
    MirroredTextureHighlightsChunk* chunk = new MirroredTextureHighlightsChunk;
    chunk->TriangleVertexIndexList.Start = (unsigned int*)std::malloc(6 * sizeof(unsigned int));
    ChunkedFileChunk* base = chunk;
    delete base;
    if (g_frees != 1 || g_n != 2 || g_order[0] != 1 || g_order[1] != 2) { std::printf("00a9d330_TEST FAIL\n"); return 1; }
    g_n = 0; g_frees = 0;
    base = new MirroredTextureHighlightsChunk;
    delete base;
    if (g_frees != 0 || g_n != 1 || g_order[0] != 2) { std::printf("00a9d330_TEST FAIL\n"); return 1; }
    std::printf("00a9d330_TEST PASS\n");
    return 0;
}