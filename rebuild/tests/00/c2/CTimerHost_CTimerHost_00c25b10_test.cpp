// Behaviour test for 0x00C25B10 CTimerHost::CTimerHost: the timer part gets (rate, arg, target) with the shutdown flag
// clear, the worker thread is created over the host with the caller's parameters, and a failed allocation traces "EMEM".
#include <cstdio>
#include <cstring>
#include <new>

static char g_lastLog[128]; static int g_breaks; static bool g_failAlloc;
struct CString {
    char m_buf[128];
    CString() { m_buf[0] = 0; }
    CString& operator<<(const char* t) { std::strcat(m_buf, t); return *this; }
    CString& operator<<(long v) { char b[32]; std::sprintf(b, "%ld", v); std::strcat(m_buf, b); return *this; }
};
static void AssertHook(const char* m) { std::strcpy(g_lastLog, m); }
static void (*g_pfnAssertHook)(const char*) = AssertHook;
struct CDebugBreak { CDebugBreak() { ++g_breaks; } };

struct SThreadParams { unsigned int m_stackSize; int m_priority; };
struct CWorkerThread {
    void* m_pOwner; const SThreadParams* m_pParams;
    CWorkerThread(void* owner, const SThreadParams* params) : m_pOwner(owner), m_pParams(params) {}
    static void* operator new(size_t n) throw() { return g_failAlloc ? 0 : ::operator new(n); }
};
struct PKCTimerTarget { int ticks; };
struct PKCTimerManager {
    bool m_bShutdown; PKCTimerTarget* m_pTarget; float m_rate; int m_10;
    PKCTimerManager(float rate, int arg, PKCTimerTarget* target)
    { m_pTarget = target; m_bShutdown = false; m_rate = rate; m_10 = arg; }
};
struct CTimerHost {
    PKCTimerManager m_timer; CWorkerThread* m_pThread;
    CTimerHost(float rate, int arg, PKCTimerTarget* target, const SThreadParams* params)
        : m_timer(rate, arg, target), m_pThread(0)
    {
        m_pThread = new CWorkerThread(this, params);
        if (!m_pThread) {
            {
                CString line;
                line << ".\\PKCTimerManager.cpp" << "(" << (long)10 << ") : " << "EMEM" << "\n";
                if (g_pfnAssertHook)
                    g_pfnAssertHook(line.m_buf);
            }
            CDebugBreak();
        }
    }
};

int main()
{
    int bad = 0;
    PKCTimerTarget target = { 0 };
    SThreadParams params = { 0x3000, 15 };
    CTimerHost h(50.0f, 0, &target, &params);
    if (h.m_timer.m_rate != 50.0f || h.m_timer.m_10 != 0 || h.m_timer.m_pTarget != &target || h.m_timer.m_bShutdown) bad = 1;
    if (!h.m_pThread || h.m_pThread->m_pOwner != (void*)&h || h.m_pThread->m_pParams != &params || g_breaks) bad = 1;
    delete h.m_pThread;
    g_failAlloc = true;
    CTimerHost f(25.0f, 3, &target, 0);
    if (f.m_pThread || g_breaks != 1 || f.m_timer.m_10 != 3 || std::strcmp(g_lastLog, ".\\PKCTimerManager.cpp(10) : EMEM\n")) bad = 1;
    if (bad) { std::printf("00c25b10_TEST FAIL\n"); return 1; }
    std::printf("00c25b10_TEST PASS\n");
    return 0;
}