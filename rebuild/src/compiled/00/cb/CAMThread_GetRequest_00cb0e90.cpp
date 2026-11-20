// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/wxutil.cpp) ?GetRequest@CAMThread@@QAEKXZ -- 20 bytes verified against Fable.exe 0x00CB0E90
// CAMThread::GetRequest() -- wait on the request event (m_EventSend.Wait(), INFINITE), then return the posted request m_dwParam
// Leaf: ?GetRequest@CAMThread@@QAEKXZ
// Retail: push esi; esi=ecx; eax=[esi+4]; push -1; push eax; call [WaitForSingleObject]; eax=[esi+0xc]; ret (20 bytes).
// Naming (NF198, 2026-11-20, q12004): the row's catalog identity CWorldMap::PrepareForLoad was a propagated label (the real
//   CWorldMap::PrepareForLoad is 0x004FF560). The body is the DirectShow base classes' CAMThread::GetRequest (wxutil.cpp:
//   `m_EventSend.Wait(); return m_dwParam;`, the vendor proof above), on the CAMThread layout vptr +0, m_EventSend +4, m_EventComplete +8,
//   m_dwParam +0xC (wxutil.h; the landed TU-mates 0x00CB0D80 CAMThread::InitialThreadProc and 0x00CB0E30 use the same layout). CAMThread
//   has no shared header (the landed 0x00CB0D80 models it locally too). Was the stand-in CWorldMap::PrepareForLoad on a local CWorldMap.
#include "rebuild_abi.h"
extern "C" __declspec(dllimport) unsigned long FABLE_STDCALL WaitForSingleObject(void *a, unsigned long b);   // IAT 0x0143FEAC = KERNEL32.dll!WaitForSingleObject
class CAMThread { public:
    char          _vptr[4];          // +0x00
    void*         m_EventSend;       // +0x04 CAMEvent (its HANDLE)
    void*         m_EventComplete;   // +0x08 CAMEvent (its HANDLE)
    unsigned long m_dwParam;         // +0x0c
    unsigned long GetRequest();
};
unsigned long CAMThread::GetRequest(){ WaitForSingleObject(m_EventSend, (unsigned long)-1); return m_dwParam; }
