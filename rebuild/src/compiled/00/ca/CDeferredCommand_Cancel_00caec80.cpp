// CDeferredCommand::Cancel() -- DirectShow BaseClasses ctlutil.cpp: VFW_E_ALREADY_CANCELLED once the command left its queue; otherwise ask the queue to remove it (CCmdQueue::Remove, virtual slot 3) and, on success, forget the queue
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/ctlutil.cpp) ?Cancel@CDeferredCommand@@UAGJXZ -- 47 bytes verified against Fable.exe 0x00CAEC80
// Leaf: ?Cancel@CDeferredCommand@@UAGJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CAEC80, __stdcall IDeferredCommand method (`this` = the IDeferredCommand subobject at +0x0C, pushed).
// Identity (vendor naming V2, proven): ctlutil.cpp `STDMETHODIMP CDeferredCommand::Cancel() { if (m_pQueue == NULL) return
//   VFW_E_ALREADY_CANCELLED; HRESULT hr = m_pQueue->Remove(this); if (FAILED(hr)) return hr; m_pQueue = NULL; return S_OK; }`;
//   V1 LABELS (strmbase vtable slot + ctlutil.h declaration order) name 0x00CAEC80 CDeferredCommand::Cancel.
//   Was ?Unadvise@CAdviseHolder_00caec80@@UAGJXZ (a descriptive stand-in with the same body).
// Layout: CUnknown (INonDelegatingUnknown vptr +0x00, m_pUnknown +0x04, m_cRef +0x08), IDeferredCommand vptr +0x0C, m_pQueue +0x10.
class CDeferredCommand;

class CCmdQueue {                                 // ctlutil.h: plain C++ virtuals (`this` in ecx)
public:
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual long Remove(CDeferredCommand* pCmd);  // slot 3 (+0x0c)
};

class CUnknown {                                  // combase.h subobject: primary vptr + m_pUnknown + m_cRef (0x0c bytes)
public:
    virtual long __stdcall NonDelegatingQueryInterface();
    void* m_pUnknown;                             // +0x04
    long m_cRef;                                  // +0x08
};

struct IDeferredCommand {                         // strmif.h (+0x0c); only the slot this row overrides
    virtual long __stdcall Cancel() = 0;
};

class CDeferredCommand : public CUnknown, public IDeferredCommand {
public:
    CCmdQueue* m_pQueue;                          // +0x10
    long __stdcall Cancel();
};

long __stdcall CDeferredCommand::Cancel()
{
    if (!m_pQueue)
        return 0x80040234;                        // VFW_E_ALREADY_CANCELLED
    long hr = m_pQueue->Remove(this);
    if (hr < 0)
        return hr;
    m_pQueue = 0;
    return 0;
}
