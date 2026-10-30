// Real-object ABI/behavior fixture for
// NUISystem::CFrontEndManager::GetInstance @ 0x00595582.

#include <cstdio>

namespace NUISystem {
class CFrontEndManager {
public:
    unsigned char _body[0xe0];
    CFrontEndManager();
    static CFrontEndManager* __fastcall GetInstance();
};
}
using NUISystem::CFrontEndManager;
extern "C" CFrontEndManager* FableFrontEndManagerSingleton = 0;

static unsigned char g_storage[0xe0];
static CFrontEndManager* g_constructed = (CFrontEndManager*)g_storage;
static void* g_allocationResult;
static unsigned long g_allocatedBytes;
static int g_allocateCalls;
static void* g_constructorStorage;
static int g_constructorCalls;

void* __cdecl operator new(unsigned int bytes)
{
    ++g_allocateCalls;
    g_allocatedBytes = (unsigned long)bytes;
    return g_allocationResult;
}

CFrontEndManager::CFrontEndManager()
{
    ++g_constructorCalls;
    g_constructorStorage = this;
}

static void Reset()
{
    FableFrontEndManagerSingleton = 0;
    g_allocationResult = 0;
    g_allocatedBytes = 0;
    g_allocateCalls = 0;
    g_constructorStorage = 0;
    g_constructorCalls = 0;
}

static bool RunExisting()
{
    static unsigned char other[0xe0];
    Reset();
    FableFrontEndManagerSingleton = (CFrontEndManager*)other;
    CFrontEndManager* result = CFrontEndManager::GetInstance();
    return result == (CFrontEndManager*)other && g_allocateCalls == 0 && g_constructorCalls == 0;
}

static bool RunAllocationFailure()
{
    Reset();
    CFrontEndManager* result = CFrontEndManager::GetInstance();
    return result == 0 && FableFrontEndManagerSingleton == 0 && g_allocateCalls == 1 &&
        g_allocatedBytes == 0xe0 && g_constructorCalls == 0;
}

static bool RunConstruction()
{
    Reset();
    g_allocationResult = g_storage;
    CFrontEndManager* result = CFrontEndManager::GetInstance();
    return result == g_constructed && FableFrontEndManagerSingleton == g_constructed && g_allocateCalls == 1 &&
        g_allocatedBytes == 0xe0 && g_constructorCalls == 1 && g_constructorStorage == g_storage;
}

int main()
{
    const bool existing = RunExisting();
    const bool failed = RunAllocationFailure();
    const bool constructed = RunConstruction();
    if (!existing || !failed || !constructed)
    {
        std::printf("FSE2_00595582_TEST FAIL existing=%d failed=%d constructed=%d\n",
            existing ? 1 : 0, failed ? 1 : 0, constructed ? 1 : 0);
        return 1;
    }
    std::puts("FSE2_00595582_TEST PASS");
    return 0;
}
