#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_freed = 0; static void* g_freedPtr[8];
static void free_(void* p) { g_freedPtr[g_freed++] = p; }
#define free free_
static int g_base = 0;
struct CChunkedFileChunk { unsigned char _body[0x10]; ~CChunkedFileChunk() { ++g_base; } };
struct CChunkedFileChunkBuffers_00aa7370 : CChunkedFileChunk { void* m_bufferA; unsigned char _pad14[8]; void* m_bufferB; ~CChunkedFileChunkBuffers_00aa7370() { if (m_bufferB) free(m_bufferB); if (m_bufferA) free(m_bufferA); } };
static void Destroy_CChunkedFileChunkBuffers_00aa7370(CChunkedFileChunkBuffers_00aa7370* o) { delete o; }
int main() {
    int x1, x2; CChunkedFileChunkBuffers_00aa7370* o = new CChunkedFileChunkBuffers_00aa7370; o->m_bufferA = &x1; o->m_bufferB = &x2; Destroy_CChunkedFileChunkBuffers_00aa7370(o);
    if (g_freed != 2 || g_freedPtr[0] != &x2 || g_freedPtr[1] != &x1 || g_base != 1) { std::printf("00aa7370_TEST FAIL\n"); return 1; }
    std::printf("00aa7370_TEST PASS\n"); return 0;
}
