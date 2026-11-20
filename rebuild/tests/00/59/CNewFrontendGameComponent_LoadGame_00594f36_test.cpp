// Behaviour test for CNewFrontendGameComponent::LoadGame 0x00594F36 (NF129: the row now uses engine/CNewFrontendGameComponent.h and
// fable_string.h, so the test drives the shared classes). The component is zeroed raw storage with a fake vtable whose slot 4
// (CGameComponent::SetQuit) records the call; CWideString::operator=(const CWideString&) (0x0099B7D0) is defined here as a probe.
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RUN
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_STARTGAME
#include "fable_string.h"
#include "engine/CNewFrontendGameComponent.h"

static unsigned char g_Component[0x100];
static int g_Sequence;
static int g_QuitSequence;
static int g_AssignSequence;
static void* g_QuitThis;
static const void* g_AssignDestination;
static const void* g_AssignSource;
static unsigned char g_FlagSeenAtQuit;
static unsigned char g_FlagSeenAtAssign;

static void __fastcall ProbeSetQuit(void* self, void*)
{
    g_QuitSequence = ++g_Sequence;
    g_QuitThis = self;
    g_FlagSeenAtQuit = static_cast<unsigned char*>(self)[0x29];
}

const CWideString& CWideString::operator=(const CWideString& other)
{
    g_AssignSequence = ++g_Sequence;
    g_AssignDestination = this;
    g_AssignSource = &other;
    g_FlagSeenAtAssign = g_Component[0x29];
    return *this;
}

int main()
{
    void* vtable[8];
    std::memset(vtable, 0, sizeof(vtable));
    vtable[4] = (void*)&ProbeSetQuit;
    std::memset(g_Component, 0, sizeof(g_Component));
    *(void***)g_Component = vtable;
    unsigned char requestedStorage[0x10];
    std::memset(requestedStorage, 0, sizeof(requestedStorage));
    CNewFrontendGameComponent* component = reinterpret_cast<CNewFrontendGameComponent*>(g_Component);
    CWideString& requested = *reinterpret_cast<CWideString*>(requestedStorage);

    component->LoadGame(requested);
    if (!(g_QuitSequence == 1 && g_AssignSequence == 2 && g_QuitThis == g_Component &&
          g_AssignDestination == g_Component + 0x84 && g_AssignSource == requestedStorage &&
          g_FlagSeenAtQuit == 0 && g_FlagSeenAtAssign == 1 && g_Component[0x29] == 1))
    {
        std::puts("FRONTEND_00594f36_TEST FAIL");
        return 1;
    }
    std::puts("FRONTEND_00594f36_TEST PASS");
    return 0;
}
