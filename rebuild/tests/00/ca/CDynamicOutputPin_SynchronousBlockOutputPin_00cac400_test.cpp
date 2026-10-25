// Behaviour test for 00cac400: CDynamicOutputPin::SynchronousBlockOutputPin -- create, async-block, wait, close; HRESULT folding
#include <cstdio>
static int g_create = 1, g_closed = 0; static long g_async = 0; static unsigned long g_wait = 0;
static void* CreateEventW(void*, int manual, int initial, const unsigned short*) { return (g_create && !manual && !initial) ? (void*)5 : 0; }
static unsigned long WaitForSingleObject(void* h, unsigned long ms) { return (h == (void*)5 && ms == 0xFFFFFFFF) ? g_wait : 99; }
static int CloseHandle(void* h) { if (h == (void*)5) ++g_closed; return 1; }
static long AmGetLastErrorToHResult() { return (long)0x80070006L; }
class CDynamicOutputPin {
public:
    long AsynchronousBlockOutputPin(void* h) { return h == (void*)5 ? g_async : -1; }
    static long WaitEvent(void* hEvent)
    {
        switch (WaitForSingleObject(hEvent, 0xFFFFFFFF)) {
        case 0: return 0;
        case 0xFFFFFFFF: return AmGetLastErrorToHResult();
        default: return (long)0x8000FFFFL;
        }
    }
    long SynchronousBlockOutputPin()
    {
        void* h = CreateEventW(0, 0, 0, 0);
        if (0 == h) return AmGetLastErrorToHResult();
        long hr = AsynchronousBlockOutputPin(h);
        if (hr < 0) { CloseHandle(h); return hr; }
        hr = WaitEvent(h);
        CloseHandle(h);
        if (hr < 0) return hr;
        return 0;
    }
};
int main()
{
    CDynamicOutputPin p; int ok = 1;
    ok &= p.SynchronousBlockOutputPin() == 0 && g_closed == 1;
    g_async = 1; ok &= p.SynchronousBlockOutputPin() == 0 && g_closed == 2;
    g_async = (long)0x80040294L; ok &= p.SynchronousBlockOutputPin() == (long)0x80040294L && g_closed == 3;
    g_async = 0; g_wait = 0xFFFFFFFF; ok &= p.SynchronousBlockOutputPin() == (long)0x80070006L && g_closed == 4;
    g_wait = 0x102; ok &= p.SynchronousBlockOutputPin() == (long)0x8000FFFFL && g_closed == 5;
    g_create = 0; ok &= p.SynchronousBlockOutputPin() == (long)0x80070006L && g_closed == 5;
    std::printf(ok ? "00cac400_TEST PASS\n" : "00cac400_TEST FAIL\n");
    return ok ? 0 : 1;
}
