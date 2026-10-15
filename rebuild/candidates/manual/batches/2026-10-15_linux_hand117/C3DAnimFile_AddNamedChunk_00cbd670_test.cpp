#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_strings = 0, g_destroyed = 0;
struct CCharString {
    char* m_pData;
    CCharString(const char* s) { m_pData = (char*)std::malloc(std::strlen(s) + 1); std::strcpy(m_pData, s); ++g_strings; }
    CCharString(const CCharString& o) { m_pData = (char*)std::malloc(std::strlen(o.m_pData) + 1); std::strcpy(m_pData, o.m_pData); ++g_strings; }
    ~CCharString() { std::free(m_pData); --g_strings; }
};
struct CCountedItem { void Destroy() { ++g_destroyed; } };
struct SCountBlock {
    long m_references;
    void (CCountedItem::*m_pfnDestroy)();
    CCountedItem* m_pItem;
};
template <class T>
struct CCountedPointer {
    T* m_pObject;
    SCountBlock* m_pBlock;
    CCountedPointer(T* o, SCountBlock* b) : m_pObject(o), m_pBlock(b) {}
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pBlock(other.m_pBlock)
    {
        if (m_pBlock) ++m_pBlock->m_references;
    }
    ~CCountedPointer()
    {
        if (m_pBlock) {
            --m_pBlock->m_references;
            if (m_pBlock->m_references == 0) {
                (m_pBlock->m_pItem->*m_pBlock->m_pfnDestroy)();
                delete m_pBlock;
            }
        }
        m_pObject = 0;
        m_pBlock = 0;
    }
};
struct C3DAnimFileRootChunk { int id; };
struct SNamedChunk {
    CCharString Name;
    CCountedPointer<C3DAnimFileRootChunk> Chunk;
    SNamedChunk(const CCharString& name, const CCountedPointer<C3DAnimFileRootChunk>& chunk) : Name(name), Chunk(chunk) {}
};
#include <new>
struct CVec {
    SNamedChunk* m_pFirst; SNamedChunk* m_pLast; SNamedChunk* m_pEnd;
    CVec() { m_pFirst = (SNamedChunk*)std::malloc(4 * sizeof(SNamedChunk)); m_pLast = m_pFirst; m_pEnd = m_pFirst + 4; }
    void push_back(const SNamedChunk& r) { new (m_pLast) SNamedChunk(r); ++m_pLast; }
};
struct C3DAnimFile {
    CVec m_Chunks; unsigned char m_flag0C; bool m_Changed;
    void AddNamedChunk(const CCharString& name, const CCountedPointer<C3DAnimFileRootChunk>& chunk)
    {
        m_Chunks.push_back(SNamedChunk(name, chunk));
        m_Changed = true;
    }
};
int main()
{
    CCountedItem item;
    SCountBlock* block = new SCountBlock;
    block->m_references = 1; block->m_pfnDestroy = &CCountedItem::Destroy; block->m_pItem = &item;
    C3DAnimFileRootChunk root; root.id = 7;
    {
        C3DAnimFile f; f.m_Changed = false;
        CCountedPointer<C3DAnimFileRootChunk> p(&root, block);
        CCharString n("bone");
        f.AddNamedChunk(n, p);
        SNamedChunk& r = *f.m_Chunks.m_pFirst;
        if (!f.m_Changed || f.m_Chunks.m_pLast - f.m_Chunks.m_pFirst != 1 || block->m_references != 2 ||
            std::strcmp(r.Name.m_pData, "bone") != 0 || r.Chunk.m_pObject != &root || g_strings != 2 || g_destroyed != 0) {
            std::printf("00cbd670_TEST FAIL\n"); return 1;
        }
        r.~SNamedChunk();
        std::free(f.m_Chunks.m_pFirst);
    }
    if (g_destroyed != 1 || g_strings != 0) { std::printf("00cbd670_TEST FAIL\n"); return 1; }
    std::printf("00cbd670_TEST PASS\n");
    return 0;
}
