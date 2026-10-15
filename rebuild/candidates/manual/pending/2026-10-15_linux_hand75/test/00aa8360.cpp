#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __cdecl
#endif
#include <string.h>
struct CBaseClassNonCopyable { virtual ~CBaseClassNonCopyable() { } CBaseClassNonCopyable() { } };
template <class T> struct CArray { T* m_pBegin; T* m_pEnd; T* m_pEndOfStorage; CArray() : m_pBegin(0), m_pEnd(0), m_pEndOfStorage(0) { } };
struct SCountBlock;
template <class T> struct CCountedPointer { T* m_pObject; SCountBlock* m_pCount; };
struct CChunkedFileSuperChunk;
struct CChunkedFileChunk : CBaseClassNonCopyable {
    char ChunkType[5];
    CChunkedFileSuperChunk* Parent;
    CChunkedFileChunk(const char* type) : Parent(0) { memset(ChunkType, 0, sizeof(ChunkType)); memcpy(ChunkType, type, 4); }
};
struct CChunkedFileSuperChunk : CChunkedFileChunk {
    CArray<CCountedPointer<CChunkedFileChunk> > SubChunks;
    CArray<CCountedPointer<CChunkedFileChunk> > RegisteredSubChunkTypes;
    unsigned long CurrentChunkIndex;
    CChunkedFileSuperChunk(const char* type) : CChunkedFileChunk(type), CurrentChunkIndex(0) { }
};
int main()
{
    CChunkedFileSuperChunk c("AOBJXYZ");
    if (strcmp(c.ChunkType, "AOBJ") != 0 || c.Parent != 0 || c.SubChunks.m_pBegin != 0 || c.SubChunks.m_pEndOfStorage != 0
        || c.RegisteredSubChunkTypes.m_pEnd != 0 || c.CurrentChunkIndex != 0) { std::printf("00aa8360_TEST FAIL\n"); return 1; }
    std::printf("00aa8360_TEST PASS\n");
    return 0;
}
