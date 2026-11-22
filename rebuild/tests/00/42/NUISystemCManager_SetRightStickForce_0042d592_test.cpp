// Behaviour test for 0x0042d592 (NF230, q12624) on the shared header engine/NUISystem_CManager.h: NUISystem::CManager::SetRightStickForce, the row's
// body under the row's defines, on the header class, called non-virtually on raw manager storage whose touched members are built in
// place (a never-constructed test subclass names the protected members). One case per condition in the row. Replaces the HEAD host model.

#include <stdio.h>
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
void CManager::SetRightStickForce(C2DVector& force)
{
    RightStickForce = force;
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
        C2DVector NUISystem::CManager::* rsf = CManagerPeek::RightStickForceMember();
        unsigned char* raw = (unsigned char*)&(g_mgr->*rsf);
        for (int i = -4; i < 12; ++i) raw[i] = 0xA5;
        C2DVector force(1.5f, -0.5f);
        g_mgr->NUISystem::CManager::SetRightStickForce(force);
        Check((g_mgr->*rsf).X == 1.5f && (g_mgr->*rsf).Y == -0.5f, "both components copied");
        Check(force.X == 1.5f && force.Y == -0.5f, "the argument is unchanged");
        Check(raw[-1] == 0xA5 && raw[8] == 0xA5, "only the 8 bytes of the member are written");
    }
    printf(g_fails ? "0042d592_TEST FAIL\n" : "0042d592_TEST PASS\n");
    return g_fails ? 1 : 0;
}
