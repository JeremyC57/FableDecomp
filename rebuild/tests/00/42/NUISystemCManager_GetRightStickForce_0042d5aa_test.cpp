// Behaviour test for 0x0042d5aa (NF230, q12624) on the shared header engine/NUISystem_CManager.h: NUISystem::CManager::GetRightStickForce, the row's
// body under the row's defines, on the header class, called non-virtually on raw manager storage whose touched members are built in
// place (a never-constructed test subclass names the protected members). One case per condition in the row. Replaces the HEAD host model.

#include <stdio.h>
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
C2DVector& CManager::GetRightStickForce()
{
    return RightStickForce;
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
        (g_mgr->*rsf).X = 0.25f; (g_mgr->*rsf).Y = -2.0f;
        C2DVector& got = g_mgr->NUISystem::CManager::GetRightStickForce();
        Check(&got == &(g_mgr->*rsf), "returns a reference to the member itself");
        Check(got.X == 0.25f && got.Y == -2.0f, "reads the stored force");
        got.X = 7.0f;
        Check((g_mgr->*rsf).X == 7.0f, "writes through the reference reach the member");
    }
    printf(g_fails ? "FAIL_0042d5aa FAIL\n" : "PASS_0042d5aa\n");
    return g_fails ? 1 : 0;
}
