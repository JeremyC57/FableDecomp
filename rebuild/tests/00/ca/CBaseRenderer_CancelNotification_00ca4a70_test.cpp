#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x00CA4A70: exercises the same logic through a copy of the model (compiled separately from the source)
static int g_cancelled = 0, g_reset = 0; static void* g_resetHandle = 0;
struct CAsyncRequest_00ca4a70 { int id; };
static int ResetEvent(void* event) { ++g_reset; g_resetHandle = event; return 1; }
struct IAsyncDevice_00ca4a70 { virtual void v00() {} virtual void Cancel(CAsyncRequest_00ca4a70* request) { if (request) ++g_cancelled; } };
struct CAsyncRequester_00ca4a70 {
    IAsyncDevice_00ca4a70* m_pDevice; void* m_hEvent; CAsyncRequest_00ca4a70* m_pRequest;
    bool CancelRequest()
    {
        CAsyncRequest_00ca4a70* request = m_pRequest;
        if (request) { m_pDevice->Cancel(request); m_pRequest = 0; }
        ResetEvent(m_hEvent);
        return request == 0;
    }
};
static bool Run()
{
    IAsyncDevice_00ca4a70 device; CAsyncRequest_00ca4a70 request; int event;
    CAsyncRequester_00ca4a70 r; r.m_pDevice = &device; r.m_hEvent = &event; r.m_pRequest = &request;
    if (r.CancelRequest()) return false;           // there was a request: false
    if (g_cancelled != 1 || g_reset != 1 || g_resetHandle != &event || r.m_pRequest != 0) return false;
    if (!r.CancelRequest()) return false;          // nothing pending: true
    return g_cancelled == 1 && g_reset == 2;
}
int main()
{
    if (!Run()) { std::printf("00ca4a70_TEST FAIL\n"); return 1; }
    std::printf("00ca4a70_TEST PASS\n");
    return 0;
}