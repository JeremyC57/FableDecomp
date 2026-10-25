// Behaviour test for 0x00CB0FC0 CMsgThread::DefaultThreadProc: CoInitialize, OnThreadInit once, then
// GetThreadMsg + ThreadMessageProc until a non-zero result (returned), CoUninitialize.
#include <cstdio>

static int g_coInit = 0, g_coUninit = 0;
static void CoInitialize_() { ++g_coInit; }
static void CoUninitialize_() { ++g_coUninit; }

class CMsg {
public:
    unsigned int uMsg;
    unsigned int dwFlags;
    void*        lpParam;
    void*        pEvent;
    CMsg() : uMsg(0), dwFlags(0), lpParam(0), pEvent(0) {}
};

class CMsgThread {
public:
    static unsigned int DefaultThreadProc(void* pParam)
    {
        CMsgThread* lpThis = (CMsgThread*)pParam;
        CMsg msg;
        long lResult;
        CoInitialize_();
        lpThis->OnThreadInit();
        do {
            lpThis->GetThreadMsg(&msg);
            lResult = lpThis->ThreadMessageProc(msg.uMsg, msg.dwFlags, msg.lpParam, msg.pEvent);
        } while (lResult == 0L);
        CoUninitialize_();
        return (unsigned int)lResult;
    }
    virtual void GetThreadMsg(CMsg* msg) = 0;
    virtual void OnThreadInit() {}
    virtual long ThreadMessageProc(unsigned int uMsg, unsigned int dwFlags, void* lpParam, void* pEvent) = 0;
    virtual ~CMsgThread() {}
};

class CTestThread : public CMsgThread {
public:
    int inits, gets, procs, sum;
    CTestThread() : inits(0), gets(0), procs(0), sum(0) {}
    void GetThreadMsg(CMsg* msg) { ++gets; msg->uMsg = gets; msg->dwFlags = gets * 10; }
    void OnThreadInit() { ++inits; }
    long ThreadMessageProc(unsigned int uMsg, unsigned int dwFlags, void*, void*)
    {
        ++procs;
        sum += (int)(uMsg + dwFlags);
        return uMsg == 3 ? 77 : 0;
    }
};

int main()
{
    CTestThread t;
    unsigned int r = CMsgThread::DefaultThreadProc(&t);
    if (r != 77 || t.inits != 1 || t.gets != 3 || t.procs != 3 || t.sum != 66 || g_coInit != 1 || g_coUninit != 1) {
        std::printf("00cb0fc0_TEST FAIL\n");
        return 1;
    }
    std::printf("00cb0fc0_TEST PASS\n");
    return 0;
}