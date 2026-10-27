// CAMThread::InitialThreadProc(void* pv) -- DirectShow wxutil.cpp: the worker thread entry: CoInitializeEx through ole32's export (CoInitializeHelper), run the derived ThreadProc, CoUninitialize when the init succeeded, return ThreadProc's result
// Leaf: ?InitialThreadProc@CAMThread@@SGKPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB0D80, 73 bytes, static `__stdcall` (WINAPI), `ret 4`.
// Identity (proven by source and data): DirectShow base classes (strmbase wxutil.cpp). The body is the SDK's InitialThreadProc
//   with CAMThread::CoInitializeHelper inlined: hr = E_FAIL (0x80004005); GetModuleHandleW(L"ole32.dll" 0x012BFA54);
//   GetProcAddress(hOle, "CoInitializeEx" 0x012BFA44); hr = pCoInitializeEx(0, COINIT_DISABLE_OLE1DDE = 4); then
//   ((CAMThread*)pv)->ThreadProc() (vtable slot 0, the first virtual CAMThread declares) and CoUninitialize when
//   SUCCEEDED(hr). Its only reference is an absolute one at 0x00CB0DFC (CAMThread::Create passing it to CreateThread).
//   DbgLog / DbgBreak compile to nothing in this release build.
// Callees: KERNEL32 GetModuleHandleW (IAT 0x0143FEA4), GetProcAddress (IAT 0x0143FE44), ole32 CoUninitialize (IAT 0x01440648).
// Layout: CAMThread -- only its vptr is touched here (ThreadProc is slot 0; the landed CWorkerThread_00cb0d20 destructor 0x00CB0D20
//   is CAMThread::~CAMThread with the SDK member order).
#include <stddef.h>
typedef long HRESULT;
typedef unsigned long DWORD;
struct HINSTANCE__ { int unused; };
typedef HINSTANCE__* HMODULE;
typedef int (__stdcall* FARPROC)();
extern "C" __declspec(dllimport) HMODULE __stdcall GetModuleHandleW(const wchar_t* moduleName);    // IAT 0x0143FEA4
extern "C" __declspec(dllimport) FARPROC __stdcall GetProcAddress(HMODULE module, const char* name);  // IAT 0x0143FE44
extern "C" __declspec(dllimport) void __stdcall CoUninitialize();                                     // IAT 0x01440648

class CAMThread {
protected:
    virtual DWORD ThreadProc() = 0;                     // slot 0
public:
    virtual ~CAMThread();
    static DWORD __stdcall InitialThreadProc(void* pv);
    static HRESULT CoInitializeHelper();
};

HRESULT CAMThread::CoInitializeHelper()
{
    HRESULT hr = (HRESULT)0x80004005L;                  // E_FAIL
    HMODULE hOle = GetModuleHandleW(L"ole32.dll");
    if (hOle)
    {
        typedef HRESULT (__stdcall* PCoInitializeEx)(void* pvReserved, DWORD dwCoInit);
        PCoInitializeEx pCoInitializeEx = (PCoInitializeEx)(GetProcAddress(hOle, "CoInitializeEx"));
        if (pCoInitializeEx)
        {
            hr = (*pCoInitializeEx)(0, 4);              // COINIT_DISABLE_OLE1DDE
        }
    }
    return hr;
}

DWORD __stdcall CAMThread::InitialThreadProc(void* pv)
{
    HRESULT hrCoInit = CAMThread::CoInitializeHelper();

    CAMThread* pThread = (CAMThread*)pv;
    HRESULT hr = pThread->ThreadProc();

    if (hrCoInit >= 0)                                  // SUCCEEDED(hrCoInit)
    {
        CoUninitialize();
    }
    return hr;
}