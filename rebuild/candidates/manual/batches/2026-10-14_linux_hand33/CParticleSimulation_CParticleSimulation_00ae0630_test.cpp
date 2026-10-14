#include <cstdio>
#include <cstdlib>
static int g_reset = 0, g_destroy = 0, g_blockFreed = 0; static void* g_destroyed = 0;
struct SCountBlock { long m_count; void (*m_pfnDestroy)(void*); void* m_pObject; };
static void op_delete(void* p) { ++g_blockFreed; std::free(p); }
struct CCountedResourcePointer { void* m_pObject; SCountBlock* m_pBlock;
    ~CCountedResourcePointer() { if (m_pBlock) { --m_pBlock->m_count; if (m_pBlock->m_count == 0) { m_pBlock->m_pfnDestroy(m_pBlock->m_pObject); op_delete(m_pBlock); } } m_pObject = 0; m_pBlock = 0; } };
static void DestroyObject(void* o) { ++g_destroy; g_destroyed = o; }
struct CMovableResource { virtual ~CMovableResource() { } };
struct CParticleSimulation : CMovableResource { CCountedResourcePointer m_resource; void Reset() { ++g_reset; } virtual ~CParticleSimulation() { Reset(); } };
int main()
{
    int obj; SCountBlock* b = (SCountBlock*)std::malloc(sizeof(SCountBlock)); b->m_count = 2; b->m_pfnDestroy = DestroyObject; b->m_pObject = &obj;
    { CParticleSimulation s; s.m_resource.m_pObject = &obj; s.m_resource.m_pBlock = b; }
    if (g_reset != 1 || g_destroy != 0 || b->m_count != 1) { std::printf("00ae0630_TEST FAIL\n"); return 1; }
    { CParticleSimulation s; s.m_resource.m_pObject = &obj; s.m_resource.m_pBlock = b; }
    if (g_reset != 2 || g_destroy != 1 || g_destroyed != &obj || g_blockFreed != 1) { std::printf("00ae0630_TEST FAIL\n"); return 1; }
    { CParticleSimulation s; s.m_resource.m_pObject = 0; s.m_resource.m_pBlock = 0; }
    if (g_reset != 3 || g_destroy != 1) { std::printf("00ae0630_TEST FAIL\n"); return 1; }
    std::printf("00ae0630_TEST PASS\n"); return 0;
}
