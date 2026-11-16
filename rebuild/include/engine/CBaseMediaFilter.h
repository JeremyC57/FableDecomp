#ifndef FABLE_ENGINE_CBASEMEDIAFILTER_PASS23_H
#define FABLE_ENGINE_CBASEMEDIAFILTER_PASS23_H
#ifdef FABLE_RETAIL_CBASEMEDIAFILTER_PASS23
// DirectShow amfilter.h/combase.h/wxutil.h, corroborated by native00caa860..caaa54.
// Reuse the package's SDK interfaces, CUnknown, CRefTime and CAutoLock.
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CUNKNOWN_SDK_REFCOUNT_PASS23
#define FABLE_RETAIL_CBASEVIDEORENDERER_PORTABLE_SDK_PASS23
#include "engine/CBaseVideoRenderer.h"
class CBaseMediaFilter : public CUnknown, public IMediaFilter {
protected:
    FILTER_STATE m_State;
    IReferenceClock* m_pClock;
    CRefTime m_tStart;
    GUID m_clsid;
    CCritSec* m_pLock;
public:
    CBaseMediaFilter(const unsigned short*,IUnknown*,CCritSec*,const GUID&);
    virtual ~CBaseMediaFilter();
    long __stdcall QueryInterface(const GUID&,void**);
    unsigned long __stdcall AddRef();
    unsigned long __stdcall Release();
    long __stdcall NonDelegatingQueryInterface(const GUID&,void**);
    long __stdcall GetClassID(GUID*);
    long __stdcall GetState(unsigned long,FILTER_STATE*);
    long __stdcall SetSyncSource(IReferenceClock*);
    long __stdcall GetSyncSource(IReferenceClock**);
    long __stdcall Stop();
    long __stdcall Pause();
    long __stdcall Run(REFERENCE_TIME);
    virtual long StreamTime(CRefTime&);
    int IsActive() { CAutoLock lock(m_pLock); return m_State==State_Paused || m_State==State_Running; }
};
// Complete native object: CUnknown12, IMediaFilter4, state4, clock4,
// stream time8, GUID16, lock4, tail alignment4. No packing adjustment.
FABLE_STATIC_ASSERT(sizeof(CBaseMediaFilter)==0x38);
#endif
#endif
