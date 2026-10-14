#include <cstdio>
#include <cstring>
#include <new>

struct CSegment { int x; }; struct CSource { int x; }; struct CRoot { int x; };
struct CChunkOwner { virtual CRoot* GetRootChunk() { return m_root; } CRoot* m_root; };
template <class T> struct CCountedPointer { T* m_pObject; long* m_pCount; CCountedPointer(T* o, long* c) : m_pObject(o), m_pCount(c) {} };
static CSource* g_src; static CRoot* g_root; static CSegment g_segment; static long g_count;
struct C3DMeshFileLoader {
    CChunkOwner* m_pMeshChunks;
    CCountedPointer<CSegment> MakeSegment(CSource* source, CRoot* root) { g_src = source; g_root = root; return CCountedPointer<CSegment>(&g_segment, &g_count); }
    CCountedPointer<CSegment> Create(CSource* source) { return MakeSegment(source, m_pMeshChunks->GetRootChunk()); }
};
int main()
{
    CRoot root; CChunkOwner owner; owner.m_root = &root; CSource source; C3DMeshFileLoader loader; loader.m_pMeshChunks = &owner;
    CCountedPointer<CSegment> result = loader.Create(&source);
    if (g_src != &source || g_root != &root || result.m_pObject != &g_segment || result.m_pCount != &g_count) { std::printf("00a9ba10_TEST FAIL\n"); return 1; }
    std::printf("00a9ba10_TEST PASS\n");
    return 0;
}