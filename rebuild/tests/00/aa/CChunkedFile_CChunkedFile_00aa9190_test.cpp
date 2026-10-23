#include <cstdio>
static int g_strings = 0, g_destroyed = 0, g_blocks = 0, g_bases = 0;
struct CChunkedFileSuperChunk { int x; };
struct CCharString { void* PStringData; ~CCharString() { ++g_strings; } };
struct SCountBlock { long m_references; void (__fastcall* m_pfnDestroy)(CChunkedFileSuperChunk*); CChunkedFileSuperChunk* m_pObject; };
struct CCountedPointer_CChunkedFileSuperChunk {
    CChunkedFileSuperChunk* m_pObject; SCountBlock* m_pCount;
    ~CCountedPointer_CChunkedFileSuperChunk()
    {
        if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); ++g_blocks; } }
        m_pObject = 0; m_pCount = 0;
    }
};
namespace N_0099a300 { struct CBaseClass { virtual ~CBaseClass() { ++g_bases; } }; } using N_0099a300::CBaseClass;
struct CChunkedFile : CBaseClass {
    CCountedPointer_CChunkedFileSuperChunk RootChunk; unsigned long Version; CCharString FileComment;
    virtual ~CChunkedFile() { }
};
static void __fastcall DestroyChunk(CChunkedFileSuperChunk*) { ++g_destroyed; }
int main()
{
    CChunkedFileSuperChunk chunk; SCountBlock block = { 1, DestroyChunk, &chunk };
    CChunkedFile* f = new CChunkedFile; f->RootChunk.m_pObject = &chunk; f->RootChunk.m_pCount = &block; f->FileComment.PStringData = 0;
    CBaseClass* b = f;
    delete b;
    if (g_strings != 1 || g_destroyed != 1 || g_blocks != 1 || g_bases != 1) { std::printf("00aa9190_TEST FAIL\n"); return 1; }
    SCountBlock block2 = { 2, DestroyChunk, &chunk };
    f = new CChunkedFile; f->RootChunk.m_pObject = &chunk; f->RootChunk.m_pCount = &block2; f->FileComment.PStringData = 0;
    delete f;
    if (g_strings != 2 || g_destroyed != 1 || block2.m_references != 1 || g_bases != 2) { std::printf("00aa9190_TEST FAIL\n"); return 1; }
    std::printf("00aa9190_TEST PASS\n");
    return 0;
}