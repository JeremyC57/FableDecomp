#ifndef FABLE_ENGINE_CDYNAMICOUTPUTPIN_PASS23_H
#define FABLE_ENGINE_CDYNAMICOUTPUTPIN_PASS23_H
#ifdef FABLE_RETAIL_CDYNAMICOUTPUTPIN_PASS23
#include "engine/CBaseOutputPin.h"
struct IPinFlowControl : IUnknown {
    virtual long __stdcall Block(unsigned long,void*) = 0;
};
struct IGraphConfig;
class CDynamicOutputPin : public CBaseOutputPin, public IPinFlowControl {
public:
    CDynamicOutputPin(const unsigned short*,CBaseFilter*,CCritSec*,long*,const unsigned short*);
    ~CDynamicOutputPin();
    long __stdcall QueryInterface(const GUID&,void**);
    unsigned long __stdcall AddRef();
    unsigned long __stdcall Release();
    long __stdcall NonDelegatingQueryInterface(const GUID&,void**);
    long __stdcall Disconnect();
    long __stdcall Block(unsigned long,void*);
    void SetConfigInfo(IGraphConfig*,void*);
    long DeliverBeginFlush();
    long DeliverEndFlush();
    long Inactive();
    long Active();
    long CompleteConnect(IPin*);
    virtual long StartUsingOutputPin();
    virtual void StopUsingOutputPin();
    virtual bool StreamingThreadUsingOutputPin();
protected:
    enum BLOCK_STATE { NOT_BLOCKED,PENDING,BLOCKED };
    CCritSec m_BlockStateLock;
    void* m_hUnblockOutputPinEvent;
    void* m_hNotifyCallerPinBlockedEvent;
    BLOCK_STATE m_BlockState;
    unsigned long m_dwBlockCallerThreadID;
    unsigned long m_dwNumOutstandingOutputPinUsers;
    void* m_hStopEvent;
    IGraphConfig* m_pGraphConfig;
    int m_bPinUsesReadOnlyAllocator;
};
// Layout: SDK amfilter.h and CAB770; flow interface+a0, lock+a4, events+bc/c0,
// state+c4, thread+c8, users+cc, stop+d0, graph+d4, allocator flag+d8.
FABLE_STATIC_ASSERT(sizeof(CDynamicOutputPin)==0xe0);
#endif
#endif
