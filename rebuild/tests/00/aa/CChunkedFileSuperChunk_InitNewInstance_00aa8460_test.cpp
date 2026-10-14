#include <cstdio>
#include <stddef.h>
static int g_assigns = 0;
struct CChunkedFileSuperChunk_Retail;
struct CChunkedFileChunk_Retail {
    virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void v04() { } virtual void v05() { }
    virtual CChunkedFileSuperChunk_Retail* GetAsSuperChunk() { return 0; } virtual void v07() { }
    char ChunkType[5]; unsigned char _pad_0x09[3]; CChunkedFileSuperChunk_Retail* Parent;
};
struct CCountedChunk { void* m_pObject; void* m_pCount; };
struct CRetailChunkArray { CCountedChunk* first; CCountedChunk* last; CCountedChunk* end_of_storage; CRetailChunkArray& operator=(const CRetailChunkArray& o) { first = o.first; last = o.last; end_of_storage = o.end_of_storage; ++g_assigns; return *this; } };
struct CChunkedFileSuperChunk_Retail : CChunkedFileChunk_Retail {
    CRetailChunkArray SubChunks; CRetailChunkArray RegisteredSubChunkTypes; unsigned long CurrentChunkIndex;
    virtual CChunkedFileSuperChunk_Retail* GetAsSuperChunk() { return this; }
    CChunkedFileChunk_Retail* InitNewInstance(CChunkedFileChunk_Retail* instance) const
    {
        CChunkedFileSuperChunk_Retail* super = instance->GetAsSuperChunk();
        CRetailChunkArray* types = &super->RegisteredSubChunkTypes;
        *types = RegisteredSubChunkTypes;
        return instance;
    }
};
int main()
{
    CCountedChunk pool[2]; CChunkedFileSuperChunk_Retail src, dst;
    src.RegisteredSubChunkTypes.first = pool; src.RegisteredSubChunkTypes.last = pool + 2; src.RegisteredSubChunkTypes.end_of_storage = pool + 2;
    dst.RegisteredSubChunkTypes.first = 0; dst.RegisteredSubChunkTypes.last = 0; dst.RegisteredSubChunkTypes.end_of_storage = 0;
    CChunkedFileChunk_Retail* r = src.InitNewInstance(&dst);
    if (r != &dst || g_assigns != 1 || dst.RegisteredSubChunkTypes.first != pool || dst.RegisteredSubChunkTypes.last != pool + 2
        || offsetof(CChunkedFileSuperChunk_Retail, RegisteredSubChunkTypes) != 0x1c) { std::printf("00aa8460_TEST FAIL\n"); return 1; }
    std::printf("00aa8460_TEST PASS\n"); return 0;
}