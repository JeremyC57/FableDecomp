// Real C++ member-method ABI fixture for
// NUISystem::CFrontEndManager::CreateComponent @ 0x00594F94.

#include <cstdio>

class CCharString
{
};

namespace NUISystem
{
class CComponent
{
};

class CManager
{
public:
    static CManager* __fastcall GetInstance();      // 0x0041E5F2
    CComponent* CreateComponent(const CCharString& definition, bool frontEnd);   // 0x0041DB1D
};

class CFrontEndManager : public CManager
{
public:
    CComponent* CreateComponent(const CCharString& definition);
};
}

static NUISystem::CFrontEndManager g_manager;
static NUISystem::CComponent g_component;
static int g_getCalls;
static int g_createCalls;
static const CCharString* g_definition;
static bool g_frontEnd;

NUISystem::CManager* __fastcall
NUISystem::CManager::GetInstance()
{
    ++g_getCalls;
    return &g_manager;
}

NUISystem::CComponent*
NUISystem::CManager::CreateComponent(
    const CCharString& definition,
    bool frontEnd)
{
    ++g_createCalls;
    g_definition = &definition;
    g_frontEnd = frontEnd;
    return &g_component;
}

int main()
{
    CCharString definition;
    g_getCalls = 0;
    g_createCalls = 0;
    g_definition = 0;
    g_frontEnd = false;

    NUISystem::CComponent* result =
        g_manager.CreateComponent(definition);

    if (result != &g_component ||
        g_getCalls != 1 ||
        g_createCalls != 1 ||
        g_definition != &definition ||
        !g_frontEnd)
    {
        std::printf(
            "FSE2_00594f94_TEST FAIL result=%d get=%d create=%d "
            "definition=%d frontEnd=%d\n",
            result == &g_component ? 1 : 0,
            g_getCalls,
            g_createCalls,
            g_definition == &definition ? 1 : 0,
            g_frontEnd ? 1 : 0);
        return 1;
    }
    std::puts("FSE2_00594f94_TEST PASS");
    return 0;
}
