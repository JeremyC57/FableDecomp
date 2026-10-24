// Behaviour test for 00add330 (CLHFile::Close): the file data reference is dropped (the last one destroys the data and frees its block) before the file object deletes itself.
#include <cstdio>
static int g_dataDestroyed = 0;
static int g_blocksFreed = 0;
static int g_filesDestroyed = 0;
static bool g_resetBeforeDelete = false;
struct SCountBlock {
    long  m_references;
    void (*m_pfnDestroy)(void* object);
    void* m_pObject;
    static void operator delete(void* p) { ++g_blocksFreed; ::operator delete(p); }
};
static void DestroyData(void*) { ++g_dataDestroyed; }
struct CCountedPointerFileData {
    void*        m_pObject;
    SCountBlock* m_pCount;
    void Reset()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pCount = 0;
        m_pObject = 0;
    }
};
namespace LHA {
struct CFile {
    virtual bool GetSize(unsigned long* size) { *size = 0; return false; }
    virtual void v01() { }
    virtual long GetMode() { return 0; }
    virtual void Close() { }
    virtual ~CFile() { }
};
}
struct CLHFile : LHA::CFile {
    CCountedPointerFileData m_data;
    virtual void Close();
    virtual ~CLHFile() { ++g_filesDestroyed; g_resetBeforeDelete = (m_data.m_pCount == 0 && m_data.m_pObject == 0); }
};
void CLHFile::Close()
{
    m_data.Reset();
    delete this;
}
int main()
{
    CLHFile* file = new CLHFile;
    SCountBlock* block = new SCountBlock;
    block->m_references = 1; block->m_pfnDestroy = DestroyData; block->m_pObject = 0;
    file->m_data.m_pObject = 0;
    file->m_data.m_pCount = block;
    LHA::CFile* base = file;
    base->Close();
    if (g_dataDestroyed != 1 || g_blocksFreed != 1 || g_filesDestroyed != 1 || !g_resetBeforeDelete) { std::printf("00add330_TEST FAIL\n"); return 1; }
    SCountBlock shared;
    shared.m_references = 2; shared.m_pfnDestroy = DestroyData; shared.m_pObject = 0;
    file = new CLHFile;
    file->m_data.m_pObject = 0;
    file->m_data.m_pCount = &shared;
    file->Close();
    if (g_dataDestroyed != 1 || shared.m_references != 1 || g_filesDestroyed != 2) { std::printf("00add330_TEST FAIL\n"); return 1; }
    std::printf("00add330_TEST PASS\n");
    return 0;
}