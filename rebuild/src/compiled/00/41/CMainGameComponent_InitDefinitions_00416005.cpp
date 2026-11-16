// CMainGameComponent::InitDefinitions(bool flag) -- bracket the definition manager's init/compile with two calls on a global service, optionally run a follow-up step
// Leaf: ?InitDefinitions@CMainGameComponent@@IAE_N_N@Z
// Retail: Fable.exe @ 0x00416005, 66 bytes, `ret 4` (thiscall, `this` unused); one caller (0x004185D9); Ego_r.pdb
// ?InitDefinitions@CMainGameComponent@@IAE_N_N@Z.
// Body: if CMemoryAllocator::PMemoryAllocator (0x013CAA90) is set, call CMemoryAllocator::GetTotalMemoryAllocated (0x009F2450) on
// it and drop the result (the same pattern as CDefinitionManager::SaveBinaryDefinitions 0x009AF7A0, engine/CMemoryAllocator.h); CGameDefinitionManager::Get() (0x0044C6B0, static
// __fastcall) -> virtual slot 2 (+8; taken as InitAndCompile, Ego_r.pdb ?InitAndCompile@CGameDefinitionManager@@UAEXXZ);
// if (flag) Get() -> ClearSymbols 0x009ACB10 (CDefinitionManager, Ego_r ?ClearSymbols@CDefinitionManager@@QAEXXZ; naming wave 24,
// labels_chunk691); the global call again; return true. CGameDefinitionManager derives from CDefinitionManager (Ego_r).
// NF42 (round 5): the CGlobalService placeholder replaced by the shared engine/CMemoryAllocator.h (protected static
// PMemoryAllocator through the public inline Get(); Ego_r / FableWin ?GetTotalMemoryAllocated@CMemoryAllocator@@QAEKXZ).
// Opt: size TU (unaligned entry).
#pragma optimize("s", on)
#define FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE
#define FABLE_RETAIL_CMEMORYALLOCATOR_SAVEBINARY_PASS23
#include "rebuild_abi.h"
#include "engine/CMemoryAllocator.h"
class CDefinitionManager {
public:
    virtual ~CDefinitionManager();
    virtual void _slot01();
    virtual void InitAndCompile();                       // slot 2 (+8)
    void ClearSymbols();                                 // 0x009ACB10
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();     // 0x0044C6B0
    virtual void InitAndCompile();                       // slot 2 (+8)
};
class CMainGameComponent {
protected:
    bool InitDefinitions(bool flag);
    friend struct CMainGameComponent_Test;               // behaviour-test hook
};
bool CMainGameComponent::InitDefinitions(bool flag)
{
    if (CMemoryAllocator::Get())
        CMemoryAllocator::Get()->GetTotalMemoryAllocated();
    CGameDefinitionManager::Get().InitAndCompile();
    if (flag)
        CGameDefinitionManager::Get().ClearSymbols();
    if (CMemoryAllocator::Get())
        CMemoryAllocator::Get()->GetTotalMemoryAllocated();
    return true;
}