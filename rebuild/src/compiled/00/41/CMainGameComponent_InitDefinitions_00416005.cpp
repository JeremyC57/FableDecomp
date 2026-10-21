// CMainGameComponent::InitDefinitions(bool flag) -- bracket the definition manager's init/compile with two calls on a global service, optionally run a follow-up step
// Leaf: ?InitDefinitions@CMainGameComponent@@IAE_N_N@Z
// Retail: Fable.exe @ 0x00416005, 66 bytes, `ret 4` (thiscall, `this` unused); one caller (0x004185D9); Ego_r.pdb
// ?InitDefinitions@CMainGameComponent@@IAE_N_N@Z.
// Body: if the global at 0x013CAA90 is set, call 0x009F2450 on it; CGameDefinitionManager::Get() (0x0044C6B0, static
// __fastcall) -> virtual slot 2 (+8; taken as InitAndCompile, Ego_r.pdb ?InitAndCompile@CGameDefinitionManager@@UAEXXZ);
// if (flag) Get() -> 0x009ACB10 (forwards to its +0x58 member); the global call again; return true.
// The global's class and both non-virtual callees are not identified; their names here are placeholders.
// Opt: size TU (unaligned entry).
#pragma optimize("s", on)
class CGlobalService {                                   // object behind 0x013CAA90 (class not identified)
public:
    void Step();                                         // 0x009F2450
};
extern CGlobalService* g_pGlobalService_13CAA90;         // 0x013CAA90
class CGameDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();     // 0x0044C6B0
    virtual ~CGameDefinitionManager();
    virtual void _slot01();
    virtual void InitAndCompile();                       // slot 2 (+8)
    void FollowUp_9ACB10();                              // 0x009ACB10
};
class CMainGameComponent {
protected:
    bool InitDefinitions(bool flag);
    friend struct CMainGameComponent_Test;               // behaviour-test hook
};
bool CMainGameComponent::InitDefinitions(bool flag)
{
    if (g_pGlobalService_13CAA90)
        g_pGlobalService_13CAA90->Step();
    CGameDefinitionManager::Get().InitAndCompile();
    if (flag)
        CGameDefinitionManager::Get().FollowUp_9ACB10();
    if (g_pGlobalService_13CAA90)
        g_pGlobalService_13CAA90->Step();
    return true;
}