#include <cstdio>
#include <cstdlib>
static int g_reset = 0, g_destroy = 0, g_blockFreed = 0, g_objFreed = 0;
struct SCountBlock { long m_count; void (*m_pfnDestroy)(void*); void* m_pObject; };
static void op_delete(void* p) { ++g_blockFreed; std::free(p); }
struct CCountedResourcePointer { void* m_pObject; SCountBlock* m_pBlock;
    ~CCountedResourcePointer() { if (m_pBlock) { --m_pBlock->m_count; if (m_pBlock->m_count == 0) { m_pBlock->m_pfnDestroy(m_pBlock->m_pObject); op_delete(m_pBlock); } } m_pObject = 0; m_pBlock = 0; } };
static void DestroyObject(void*) { ++g_destroy; }
class CMovableResource { public: virtual ~CMovableResource() { } static void operator delete(void* p) { ++g_objFreed; ::operator delete(p); } };
class CParticleSimulation : public CMovableResource { public: CCountedResourcePointer m_resource; void Reset() { ++g_reset; } virtual ~CParticleSimulation() { Reset(); } };
int main()
{
    int obj; SCountBlock* b = (SCountBlock*)std::malloc(sizeof(SCountBlock)); b->m_count = 1; b->m_pfnDestroy = DestroyObject; b->m_pObject = &obj;
    CMovableResource* s = new CParticleSimulation; static_cast<CParticleSimulation*>(s)->m_resource.m_pObject = &obj; static_cast<CParticleSimulation*>(s)->m_resource.m_pBlock = b;
    delete s;                                     // the scalar deleting destructor: destructor body, then the object's storage is freed
    if (g_reset != 1 || g_destroy != 1 || g_blockFreed != 1 || g_objFreed != 1) { std::printf("00ae6070_TEST FAIL\n"); return 1; }
    std::printf("00ae6070_TEST PASS\n"); return 0;
}