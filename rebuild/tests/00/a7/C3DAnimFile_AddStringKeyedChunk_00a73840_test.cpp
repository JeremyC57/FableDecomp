#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_strings = 0, g_destroyed = 0;
class CCharString { public:
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
class CCountedPointer { public:
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
class C3DAnimFileRootChunk { public: int id; };
struct SNamedChunk {
    CCharString Name;
    CCountedPointer<C3DAnimFileRootChunk> Chunk;
    SNamedChunk(const CCharString& name, const CCountedPointer<C3DAnimFileRootChunk>& chunk) : Name(name), Chunk(chunk) {}
};
#include <new>
struct SKeyedChunk {
    CCharString Key;
    SNamedChunk Value;
    SKeyedChunk(const CCharString& key, const SNamedChunk& value) : Key(key), Value(value) {}
};
struct CVec {
    SKeyedChunk* m_pFirst; SKeyedChunk* m_pLast; SKeyedChunk* m_pEnd;
    CVec() { m_pFirst = (SKeyedChunk*)std::malloc(4 * sizeof(SKeyedChunk)); m_pLast = m_pFirst; m_pEnd = m_pFirst + 4; }
    void push_back(const SKeyedChunk& r) { new (m_pLast) SKeyedChunk(r); ++m_pLast; }
};
class C3DAnimFile { public:
    CVec m_Chunks; unsigned char m_flag0C; bool m_Changed;
    void AddStringKeyedChunk(const CCharString& key, const SNamedChunk& value)
    {
        m_Chunks.push_back(SKeyedChunk(key, value));
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
        CCharString key("k");
        SNamedChunk v(CCharString("bone"), CCountedPointer<C3DAnimFileRootChunk>(&root, block));
        f.AddStringKeyedChunk(key, v);
        SKeyedChunk& r = *f.m_Chunks.m_pFirst;
        if (!f.m_Changed || f.m_Chunks.m_pLast - f.m_Chunks.m_pFirst != 1 || block->m_references != 2 || std::strcmp(r.Key.m_pData, "k") != 0 ||
            std::strcmp(r.Value.Name.m_pData, "bone") != 0 || r.Value.Chunk.m_pObject != &root || g_strings != 4 || g_destroyed != 0) {
            std::printf("00a73840_TEST FAIL\n"); return 1;
        }
        r.~SKeyedChunk();
        std::free(f.m_Chunks.m_pFirst);
    }
    if (g_destroyed != 1 || g_strings != 0) { std::printf("00a73840_TEST FAIL\n"); return 1; }
    std::printf("00a73840_TEST PASS\n");
    return 0;
}