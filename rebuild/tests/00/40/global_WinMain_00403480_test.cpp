#include <cstdio>
#include <cstring>

// Model of 0x00403480 WinMain: if the named "Global\Fable: The Lost Chapters" mutex already exists, quit (0);
// otherwise create it, carve the 0x32000-byte micro-thread stack out of the WinMain frame (top = end of the
// 0x32001-byte buffer, bottom = its start), hand it to MicroThread::SetStack and run GFMain. Win32 calls are stubs.
static bool g_mutexExists = false;
static int g_created = 0;
static int g_mainCalls = 0;
static unsigned char* g_setTop = 0;
static unsigned long g_setSize = 0;
static int g_mainShow = 0;
static const char* g_mainCommandLine = 0;
static void* g_mainInstance = 0;
static const char* g_lastMutexName = 0;
static void* StubOpenMutex(unsigned long access, int inherit, const char* name)
{
    g_lastMutexName = name;
    return (access == 0x001F0001 && inherit == 0 && g_mutexExists) ? (void*)&g_mutexExists : 0;
}
static void* StubCreateMutex(void*, int, const char* name) { ++g_created; g_lastMutexName = name; return &g_created; }
static void SetStack(unsigned char* top, unsigned long size) { g_setTop = top; g_setSize = size; }
static long GFMain(void* instance, char* commandLine, int show)
{
    ++g_mainCalls; g_mainInstance = instance; g_mainCommandLine = commandLine; g_mainShow = show;
    return 0;
}
static unsigned char* g_MicroThreadStackTop = 0;
static unsigned char* g_MicroThreadStackBottom = 0;
struct SWinMainStackFrame {
    unsigned char m_alignmentPad[3];
    unsigned char m_microThreadStack[0x32001];
};
static unsigned char* g_frameStack = 0;

static int WinMainModel(void* instance, void*, char* commandLine, int showCommand)
{
    SWinMainStackFrame frame;
    g_frameStack = frame.m_microThreadStack;
    if (StubOpenMutex(0x001F0001, 0, "Global\\Fable: The Lost Chapters") == 0) {
        StubCreateMutex(0, 0, "Global\\Fable: The Lost Chapters");
        g_MicroThreadStackTop = frame.m_microThreadStack + sizeof(frame.m_microThreadStack);
        g_MicroThreadStackBottom = frame.m_microThreadStack;
        SetStack(g_MicroThreadStackTop, 0x32000);
        GFMain(instance, commandLine, showCommand);
    }
    return 0;
}

static int Fail()
{
    std::printf("00403480_TEST FAIL\n");
    return 1;
}

int main()
{
    static char commandLine[] = "-nointro";
    int instance = 0;
    if (WinMainModel(&instance, 0, commandLine, 5) != 0)
        return Fail();
    if (g_created != 1 || g_mainCalls != 1 || g_mainInstance != &instance || g_mainCommandLine != commandLine || g_mainShow != 5)
        return Fail();
    if (std::strcmp(g_lastMutexName, "Global\\Fable: The Lost Chapters") != 0)
        return Fail();
    if (g_MicroThreadStackBottom != g_frameStack || g_MicroThreadStackTop != g_frameStack + 0x32001 ||
        g_setTop != g_MicroThreadStackTop || g_setSize != 0x32000)
        return Fail();
    g_mutexExists = true;                          // second instance: nothing runs
    if (WinMainModel(&instance, 0, commandLine, 1) != 0 || g_created != 1 || g_mainCalls != 1)
        return Fail();
    std::printf("00403480_TEST PASS\n");
    return 0;
}