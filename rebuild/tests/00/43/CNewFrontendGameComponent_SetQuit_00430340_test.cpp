// Behaviour test for 0x00430340 (NF231, q12625) on the shared header engine/CNewFrontendGameComponent.h: CNewFrontendGameComponent::SetQuit,
// the row's body under the row's define, on the header class, called non-virtually on raw component storage (its constructor is out of
// line); a never-constructed test subclass names the protected CGameComponent members. Replaces the HEAD print-only test.

#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_SETQUIT
#include "engine/CNewFrontendGameComponent.h"
void CNewFrontendGameComponent::SetQuit()
{
    Quit = true;
}
// Test-only access to the protected base members through a derived class (never constructed).
struct CNewFrontendGameComponentPeek : CNewFrontendGameComponent {
    static bool CGameComponent::* QuitMember() { return &CNewFrontendGameComponentPeek::Quit; }
    static bool CGameComponent::* RunningMember() { return &CNewFrontendGameComponentPeek::Running; }
};
static double g_store[(sizeof(CNewFrontendGameComponent) + 7) / 8];
static CNewFrontendGameComponent* const g_comp = (CNewFrontendGameComponent*)g_store;
static int g_fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); ++g_fails; } }

int main()
{
    bool CGameComponent::* quit = CNewFrontendGameComponentPeek::QuitMember();
    bool CGameComponent::* running = CNewFrontendGameComponentPeek::RunningMember();
    {   // Quit clear: SetQuit raises it, and only the byte at +0x08
        memset(g_store, 0xAB, sizeof(g_store));
        g_comp->*quit = false;
        g_comp->*running = true;
        g_comp->CNewFrontendGameComponent::SetQuit();
        Check(g_comp->*quit == true, "Quit raised");
        Check(((unsigned char*)g_store)[8] == 1, "Quit is the byte at +0x08");
        Check(g_comp->*running == true, "Running untouched");
        Check(((unsigned char*)g_store)[7] == 0xAB && ((unsigned char*)g_store)[10] == 0xAB, "neighbouring bytes untouched");
    }
    {   // Quit already set: stays set
        g_comp->*quit = true;
        g_comp->CNewFrontendGameComponent::SetQuit();
        Check(g_comp->*quit == true, "Quit stays raised");
    }
    printf(g_fails ? "FAIL_00430340 FAIL\n" : "PASS_00430340\n");
    return g_fails ? 1 : 0;
}
