#include <cstdio>
#include <cstdlib>
static int g_clear = 0, g_destroy = 0, g_blockFreed = 0; static void* g_destroyed = 0;
struct SCountBlock { long m_count; void (*m_pfnDestroy)(void*); void* m_pObject; };
static void op_delete(void* p) { ++g_blockFreed; std::free(p); }
struct CCountedResourcePointer { void* m_pObject; SCountBlock* m_pBlock;
    ~CCountedResourcePointer() { if (m_pBlock) { --m_pBlock->m_count; if (m_pBlock->m_count == 0) { m_pBlock->m_pfnDestroy(m_pBlock->m_pObject); op_delete(m_pBlock); } } m_pObject = 0; m_pBlock = 0; } };
static void DestroyObject(void* o) { ++g_destroy; g_destroyed = o; }
struct CMovableResource { virtual ~CMovableResource() { } };
struct CEngineLandscapePatch : CMovableResource { unsigned char _pad04[0x3c]; CCountedResourcePointer m_resource; void Clear() { ++g_clear; } virtual ~CEngineLandscapePatch() { Clear(); } };
int main()
{
    int obj; SCountBlock* b = (SCountBlock*)std::malloc(sizeof(SCountBlock)); b->m_count = 2; b->m_pfnDestroy = DestroyObject; b->m_pObject = &obj;
    { CEngineLandscapePatch p; p.m_resource.m_pObject = &obj; p.m_resource.m_pBlock = b; }
    if (g_clear != 1 || g_destroy != 0 || b->m_count != 1) { std::printf("00bf3a40_TEST FAIL\n"); return 1; }
    { CEngineLandscapePatch p; p.m_resource.m_pObject = &obj; p.m_resource.m_pBlock = b; }
    if (g_clear != 2 || g_destroy != 1 || g_destroyed != &obj || g_blockFreed != 1) { std::printf("00bf3a40_TEST FAIL\n"); return 1; }
    std::printf("00bf3a40_TEST PASS\n"); return 0;
}
