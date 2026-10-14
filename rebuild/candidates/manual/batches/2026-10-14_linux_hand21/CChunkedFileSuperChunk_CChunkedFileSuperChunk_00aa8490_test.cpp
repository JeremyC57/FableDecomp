#include <cstdio>
#include <stddef.h>
static int g_frees = 0, g_ranges = 0, g_removes = 0, g_baseDtors = 0; static void* g_freed[2]; static void* g_rangeFirst[2];
extern "C" void __cdecl free_stub(void* block) { g_freed[g_frees++ & 1] = block; }
#define free free_stub
struct CBaseClassNonCopyable { virtual void v00() { } ~CBaseClassNonCopyable() { ++g_baseDtors; } };
struct CChunkedFileChunk_Retail : CBaseClassNonCopyable { unsigned char _body[0x0c]; virtual void v00() { } ~CChunkedFileChunk_Retail() { } };
struct CCountedChunk { void* m_pObject; void* m_pCount; };
struct SNonscalarPtrIteratorTag { SNonscalarPtrIteratorTag() { } };
void __fastcall DestroyRange(CCountedChunk* first, CCountedChunk* last, const SNonscalarPtrIteratorTag&) { g_rangeFirst[g_ranges++ & 1] = first; (void)last; }
template<class T> struct CRetailArray { T* first; T* last; T* end_of_storage; ~CRetailArray() { DestroyRange(first, last, SNonscalarPtrIteratorTag()); if (first != 0) free(first); } };
struct CChunkedFileSuperChunk_Retail : CChunkedFileChunk_Retail {
    CRetailArray<CCountedChunk> SubChunks; CRetailArray<CCountedChunk> RegisteredSubChunkTypes; unsigned long CurrentChunkIndex;
    virtual void v00() { }
    void RemoveAllSubChunks() { ++g_removes; }
    ~CChunkedFileSuperChunk_Retail() { RemoveAllSubChunks(); }
};
int main()
{
    CCountedChunk a[2], b[3];
    {
        CChunkedFileSuperChunk_Retail chunk;
        chunk.SubChunks.first = a; chunk.SubChunks.last = a + 2; chunk.RegisteredSubChunkTypes.first = b; chunk.RegisteredSubChunkTypes.last = b + 3;
    }
    if (g_removes != 1 || g_ranges != 2 || g_rangeFirst[0] != b || g_rangeFirst[1] != a || g_frees != 2 || g_freed[0] != b || g_freed[1] != a || g_baseDtors != 1
        || offsetof(CChunkedFileSuperChunk_Retail, SubChunks) != 0x10 || offsetof(CChunkedFileSuperChunk_Retail, RegisteredSubChunkTypes) != 0x1c || sizeof(CChunkedFileSuperChunk_Retail) != 0x2c) { std::printf("00aa8490_TEST FAIL\n"); return 1; }
    std::printf("00aa8490_TEST PASS\n"); return 0;
}
