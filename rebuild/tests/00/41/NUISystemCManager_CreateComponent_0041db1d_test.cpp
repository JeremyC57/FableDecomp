// Behaviour test for NUISystem::CManager::CreateComponent(const CCharString&, bool) (0x0041DB1D): the def index comes from
// the startup definitions when asked, else from the manager's own definition manager, else from the game's; the
// component created from it is named and returned.
#include <cstdio>
#include <cstring>

class CCharString { public: const char* Text; };

class CDefinitionManager { public:
    long Base;
    long GetDefGlobalIndexFromName(const CCharString& name) const { return Base + (long)std::strlen(name.Text); }
};

static CDefinitionManager g_startup = { 100 };
static CDefinitionManager g_game = { 200 };

struct CComponent {
    long DefIndex;
    const char* Name;
    void SetName(const CCharString& name) { Name = name.Text; }
};

struct CManager {
    CDefinitionManager* m_pDefinitionManager;
    CComponent Pool[4];
    int Used;
    CComponent* CreateComponent(long defIndex)
    {
        CComponent* c = &Pool[Used++];
        c->DefIndex = defIndex;
        c->Name = 0;
        return c;
    }
    CComponent* CreateComponent(const CCharString& name, bool startupDefs)
    {
        CComponent* component;
        if (startupDefs)
            component = CreateComponent(g_startup.GetDefGlobalIndexFromName(name));
        else if (m_pDefinitionManager)
            component = CreateComponent(m_pDefinitionManager->GetDefGlobalIndexFromName(name));
        else
            component = CreateComponent(g_game.GetDefGlobalIndexFromName(name));
        component->SetName(name);
        return component;
    }
};

int main()
{
    CDefinitionManager own = { 300 };
    CManager m;
    m.Used = 0;
    m.m_pDefinitionManager = &own;
    CCharString name = { "BUTTON" };
    CComponent* a = m.CreateComponent(name, true);
    CComponent* b = m.CreateComponent(name, false);
    m.m_pDefinitionManager = 0;
    CComponent* c = m.CreateComponent(name, false);
    if (a->DefIndex != 106 || b->DefIndex != 306 || c->DefIndex != 206 ||
        a->Name != name.Text || b->Name != name.Text || c->Name != name.Text || m.Used != 3)
    {
        std::printf("0041db1d_TEST FAIL\n");
        return 1;
    }
    std::printf("0041db1d_TEST PASS\n");
    return 0;
}