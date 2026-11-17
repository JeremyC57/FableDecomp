// Behaviour test for 0x005952C3 NUISystem::CFrontEndManager::ChangeStateFirstTime (NF79 round 7), on the row's own shared headers
// (engine/CFrontEndManager.h full-layout branch: m_stackScreens is the STLport stack<CComponent*> at +0x20). The row object imports the
// out-of-line std::deque<NUISystem::CComponent*>::back (0x0059B039), supplied here as an explicit specialization that checks it is called
// on the manager's stack (+0x20) and hands back the probe component; the probe's vtable slot 48 (+0xC0, ChangeState(unsigned long))
// records the state. Keeps the catalog PassPattern FSE2_005952c3_TEST.
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_STLP_STACK_TOP_POP
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CFRONTENDMANAGER_DESTRUCTOR
#define FABLE_RETAIL_CFRONTENDMANAGER_STACKSCREENS
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "stlport/fable_deque.h"
#include "fable_string.h"
#include "engine/CFrontEndManager.h"

struct ComponentProbe { void** vtable; long state; int stateCalls; };

static void* g_expectedStack;
static NUISystem::CComponent* g_topComponent;
static int g_topCalls;
static void* g_vtable[0xc4 / 4];

template <> std::deque<NUISystem::CComponent*>::reference std::deque<NUISystem::CComponent*>::back()
{
    if ((void*)this == g_expectedStack)
        ++g_topCalls;
    return g_topComponent;
}

static void __fastcall ComponentChangeState(void* object, void*, unsigned long state)
{
    ComponentProbe* component = (ComponentProbe*)object;
    component->state = (long)state;
    ++component->stateCalls;
}

int main()
{
    static unsigned long storage[sizeof(NUISystem::CFrontEndManager) / 4];
    ComponentProbe component;
    std::memset(storage, 0, sizeof(storage));
    std::memset(&component, 0, sizeof(component));
    std::memset(g_vtable, 0, sizeof(g_vtable));
    g_vtable[0xc0 / 4] = (void*)ComponentChangeState;
    component.vtable = g_vtable;
    component.state = -1;
    NUISystem::CFrontEndManager* manager = reinterpret_cast<NUISystem::CFrontEndManager*>(storage);
    g_expectedStack = (unsigned char*)storage + 0x20;
    g_topComponent = reinterpret_cast<NUISystem::CComponent*>(&component);
    g_topCalls = 0;

    manager->ChangeStateFirstTime();

    if (g_topCalls != 1 || component.state != 5 || component.stateCalls != 1) {
        std::printf("FSE2_005952c3_TEST FAIL top=%d state=%ld calls=%d\n", g_topCalls, component.state, component.stateCalls);
        return 1;
    }
    std::puts("FSE2_005952c3_TEST PASS");
    return 0;
}
