// Behaviour test for 0x00A9D3C0 C3DMeshFileClothChunk::~C3DMeshFileClothChunk (compiler-generated):
// CompiledCode's block freed, ParticleCode then Name destroyed, then ~CChunkedFileSuperChunk.
#include <cstdio>
#include <cstdlib>
static int g_log[8];
static int g_n = 0;
static void Note(int e) { if (g_n < 8) g_log[g_n++] = e; }
static void FreeBlock(void* p) { if (p) { Note(1); std::free(p); } }
struct CharArray {
    char* Start; char* Finish; char* EndOfStorage;
    CharArray() : Start(0), Finish(0), EndOfStorage(0) { }
    ~CharArray() { FreeBlock(Start); }
};
struct CharString { int Id; CharString(int id) : Id(id) { } ~CharString() { Note(Id); } };
struct ChunkedFileSuperChunk { virtual ~ChunkedFileSuperChunk() { Note(9); } unsigned char Body[0x28]; };
struct ClothChunk : ChunkedFileSuperChunk {
    CharString Name; long MaterialIndex; CharString ParticleCode; CharArray CompiledCode;
    ClothChunk() : Name(3), MaterialIndex(0), ParticleCode(2) { }
};
int main()
{
    ClothChunk* chunk = new ClothChunk;
    chunk->CompiledCode.Start = (char*)std::malloc(16);
    ChunkedFileSuperChunk* base = chunk;
    delete base;
    const int expected[4] = { 1, 2, 3, 9 };
    if (g_n != 4) { std::printf("00a9d3c0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i)
        if (g_log[i] != expected[i]) { std::printf("00a9d3c0_TEST FAIL\n"); return 1; }
    std::printf("00a9d3c0_TEST PASS\n");
    return 0;
}