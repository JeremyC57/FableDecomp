// Behaviour test for 0x0042d4f0 (NF230, q12624) on the shared header engine/NUISystem_CManager.h: NUISystem::CManager::GetMouseDisabled, the row's
// body under the row's defines, on the header class, called non-virtually on raw manager storage whose touched members are built in
// place (a never-constructed test subclass names the protected members). One case per condition in the row. Replaces the HEAD host model.

#include <stdio.h>
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
bool CManager::GetMouseDisabled()
{
    return MouseDisabled;
}
}
// Test-only access to the protected members through a derived class (never constructed: the members a case needs are built in place).
struct CManagerPeek : NUISystem::CManager {
    static bool NUISystem::CManager::* MouseDisabledMember() { return &CManagerPeek::MouseDisabled; }
    static C2DVector NUISystem::CManager::* RightStickForceMember() { return &CManagerPeek::RightStickForce; }
    static std::map<long,EEngine2DLayer> NUISystem::CManager::* LayersMember() { return &CManagerPeek::Layers; }
    static CCountedPointer<NUISystem::CComponent> NUISystem::CManager::* BastardChildMember() { return &CManagerPeek::BastardChild; }
    static std::map<CCharString,CCountedPointer<NUISystem::CComponent> > NUISystem::CManager::* BastardChildrenMember() { return &CManagerPeek::BastardChildren; }
};
static double g_store[(sizeof(NUISystem::CManager) + 7) / 8];   // raw storage for the manager (its constructor is protected and out of line)
static NUISystem::CManager* const g_mgr = (NUISystem::CManager*)g_store;
static int g_fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); ++g_fails; } }

int main()
{
    {
        bool NUISystem::CManager::* md = CManagerPeek::MouseDisabledMember();
        g_mgr->*md = true;
        Check(g_mgr->NUISystem::CManager::GetMouseDisabled() == true, "MouseDisabled set: true");
        g_mgr->*md = false;
        Check(g_mgr->NUISystem::CManager::GetMouseDisabled() == false, "MouseDisabled clear: false");
    }
    printf(g_fails ? "FAIL_0042d4f0 FAIL\n" : "PASS_0042d4f0\n");
    return g_fails ? 1 : 0;
}
