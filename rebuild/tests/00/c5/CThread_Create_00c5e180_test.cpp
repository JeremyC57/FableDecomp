// Behaviour test for CThread::Create (0x00C5E180): the CreateThread arguments are routed in retail's order, the handle
// and thread id are kept, and the result says whether a handle came back.
#include <cstdio>
typedef unsigned int (*PFN_Start)(void*);
static void* g_sec; static unsigned int g_stack, g_flags; static PFN_Start g_start; static void* g_param; static void* g_result;
static void* StubCreateThread(void* security, unsigned int stackSize, PFN_Start start, void* param, unsigned int flags, unsigned int* threadId)
{
    g_sec = security; g_stack = stackSize; g_start = start; g_param = param; g_flags = flags; *threadId = 0x1234; return g_result;
}
static unsigned int Proc(void*) { return 0; }
struct CThread {
    unsigned int m_0;
    void*        m_handle;
    unsigned int m_threadId;
    bool Create(PFN_Start start, void* param, void* security, unsigned int flags, unsigned int stackSize)
    {
        m_handle = StubCreateThread(security, stackSize, start, param, flags, &m_threadId);
        return m_handle != 0;
    }
};
int main()
{
    CThread t = { 0, 0, 0 }; int token = 0, sec = 0;
    g_result = &token;
    int ok = t.Create(Proc, &t, &sec, 4, 0x10000) && t.m_handle == &token && t.m_threadId == 0x1234
          && g_sec == &sec && g_stack == 0x10000 && g_start == Proc && g_param == &t && g_flags == 4;
    g_result = 0;
    ok = ok && !t.Create(Proc, 0, 0, 0, 0) && t.m_handle == 0;
    if (!ok) { std::printf("00c5e180_TEST FAIL\n"); return 1; }
    std::printf("00c5e180_TEST PASS\n");
    return 0;
}