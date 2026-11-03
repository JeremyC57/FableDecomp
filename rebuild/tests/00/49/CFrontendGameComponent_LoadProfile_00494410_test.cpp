#include <cstdio>

// Self-contained copy of the 0x00494410 body (CFrontendGameComponent::LoadProfile) with stub callees.
class CWideString { public: int Id; };
class CUserProfileManager { public:
    static CUserProfileManager& Get();
    void SetProfileName(const CWideString& profileName, bool loadProfile);
};
class CFrontendGameComponent {
private:
    void LoadProfile(const CWideString& profileName, bool loadProfile);
public:
    void TestLoadProfile(const CWideString& profileName, bool loadProfile) { LoadProfile(profileName, loadProfile); }
};
void CFrontendGameComponent::LoadProfile(const CWideString& profileName, bool loadProfile)
{
    CUserProfileManager::Get().SetProfileName(profileName, loadProfile);
}
static CUserProfileManager g_Manager;
static unsigned long g_GetCalls;
static unsigned long g_SetCalls;
static const CWideString* g_Name;
static bool g_Load;
static CUserProfileManager* g_SetThis;

CUserProfileManager& CUserProfileManager::Get()
{
    ++g_GetCalls;
    return g_Manager;
}

void CUserProfileManager::SetProfileName(const CWideString& profileName, bool loadProfile)
{
    ++g_SetCalls;
    g_Name = &profileName;
    g_Load = loadProfile;
    g_SetThis = this;
}

int main()
{
    CFrontendGameComponent component;
    CWideString name = { 7 };
    component.TestLoadProfile(name, true);
    bool ok = g_GetCalls == 1 && g_SetCalls == 1 && g_Name == &name && g_Load && g_SetThis == &g_Manager;
    component.TestLoadProfile(name, false);
    ok = ok && g_SetCalls == 2 && !g_Load;
    if (!ok) {
        std::printf("global_DispatchFrontendInputAction_00494410_test FAIL\n");
        return 1;
    }
    std::printf("global_DispatchFrontendInputAction_00494410_test PASS\n");
    return 0;
}
