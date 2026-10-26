#include <cstdio>
#include "engine/CDisplayEngine.h"
#include "engine/CHandleBasedMemoryPool.h"
struct CHandleBasedMemoryPool_Methods : CHandleBasedMemoryPool {
    bool ResizePool(unsigned long new_size, float time);  // 0x00A02680
};
class CIEngine { public:
    virtual void _slot00(); virtual void _slot01(); virtual void _slot02(); virtual void _slot03();
    virtual void _slot04(); virtual void _slot05(); virtual void _slot06(); virtual void _slot07();
    virtual void _slot08(); virtual void _slot09(); virtual void _slot10(); virtual void _slot11();
    virtual void _slot12(); virtual void _slot13(); virtual void _slot14(); virtual void _slot15();
    virtual void _slot16(); virtual void _slot17(); virtual void _slot18(); virtual void _slot19();
    virtual void _slot20(); virtual void _slot21(); virtual void _slot22(); virtual void _slot23();
    virtual void _slot24(); virtual void _slot25(); virtual void _slot26(); virtual void _slot27();
    virtual void _slot28(); virtual void _slot29(); virtual void _slot30(); virtual void _slot31();
    virtual void _slot32(); virtual void _slot33(); virtual void _slot34(); virtual void _slot35();
    virtual void _slot36(); virtual void _slot37(); virtual void _slot38(); virtual void _slot39();
    virtual void _slot40(); virtual void _slot41(); virtual void _slot42(); virtual void _slot43();
    virtual void _slot44(); virtual void _slot45(); virtual void _slot46(); virtual void _slot47();
    virtual void _slot48(); virtual void _slot49(); virtual void _slot50(); virtual void _slot51();
    virtual void _slot52(); virtual void _slot53(); virtual void _slot54(); virtual void _slot55();
    virtual void _slot56(); virtual void _slot57(); virtual void _slot58(); virtual void _slot59();
    virtual void _slot60(); virtual void _slot61(); virtual void _slot62(); virtual void _slot63();
    virtual void _slot64(); virtual void _slot65(); virtual void _slot66(); virtual void _slot67();
    virtual void _slot68(); virtual void _slot69(); virtual void _slot70(); virtual void _slot71();
    virtual CHandleBasedMemoryPool_Methods* GetResourceMemoryPool();   // +0x120
};
class CMainGameComponent { public:
    unsigned char   _pad00[0x28];
    CDisplayEngine* PDisplayEngine;                      // +0x28 scoped_ptr<CDisplayEngine>
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
namespace NUISystem {
class CManager {
public:
    static long          OpenMenuCount;                  // 0x013B8714
    static unsigned long PoolSizeBeforeMenus;            // 0x013B8718
    static unsigned long MenuPoolSize;                   // 0x013758CC
    static void __fastcall OnOpenMenus();
};
void __fastcall CManager::OnOpenMenus()
{
    ++OpenMenuCount;
    if (OpenMenuCount == 1) {
        CHandleBasedMemoryPool_Methods* pool = g_pMainGameComponent->PDisplayEngine->Engine3D->GetResourceMemoryPool();
        if (pool) {
            PoolSizeBeforeMenus = pool->PoolSize;
            pool->ResizePool(MenuPoolSize, 1.0f);
        }
    }
}
}

void CIEngine::_slot00() {}
void CIEngine::_slot01() {}
void CIEngine::_slot02() {}
void CIEngine::_slot03() {}
void CIEngine::_slot04() {}
void CIEngine::_slot05() {}
void CIEngine::_slot06() {}
void CIEngine::_slot07() {}
void CIEngine::_slot08() {}
void CIEngine::_slot09() {}
void CIEngine::_slot10() {}
void CIEngine::_slot11() {}
void CIEngine::_slot12() {}
void CIEngine::_slot13() {}
void CIEngine::_slot14() {}
void CIEngine::_slot15() {}
void CIEngine::_slot16() {}
void CIEngine::_slot17() {}
void CIEngine::_slot18() {}
void CIEngine::_slot19() {}
void CIEngine::_slot20() {}
void CIEngine::_slot21() {}
void CIEngine::_slot22() {}
void CIEngine::_slot23() {}
void CIEngine::_slot24() {}
void CIEngine::_slot25() {}
void CIEngine::_slot26() {}
void CIEngine::_slot27() {}
void CIEngine::_slot28() {}
void CIEngine::_slot29() {}
void CIEngine::_slot30() {}
void CIEngine::_slot31() {}
void CIEngine::_slot32() {}
void CIEngine::_slot33() {}
void CIEngine::_slot34() {}
void CIEngine::_slot35() {}
void CIEngine::_slot36() {}
void CIEngine::_slot37() {}
void CIEngine::_slot38() {}
void CIEngine::_slot39() {}
void CIEngine::_slot40() {}
void CIEngine::_slot41() {}
void CIEngine::_slot42() {}
void CIEngine::_slot43() {}
void CIEngine::_slot44() {}
void CIEngine::_slot45() {}
void CIEngine::_slot46() {}
void CIEngine::_slot47() {}
void CIEngine::_slot48() {}
void CIEngine::_slot49() {}
void CIEngine::_slot50() {}
void CIEngine::_slot51() {}
void CIEngine::_slot52() {}
void CIEngine::_slot53() {}
void CIEngine::_slot54() {}
void CIEngine::_slot55() {}
void CIEngine::_slot56() {}
void CIEngine::_slot57() {}
void CIEngine::_slot58() {}
void CIEngine::_slot59() {}
void CIEngine::_slot60() {}
void CIEngine::_slot61() {}
void CIEngine::_slot62() {}
void CIEngine::_slot63() {}
void CIEngine::_slot64() {}
void CIEngine::_slot65() {}
void CIEngine::_slot66() {}
void CIEngine::_slot67() {}
void CIEngine::_slot68() {}
void CIEngine::_slot69() {}
void CIEngine::_slot70() {}
void CIEngine::_slot71() {}
static CHandleBasedMemoryPool_Methods* g_pool; static int g_calls; static unsigned long g_size; static float g_time;
CHandleBasedMemoryPool_Methods* CIEngine::GetResourceMemoryPool() { return g_pool; }
bool CHandleBasedMemoryPool_Methods::ResizePool(unsigned long n, float t) { ++g_calls; g_size = n; g_time = t; if (this != g_pool) g_calls = 100; return true; }
long NUISystem::CManager::OpenMenuCount = 0;
unsigned long NUISystem::CManager::PoolSizeBeforeMenus = 0;
unsigned long NUISystem::CManager::MenuPoolSize = 0x200000;
CMainGameComponent* g_pMainGameComponent = 0;
int main()
{
    static unsigned char pool_mem[sizeof(CHandleBasedMemoryPool)], de_mem[sizeof(CDisplayEngine)];
    CIEngine engine; CMainGameComponent comp;
    CDisplayEngine* de = (CDisplayEngine*)de_mem; de->Engine3D = &engine; comp.PDisplayEngine = de;
    g_pMainGameComponent = &comp; g_pool = (CHandleBasedMemoryPool_Methods*)pool_mem; g_pool->PoolSize = 0x5000;
    NUISystem::CManager::OnOpenMenus();
    bool ok = NUISystem::CManager::OpenMenuCount == 1 && g_calls == 1 && g_size == 0x200000 && g_time == 1.0f
        && NUISystem::CManager::PoolSizeBeforeMenus == 0x5000;
    g_pool->PoolSize = 0x7777;
    NUISystem::CManager::OnOpenMenus();
    ok = ok && NUISystem::CManager::OpenMenuCount == 2 && g_calls == 1 && NUISystem::CManager::PoolSizeBeforeMenus == 0x5000;
    g_pool = 0; NUISystem::CManager::OpenMenuCount = 0;
    NUISystem::CManager::OnOpenMenus();
    ok = ok && NUISystem::CManager::OpenMenuCount == 1 && g_calls == 1;
    std::printf(ok ? "0041d122_TEST PASS\n" : "FAIL\n");
    return 0;
}