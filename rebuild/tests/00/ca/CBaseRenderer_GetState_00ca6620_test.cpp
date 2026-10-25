// Behaviour test for 0x00CA6620 CBaseRenderer::GetState (DirectShow renbase.cpp, IMediaFilter): wait (dispatching messages) for the state change to complete; report the state, with VFW_S_STATE_INTERMEDIATE on timeout.
// Test model: plain C++98 copies of the members the function touches; Win32 / COM callees are recording stand-ins.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#ifdef _MSC_VER
typedef __int64 LONGLONG;
#else
typedef long long LONGLONG;
#endif
typedef LONGLONG REFERENCE_TIME;
typedef int HRESULT;
struct GUID { unsigned int Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; };
inline int operator==(const GUID& a, const GUID& b) { return std::memcmp(&a, &b, sizeof(GUID)) == 0; }
#define NOERROR 0
#define S_OK 0
#define S_FALSE 1
#define E_POINTER ((HRESULT)0x80004003u)
#define E_OUTOFMEMORY ((HRESULT)0x8007000Eu)
#define E_FAIL ((HRESULT)0x80004005u)
#define FAILED(hr) (((HRESULT)(hr)) < 0)
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define CheckPointer(p, ret) { if ((p) == 0) return (ret); }
#define State_Stopped 0
#define State_Paused 1
#define State_Running 2
struct CCritSec {
    int depth; int entered;
    CCritSec() : depth(0), entered(0) {}
    void Lock() { depth++; entered++; }
    void Unlock() { depth--; }
};
struct CAutoLock {
    CCritSec* m_pLock;
    CAutoLock(CCritSec* plock) : m_pLock(plock) { m_pLock->Lock(); }
    ~CAutoLock() { m_pLock->Unlock(); }
};
struct CAMEvent {                                // the test's event: a flag instead of a kernel HANDLE
    int signalled;
    CAMEvent() : signalled(0) {}
    void Set() { signalled = 1; }
    void Reset() { signalled = 0; }
};
#define WAIT_TIMEOUT 258
#define VFW_S_STATE_INTERMEDIATE ((HRESULT)0x00040237)
static unsigned int g_result; static void* g_obj; static unsigned int g_wait;
static unsigned int WaitDispatchingMessages(void* hObject, unsigned int dwWait) { g_obj = hObject; g_wait = dwWait; return g_result; }
struct CBaseRenderer {
    int m_State; CAMEvent m_evComplete;
    HRESULT GetState(unsigned int dwMSecs, int* State);
};
HRESULT CBaseRenderer::GetState(unsigned int dwMSecs, int* State)
{
    CheckPointer(State, E_POINTER);
    if (WaitDispatchingMessages(&m_evComplete, dwMSecs) == WAIT_TIMEOUT) {
        *State = m_State;
        return VFW_S_STATE_INTERMEDIATE;
    }
    *State = m_State;
    return NOERROR;
}
int main()
{
    CBaseRenderer r; r.m_State = State_Paused;
    int st = -1;
    int ok = 1;
    ok &= r.GetState(100, 0) == E_POINTER;
    g_result = WAIT_TIMEOUT; ok &= r.GetState(100, &st) == VFW_S_STATE_INTERMEDIATE && st == State_Paused && g_obj == &r.m_evComplete && g_wait == 100;
    g_result = 0; r.m_State = State_Running; st = -1; ok &= r.GetState(5, &st) == NOERROR && st == State_Running && g_wait == 5;
    if (!ok) { std::printf("00ca6620_TEST FAIL\n"); return 1; }
    std::printf("00ca6620_TEST PASS\n");
    return 0;
}