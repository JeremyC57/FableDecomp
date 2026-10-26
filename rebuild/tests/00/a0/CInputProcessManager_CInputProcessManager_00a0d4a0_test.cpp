#include <cstdio>
#include <cstring>
static const bool g_is32 = sizeof(void*) == 4 && sizeof(long) == 4;   // the layout checks hold for the 32-bit VC7.1 build
static long g_events = 0, g_base = 0;
class CGameEvent { public: long Type; signed char Player; unsigned char Data[32]; unsigned char EndPos; bool Valid; bool Replacement; CGameEvent() { ++g_events; Type = 77; } };
class CProcessedInput { public: long Player; long Type; CGameEvent GameEvents[4]; unsigned char NoGameEvents; long Priority; CProcessedInput() { } };
struct CLinkedList_CAInputProcess { void* Head; void* Tail; long NoEntries; void* FirstScanInfo; CLinkedList_CAInputProcess() : Head(0), Tail(0), NoEntries(0), FirstScanInfo(0) { } };
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
class CInputProcessManager : public CBase { public: virtual void v00() { } CLinkedList_CAInputProcess InputProcessList; CProcessedInput QueuedProcessedInputs[10]; long NoQueuedProcessedInputs; CInputProcessManager() : CBase(), InputProcessList() { NoQueuedProcessedInputs = 0; } };
int main()
{
    CInputProcessManager* m = new CInputProcessManager();
    if (g_base != 1 || g_events != 40 || m->NoQueuedProcessedInputs != 0 || m->InputProcessList.Head || m->InputProcessList.Tail || m->InputProcessList.NoEntries || m->InputProcessList.FirstScanInfo || m->QueuedProcessedInputs[9].GameEvents[3].Type != 77
        || (g_is32 && sizeof(CProcessedInput) != 0xb0) || (g_is32 && (char*)&m->NoQueuedProcessedInputs - (char*)m != 0x6f4)) { std::printf("00a0d4a0_TEST FAIL\n"); return 1; }
    delete m;
    std::printf("00a0d4a0_TEST PASS\n");
    return 0;
}