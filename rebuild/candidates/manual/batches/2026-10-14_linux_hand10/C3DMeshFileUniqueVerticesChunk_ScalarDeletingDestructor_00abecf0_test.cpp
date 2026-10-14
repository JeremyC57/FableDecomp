#include <cstdio>
// Model of C3DMeshFileUniqueVerticesChunk's deleting destructor @ 0x00ABECF0: free the array data, chunk base destructor.
static int g_freed, g_base; static void* g_freedPtr;
void MyFree(void* p) { ++g_freed; g_freedPtr = p; }
struct CArrayULong { unsigned long* m_pData; long m_count, m_capacity; ~CArrayULong() { if (m_pData != 0) MyFree(m_pData); } };
struct CChunkedFileChunk { virtual ~CChunkedFileChunk() { ++g_base; } };
struct Chunk : CChunkedFileChunk { CArrayULong UniqueVerticesList; virtual ~Chunk() {} };
int main()
{
    unsigned long data[2];
    { Chunk c; c.UniqueVerticesList.m_pData = data; }
    if (g_freed != 1 || g_freedPtr != data || g_base != 1) { std::printf("00abecf0_TEST FAIL\n"); return 1; }
    Chunk* p = new Chunk; p->UniqueVerticesList.m_pData = 0; CChunkedFileChunk* b = p; delete b;
    if (g_freed != 1 || g_base != 2) { std::printf("00abecf0_TEST FAIL\n"); return 1; }
    std::printf("00abecf0_TEST PASS\n"); return 0;
}
