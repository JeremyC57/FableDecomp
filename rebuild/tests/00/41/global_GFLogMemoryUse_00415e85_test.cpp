// Behaviour test for 0x00415E85 GFLogMemoryUse: nothing while logging is off; with logging on, the first call (last time
// negative) and any call more than 300 s after the previous log record the time, read the memory status and dump
// Memlog%4d.log with a rising index; calls inside the 300 s window do nothing.
#include <cstdio>
#include <cstring>

static double g_now;
static int g_statusReads = 0, g_dumps = 0;
static char g_lastDump[64];
static float g_lastUsedMB;
struct MemStatus { unsigned long dwLength, dwMemoryLoad, dwTotalPhys, dwAvailPhys, a, b, c, d; };
static void GlobalMemoryStatus_(MemStatus* s) { ++g_statusReads; s->dwTotalPhys = 512u << 20; s->dwAvailPhys = 200u << 20; }
static double GFGetTime() { return g_now; }
static void DumpLog(const char* name) { ++g_dumps; std::strcpy(g_lastDump, name); }

static bool   g_bLogMemoryUse;
static long   g_MemLogIndex;
static double g_LastMemLogTime = -1.0;

// the function under test (shape of 0x00415E85; the unused text line omitted)
static void GFLogMemoryUse()
{
    if (!g_bLogMemoryUse)
        return;
    double now = GFGetTime();
    if (g_LastMemLogTime < 0.0 || now - g_LastMemLogTime > 300.0) {
        g_LastMemLogTime = now;
        MemStatus status;
        GlobalMemoryStatus_(&status);
        float usedMB = (float)(status.dwTotalPhys - status.dwAvailPhys) / (1024.0f * 1024.0f);
        g_lastUsedMB = usedMB;
        char fileName[256];
        std::sprintf(fileName, "Memlog%4d.log", (int)g_MemLogIndex);
        DumpLog(fileName);
        ++g_MemLogIndex;
    }
}

int main()
{
    int ok = 1;
    g_bLogMemoryUse = false; g_now = 10.0;
    GFLogMemoryUse();
    if (g_dumps != 0 || g_statusReads != 0 || g_LastMemLogTime != -1.0) ok = 0;
    g_bLogMemoryUse = true;
    GFLogMemoryUse();                                   // first log (last < 0)
    if (g_dumps != 1 || std::strcmp(g_lastDump, "Memlog   0.log") != 0 || g_LastMemLogTime != 10.0 || g_lastUsedMB != 312.0f) ok = 0;
    g_now = 200.0; GFLogMemoryUse();                    // inside the window
    g_now = 310.0; GFLogMemoryUse();                    // exactly 300 s: not more than 300
    if (g_dumps != 1 || g_statusReads != 1) ok = 0;
    g_now = 310.5; GFLogMemoryUse();                    // past the window
    if (g_dumps != 2 || std::strcmp(g_lastDump, "Memlog   1.log") != 0 || g_LastMemLogTime != 310.5) ok = 0;
    g_MemLogIndex = 1234; g_now = 1000.0; GFLogMemoryUse();
    if (g_dumps != 3 || std::strcmp(g_lastDump, "Memlog1234.log") != 0 || g_MemLogIndex != 1235) ok = 0;
    std::printf(ok ? "00415e85_TEST PASS\n" : "00415e85_TEST FAIL\n");
    return ok ? 0 : 1;
}