#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model.
typedef int HRESULT;                                   // 4 bytes on every host (g++ LP64: long is 8)
typedef unsigned int DWORD;
static int g_mode, g_coinit, g_uninit;
static void* GetModuleHandleW_(const wchar_t* name) { return (g_mode >= 1 && name[0] == L'o') ? (void*)&g_mode : 0; }
static HRESULT __stdcall_CoInitializeEx(void* r, DWORD flags) { (void)r; ++g_coinit; return flags == 4 ? (g_mode == 3 ? 1 : 0) : (HRESULT)0x80000000U; }
typedef HRESULT (*PCoInitializeEx)(void*, DWORD);
static PCoInitializeEx GetProcAddress_(void* h, const char* n) { (void)h; return (g_mode >= 2 && std::strcmp(n, "CoInitializeEx") == 0) ? __stdcall_CoInitializeEx : 0; }
static void CoUninitialize_() { ++g_uninit; }
class CAMThread {
public:
    DWORD Result;
    virtual DWORD ThreadProc() { return Result; }
    static HRESULT CoInitializeHelper()
    {
        HRESULT hr = (HRESULT)0x80004005U;
        void* hOle = GetModuleHandleW_(L"ole32.dll");
        if (hOle) {
            PCoInitializeEx p = GetProcAddress_(hOle, "CoInitializeEx");
            if (p) hr = (*p)(0, 4);
        }
        return hr;
    }
    static DWORD InitialThreadProc(void* pv)
    {
        HRESULT hrCoInit = CoInitializeHelper();
        CAMThread* pThread = (CAMThread*)pv;
        HRESULT hr = pThread->ThreadProc();
        if (hrCoInit >= 0) CoUninitialize_();
        return hr;
    }
};
int main()
{
    int ok = 1;
    CAMThread t; t.Result = 77;
    g_mode = 0; ok = CAMThread::InitialThreadProc(&t) == 77 && g_coinit == 0 && g_uninit == 0;      // no ole32: E_FAIL, no uninit
    g_mode = 1; ok = ok && CAMThread::InitialThreadProc(&t) == 77 && g_coinit == 0 && g_uninit == 0; // no export
    g_mode = 2; ok = ok && CAMThread::InitialThreadProc(&t) == 77 && g_coinit == 1 && g_uninit == 1; // S_OK
    g_mode = 3; ok = ok && CAMThread::InitialThreadProc(&t) == 77 && g_coinit == 2 && g_uninit == 2; // S_FALSE still succeeded
    if (!ok) { std::printf("00cb0d80_TEST FAIL\n"); return 1; }
    std::printf("00cb0d80_TEST PASS\n");
    return 0;
}