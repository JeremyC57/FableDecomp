// behaviour test: C3DAnimFileLoader::~C3DAnimFileLoader (empty user body) -- the chunk-type vector and the nine
// counted pointers are released (last declared first), then the CChunkedFileLoader base destructor runs.
#include <cstdio>
static int g_order[16]; static int g_n = 0;
struct CCountedPointer { int id; int* refs; ~CCountedPointer() { if (refs && --*refs == 0) g_order[g_n++] = id; } };
struct ChunkVector { ~ChunkVector() { g_order[g_n++] = 100; } };
class CChunkedFileLoader { public: virtual ~CChunkedFileLoader() { g_order[g_n++] = 200; } };
struct C3DAnimFileLoader : CChunkedFileLoader {
    CCountedPointer P[9]; ChunkVector ChunkTypes;
    ~C3DAnimFileLoader() { }
};
int main()
{
    int refs[9];
    C3DAnimFileLoader* l = new C3DAnimFileLoader;
    for (int i = 0; i < 9; ++i) { refs[i] = (i == 4) ? 2 : 1; l->P[i].id = i; l->P[i].refs = &refs[i]; }
    CChunkedFileLoader* base = l;
    delete base;
    int want[10] = { 100, 8, 7, 6, 5, 3, 2, 1, 0, 200 };   // P[4] still shared: not freed
    if (g_n != 10 || refs[4] != 1) { std::printf("00a996d0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 10; ++i) if (g_order[i] != want[i]) { std::printf("00a996d0_TEST FAIL\n"); return 1; }
    std::printf("00a996d0_TEST PASS\n");
    return 0;
}