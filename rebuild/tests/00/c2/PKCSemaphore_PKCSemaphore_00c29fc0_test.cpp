// Behaviour test for PKCSemaphore::PKCSemaphore @ 0x00C29FC0 (copy of the model; the Win32 API and the libpk report hook are fakes).
#include <cstdio>
#include <cstring>

// The libpk message stream: appends text and signed numbers to one buffer; the report hook and the debugger break are counted.
struct CDebugStream {
    char m_text[256];
    CDebugStream() { m_text[0] = 0; }
    CDebugStream& operator<<(const char* text) { std::strcat(m_text, text); return *this; }
    CDebugStream& operator<<(long value) { char b[32]; std::sprintf(b, "%ld", value); std::strcat(m_text, b); return *this; }
};
static char g_lastReport[256];
static int g_reports = 0, g_breaks = 0;
static void ReportHook(const char* text) { ++g_reports; std::strcpy(g_lastReport, text); }
static void (*g_pfnErrorReport)(const char* text) = ReportHook;
struct CDebugBreak { CDebugBreak() { ++g_breaks; } };

#define PK_REPORT_ERROR(line, text)                                                         \
    {                                                                                       \
        {                                                                                   \
            CDebugStream s;                                                                 \
            s << ".\\PKCSemaphore.cpp" << "(" << (long)(line) << ") : " << text << "\n";  \
            if (g_pfnErrorReport)                                                           \
                g_pfnErrorReport(s.m_text);                                                 \
        }                                                                                   \
        CDebugBreak();                                                                      \
    }

// Fake Win32 semaphore API: a handle is a counter block; failures are switched on by the test.
struct FakeSemaphore { long count; long maximum; };
static FakeSemaphore g_block;
static bool g_failCreate = false, g_failWait = false, g_failRelease = false;
static long g_lastInitial = -1, g_lastMaximum = -1;
static void* FakeCreateSemaphore(void*, long initialCount, long maximumCount, const char*)
{
    g_lastInitial = initialCount; g_lastMaximum = maximumCount;
    if (g_failCreate) return 0;
    g_block.count = initialCount; g_block.maximum = maximumCount;
    return &g_block;
}
static unsigned int FakeWaitForSingleObject(void* handle, unsigned int)
{
    if (g_failWait || !handle) return 0xffffffffu;
    --((FakeSemaphore*)handle)->count;
    return 0;
}
static int FakeReleaseSemaphore(void* handle, long releaseCount, long*)
{
    FakeSemaphore* s = (FakeSemaphore*)handle;
    if (g_failRelease || !s || s->count + releaseCount > s->maximum) return 0;
    s->count += releaseCount;
    return 1;
}

class PKCSemaphore {
public:
    PKCSemaphore(long initialCount, long maximumCount)
    {
        m_hSemaphore = FakeCreateSemaphore(0, initialCount, maximumCount, 0);
        if (m_hSemaphore == 0)
            PK_REPORT_ERROR(9, "Could not create semaphore");
    }
    void Wait()
    {
        if (FakeWaitForSingleObject(m_hSemaphore, 0xffffffffu) == 0xffffffffu)
            PK_REPORT_ERROR(20, "Waiting for semaphore unit failed");
    }
    void Release()
    {
        if (FakeReleaseSemaphore(m_hSemaphore, 1, 0) == 0)
            PK_REPORT_ERROR(27, "Could not put unit on semaphore");
    }
    void* m_hSemaphore;
};

int main()
{
    bool ok = true;
    PKCSemaphore a(2, 5);
    if (a.m_hSemaphore != &g_block || g_block.count != 2 || g_block.maximum != 5 || g_reports != 0 || g_breaks != 0) ok = false;
    if (g_lastInitial != 2 || g_lastMaximum != 5) ok = false;
    g_failCreate = true;
    PKCSemaphore b(0, 1);
    if (b.m_hSemaphore != 0 || g_reports != 1 || g_breaks != 1) ok = false;
    if (std::strcmp(g_lastReport, ".\\PKCSemaphore.cpp(9) : Could not create semaphore\n") != 0) ok = false;
    if (!ok) { std::printf("00c29fc0_TEST FAIL\n"); return 1; }
    std::printf("00c29fc0_TEST PASS\n");
    return 0;
}