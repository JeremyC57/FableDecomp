// NUISystem::CManager::OnCloseMenus() -- when the last open menu closes, give the engine resource pool back its pre-menu size
// Leaf: ?OnCloseMenus@CManager@NUISystem@@SIXXZ
// Retail: Fable.exe @ 0x0041D165, 51 bytes, plain `ret`; Ego_r.pdb ?OnCloseMenus@CManager@NUISystem@@SIXXZ (static
// __fastcall, no arguments). Counterpart of OnOpenMenus (0x0041D122), which bumps the open-menu count (0x013B8714) and,
// on the first menu, saves the pool's PoolSize (+0x0c) to 0x013B8718 and resizes the pool to 0x013758CC. Here:
// --count == 0 -> main game component (0x013B86A0; +0x1c PPlayerManager, +0x28 PDisplayEngine as at 0x0066E98B) ->
// PDisplayEngine -> Engine3D (+0x2c) -> virtual slot 0x120 (the engine's CHandleBasedMemoryPool; name not recovered)
// -> CHandleBasedMemoryPool::ResizePool(saved size, 1.0f) (0x00A02680, Ego_r.pdb ?ResizePool@...@QAE_NKM@Z).
// Layout: CDisplayEngine / CHandleBasedMemoryPool from engine/ headers (Engine3D +0x2c); CIEngine is local because
// engine/CIEngine.h models the vptr as a plain void* and the slot-0x120 call needs a polymorphic declaration.
// Opt: size TU (unaligned entry, `push ecx / fld1 / fstp [esp]` for the float argument).
#pragma optimize("s", on)
#include "engine/CDisplayEngine.h"
#include "engine/CHandleBasedMemoryPool.h"
struct CHandleBasedMemoryPool_Methods : CHandleBasedMemoryPool {
    bool ResizePool(unsigned long new_size, float time);  // 0x00A02680
};
struct CIEngine {
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
struct CMainGameComponent {
    unsigned char   _pad00[0x28];
    CDisplayEngine* PDisplayEngine;                      // +0x28 scoped_ptr<CDisplayEngine>
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
namespace NUISystem {
class CManager {
public:
    static long          OpenMenuCount;                  // 0x013B8714
    static unsigned long PoolSizeBeforeMenus;            // 0x013B8718
    static void __fastcall OnCloseMenus();
};
void __fastcall CManager::OnCloseMenus()
{
    if (--OpenMenuCount == 0) {
        CHandleBasedMemoryPool_Methods* pool = g_pMainGameComponent->PDisplayEngine->Engine3D->GetResourceMemoryPool();
        if (pool)
            pool->ResizePool(PoolSizeBeforeMenus, 1.0f);
    }
}
}