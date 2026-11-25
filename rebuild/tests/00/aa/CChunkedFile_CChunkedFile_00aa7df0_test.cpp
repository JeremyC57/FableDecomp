#include <cstdio>
#include <cstring>
static int g_baseCtor = 0, g_baseDtor = 0, g_destroyed = 0, g_commentCtor = 0, g_commentDtor = 0; static const char* g_commentText = 0; static long g_commentLength = 0; static void* g_destroyedObject = 0;
// NF416: models the shared engine/CChunkedFile.h shape (CBaseClassNonCopyable base with a virtual destructor; virtual ~CChunkedFile,
// so a delete through the base pointer runs it).
struct CBaseClassNonCopyable { CBaseClassNonCopyable() { ++g_baseCtor; } virtual ~CBaseClassNonCopyable() { ++g_baseDtor; } };
class CChunkedFileSuperChunk;
struct SCountBlock { long m_references; void (__fastcall* m_pfnDestroy)(void*); void* m_pObject; };
static void __fastcall DestroyChunk(void* object) { ++g_destroyed; g_destroyedObject = object; }
struct CCountedSuperChunkPointer {
    CChunkedFileSuperChunk* Data; SCountBlock* Info;
    CCountedSuperChunkPointer() : Data(0), Info(0) { }
    CCountedSuperChunkPointer(const CCountedSuperChunkPointer& other) : Data(other.Data), Info(other.Info) { if (Info != 0) ++Info->m_references; }
    ~CCountedSuperChunkPointer()
    {
        if (Info != 0) { --Info->m_references; if (Info->m_references == 0) { Info->m_pfnDestroy(Info->m_pObject); delete Info; } }
        Data = 0; Info = 0;
    }
};
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length) : PStringData(0) { ++g_commentCtor; g_commentText = text; g_commentLength = length; }
    ~CCharString() { ++g_commentDtor; }
};
class CChunkedFile : public CBaseClassNonCopyable { public:
    CCountedSuperChunkPointer RootChunk; unsigned long Version; CCharString FileComment;
    CChunkedFile(const CCountedSuperChunkPointer& rootChunk) : CBaseClassNonCopyable(), RootChunk(rootChunk), Version(0), FileComment("", -1) { }
    virtual ~CChunkedFile() { }
};
int main()
{
    SCountBlock* block = new SCountBlock; block->m_references = 1; block->m_pfnDestroy = DestroyChunk; block->m_pObject = (void*)0x5678;
    CCountedSuperChunkPointer root; root.Data = (CChunkedFileSuperChunk*)0x5678; root.Info = block;
    CChunkedFile* a = new CChunkedFile(root);
    CChunkedFile* b = new CChunkedFile(root);
    if (block->m_references != 3) { std::printf("00aa7df0_TEST FAIL\n"); return 1; }
    delete a;
    if (block->m_references != 2 || g_destroyed != 0 || g_commentDtor != 1 || g_baseDtor != 1) { std::printf("00aa7df0_TEST FAIL\n"); return 1; }
    root.Info = 0; root.Data = 0;   // let the local go out of scope quietly
    block->m_references = 1;
    CBaseClassNonCopyable* base = b;
    delete base;                    // virtual: ~CChunkedFile runs through the base pointer
    if (g_destroyed != 1 || g_destroyedObject != (void*)0x5678 || g_commentDtor != 2 || g_baseDtor != 2) { std::printf("00aa7df0_TEST FAIL\n"); return 1; }
    std::printf("00aa7df0_TEST PASS\n"); return 0;
}