// test for 0041732a InitPlayerManager
#include <stdio.h>
class CMainGameComponent;
class CGameDefinitionManager {
public:
    static const CGameDefinitionManager& Get();          // 0x0044C6B0
};
class CPlayerManager {
public:
    unsigned char _body[0x2c];
    CPlayerManager(CMainGameComponent& component, const CGameDefinitionManager& defs);   // 0x0044A3B0
};
template <class T> class scoped_ptr {
public:
    T* Ptr;
    void reset(T* p);                                    // 0x004193A0
};
class CMainGameComponent {
public:
    unsigned char              _pad00[0x1c];
    scoped_ptr<CPlayerManager> PPlayerManager;           // +0x1c
protected:
    void InitPlayerManager();
};
void CMainGameComponent::InitPlayerManager()
{
    PPlayerManager.reset(new CPlayerManager(*this, CGameDefinitionManager::Get()));
}

static CGameDefinitionManager g_defs; static CMainGameComponent* g_comp; static const CGameDefinitionManager* g_d; static CPlayerManager* g_new;
const CGameDefinitionManager& CGameDefinitionManager::Get() { return g_defs; }
CPlayerManager::CPlayerManager(CMainGameComponent& c, const CGameDefinitionManager& d) { g_comp = &c; g_d = &d; g_new = this; }
template <class T> void scoped_ptr<T>::reset(T* p) { Ptr = p; }
struct CMainGameComponent_T : CMainGameComponent { void Run() { InitPlayerManager(); } };
int main()
{
    static CMainGameComponent_T m; m.PPlayerManager.Ptr = 0;
    m.Run();
    int ok = g_comp == &m && g_d == &g_defs && m.PPlayerManager.Ptr == g_new && g_new != 0;
    printf(ok ? "0041732a_TEST PASS\n" : "0041732a_TEST FAIL\n");
    return 0;
}