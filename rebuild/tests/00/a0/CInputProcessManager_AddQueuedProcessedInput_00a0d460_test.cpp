#include <cstdio>
#include <cstring>
static const bool g_is32 = sizeof(void*) == 4 && sizeof(long) == 4;   // the layout checks hold for the 32-bit VC7.1 build
class CProcessedInput { public: long Player; long Type; unsigned char GameEvents[0xa0]; unsigned char NoGameEvents; unsigned char _pad[3]; long Priority; };
class CInputProcessManager { public: virtual void v00() { } unsigned char InputProcessList[0x10]; CProcessedInput QueuedProcessedInputs[10]; long NoQueuedProcessedInputs;
    void AddQueuedProcessedInput(const CProcessedInput& input) { if (NoQueuedProcessedInputs >= 0 && NoQueuedProcessedInputs < 10) { QueuedProcessedInputs[NoQueuedProcessedInputs] = input; ++NoQueuedProcessedInputs; } } };
int main()
{
    CInputProcessManager m; std::memset(m.QueuedProcessedInputs, 0, sizeof m.QueuedProcessedInputs); m.NoQueuedProcessedInputs = 0; CProcessedInput in; std::memset(&in, 0, sizeof in); in.Player = 3; in.Priority = 9; in.GameEvents[0x9f] = 0x5a;
    for (int i = 0; i < 11; ++i) m.AddQueuedProcessedInput(in);
    if (m.NoQueuedProcessedInputs != 10 || std::memcmp(&m.QueuedProcessedInputs[9], &in, sizeof in) || std::memcmp(&m.QueuedProcessedInputs[0], &in, sizeof in) || (g_is32 && (char*)&m.NoQueuedProcessedInputs - (char*)&m != 0x6f4)) { std::printf("00a0d460_TEST FAIL\n"); return 1; }
    m.NoQueuedProcessedInputs = -1; m.AddQueuedProcessedInput(in); if (m.NoQueuedProcessedInputs != -1) { std::printf("00a0d460_TEST FAIL\n"); return 1; }
    std::printf("00a0d460_TEST PASS\n");
    return 0;
}