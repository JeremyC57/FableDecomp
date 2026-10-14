#include <cstdio>
#include <cstdlib>
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
static int g_destroyed = 0;
static void DestroyRoot(void*) { ++g_destroyed; }
template <class T> struct CCountedPointer {
    T* m_pObject; SCountBlock* m_pCount;
    CCountedPointer(T* object) : m_pObject(object), m_pCount(new SCountBlock) { m_pCount->m_references = 1; m_pCount->m_pfnDestroy = DestroyRoot; m_pCount->m_pObject = object; }
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount) { if (m_pCount) ++m_pCount->m_references; }
    ~CCountedPointer() { if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); delete m_pCount; } } }
};
struct CChunkedFileSuperChunk { virtual void v00() {} };
struct C3DMeshFileRootChunk : CChunkedFileSuperChunk { };
static long g_baseRefsSeen = -1;
struct CChunkedFile {
    virtual void v00() {}
    CChunkedFileSuperChunk* m_pRoot;
    CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>& root) : m_pRoot(root.m_pObject) { g_baseRefsSeen = root.m_pCount->m_references; }
};
struct C3DMeshFile : CChunkedFile {
    virtual void v00() {}
    C3DMeshFile(const CCountedPointer<C3DMeshFileRootChunk>& root) : CChunkedFile(root) {}
};
int main()
{
    C3DMeshFileRootChunk chunk;
    {
        CCountedPointer<C3DMeshFileRootChunk> root(&chunk);
        C3DMeshFile file(root);
        // the base saw the conversion temporary holding a second reference; it is gone again afterwards
        if (file.m_pRoot != &chunk || g_baseRefsSeen != 2 || root.m_pCount->m_references != 1 || g_destroyed != 0) { std::printf("00a9ae60_TEST FAIL\n"); return 1; }
    }
    if (g_destroyed != 1) { std::printf("00a9ae60_TEST FAIL\n"); return 1; }
    std::printf("00a9ae60_TEST PASS\n");
    return 0;
}