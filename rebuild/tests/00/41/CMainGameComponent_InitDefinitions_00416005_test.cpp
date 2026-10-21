#include <cstdio>
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

static char g_log[16]; static int g_n = 0;
void CGlobalService::Step() { g_log[g_n++] = 'S'; }
CGlobalService* g_pGlobalService_13CAA90 = 0;
CGameDefinitionManager::~CGameDefinitionManager() {}
void CGameDefinitionManager::_slot01() {}
void CGameDefinitionManager::InitAndCompile() { g_log[g_n++] = 'I'; }
void CGameDefinitionManager::FollowUp_9ACB10() { g_log[g_n++] = 'F'; }
static CGameDefinitionManager* g_dm = 0;
CGameDefinitionManager& __fastcall CGameDefinitionManager::Get() { return *g_dm; }
struct CMainGameComponent_Test { static bool Run(CMainGameComponent& c, bool f) { return c.InitDefinitions(f); } };
int main()
{
    g_dm = new CGameDefinitionManager; static CGlobalService svc; CMainGameComponent c;
    bool ok = CMainGameComponent_Test::Run(c, false) && g_n == 1 && g_log[0] == 'I';
    g_n = 0; g_pGlobalService_13CAA90 = &svc;
    ok = ok && CMainGameComponent_Test::Run(c, true) && g_n == 4 && g_log[0] == 'S' && g_log[1] == 'I' && g_log[2] == 'F' && g_log[3] == 'S';
    if (ok) std::printf("00416005_TEST PASS\n");
    return 0;
}