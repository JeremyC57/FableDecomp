#include <cstdio>
#include <stddef.h>
#include <string.h>
#include <new>
static int g_base = 0;
struct CChunkedFileSuperChunk_Retail;
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
struct SChunkTypeCode { char Code[4]; };
struct SChunkInfo { unsigned long Flags; SChunkTypeCode Type; };
struct CChunkedFileChunk_Retail : CBase {
    char ChunkType[5]; unsigned char _pad_0x09[3]; CChunkedFileSuperChunk_Retail* Parent; virtual void v00() { }
    CChunkedFileChunk_Retail(const SChunkTypeCode& type) : CBase(), Parent(0) { memset(ChunkType, 0, sizeof(ChunkType)); memcpy(ChunkType, &type, sizeof(type)); }
};
struct CCountedChunk { void* m_pObject; void* m_pCount; };
template<class T> struct CRetailArray { T* first; T* last; T* end_of_storage; CRetailArray() : first(0), last(0), end_of_storage(0) { } };
struct CChunkedFileSuperChunk_Retail : CChunkedFileChunk_Retail {
    CRetailArray<CCountedChunk> SubChunks; CRetailArray<CCountedChunk> RegisteredSubChunkTypes; unsigned long CurrentChunkIndex; virtual void v00() { }
    CChunkedFileSuperChunk_Retail(const SChunkInfo& info) : CChunkedFileChunk_Retail(info.Type), SubChunks(), RegisteredSubChunkTypes() { }
};
int main()
{
    SChunkInfo info; info.Flags = 0x11223344; memcpy(info.Type.Code, "MESH", 4);
    unsigned char raw[sizeof(CChunkedFileSuperChunk_Retail)]; for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xAA;
    CChunkedFileSuperChunk_Retail* c = new (raw) CChunkedFileSuperChunk_Retail(info);
    if (g_base != 1 || strcmp(c->ChunkType, "MESH") != 0 || c->Parent != 0 || c->SubChunks.first || c->SubChunks.last || c->SubChunks.end_of_storage
        || c->RegisteredSubChunkTypes.first || c->RegisteredSubChunkTypes.last || c->RegisteredSubChunkTypes.end_of_storage
        || offsetof(CChunkedFileSuperChunk_Retail, ChunkType) != 4 || offsetof(CChunkedFileSuperChunk_Retail, Parent) != 0xc
        || offsetof(CChunkedFileSuperChunk_Retail, SubChunks) != 0x10 || offsetof(CChunkedFileSuperChunk_Retail, RegisteredSubChunkTypes) != 0x1c
        || offsetof(CChunkedFileSuperChunk_Retail, CurrentChunkIndex) != 0x28) { std::printf("00aa83b0_TEST FAIL\n"); return 1; }
    std::printf("00aa83b0_TEST PASS\n"); return 0;
}