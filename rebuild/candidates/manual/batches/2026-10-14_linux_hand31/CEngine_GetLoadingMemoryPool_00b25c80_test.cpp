#include <cstdio>
#include <cstdlib>
static int g_destroyed = 0, g_deleted = 0;
static void operator_delete_x(void* p) { ++g_deleted; }
struct CMemoryPool { int x; };
struct SCountedInfo { long m_count; void (*m_pfnDestroy)(void*); void* m_pObject; };
static void DestroyPool(void*) { ++g_destroyed; }
struct CCountedPointer_Pool { CMemoryPool* m_pObject; SCountedInfo* m_pInfo;
    CCountedPointer_Pool() { }
    CCountedPointer_Pool(const CCountedPointer_Pool& o) : m_pObject(o.m_pObject), m_pInfo(o.m_pInfo) { if (m_pInfo) ++m_pInfo->m_count; }
    ~CCountedPointer_Pool() { if (m_pInfo) { if (--m_pInfo->m_count == 0) { m_pInfo->m_pfnDestroy(m_pInfo->m_pObject); operator_delete_x(m_pInfo); } } } };
struct CEngineData { unsigned char _pad[0x8e0]; CCountedPointer_Pool m_pool; };
static CEngineData g_data; static CEngineData* g_pEngineData = &g_data;
struct CEngine { static CMemoryPool* GetLoadingMemoryPool() { CCountedPointer_Pool pool(g_pEngineData->m_pool); return pool.m_pObject; } };
int main() { CMemoryPool pool; SCountedInfo info = { 1, DestroyPool, &pool }; g_data.m_pool.m_pObject = &pool; g_data.m_pool.m_pInfo = &info;
    if (CEngine::GetLoadingMemoryPool() != &pool || info.m_count != 1 || g_destroyed || g_deleted) { std::printf("00b25c80_TEST FAIL\n"); return 1; }
    info.m_count = 0; /* a dead holder: the temporary copy's release destroys */ if (CEngine::GetLoadingMemoryPool() != &pool || g_destroyed != 1 || g_deleted != 1) { std::printf("00b25c80_TEST FAIL\n"); return 1; }
    g_data.m_pool.m_pInfo = 0; if (CEngine::GetLoadingMemoryPool() != &pool || g_destroyed != 1) { std::printf("00b25c80_TEST FAIL\n"); return 1; } std::printf("00b25c80_TEST PASS\n"); return 0; }
