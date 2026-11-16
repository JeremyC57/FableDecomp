#ifndef FABLE_ENGINE_CBASEOUTPUTPIN_PASS23_H
#define FABLE_ENGINE_CBASEOUTPUTPIN_PASS23_H
#ifdef FABLE_RETAIL_CDYNAMICOUTPUTPIN_PASS23
#include "engine/CBasePin.h"
struct IMemAllocator;
struct IMemInputPin;
struct _AllocatorProperties { long cBuffers,cbBuffer,cbAlign,cbPrefix; };
class CBaseOutputPin : public CBasePin {
protected:
    IMemAllocator* m_pAllocator;
    IMemInputPin* m_pInputPin;
public:
    CBaseOutputPin(const unsigned short*,CBaseFilter*,CCritSec*,long*,const unsigned short*);
    long CompleteConnect(IPin*);
    virtual long DecideAllocator(IMemInputPin*,IMemAllocator**);
    virtual long DecideBufferSize(IMemAllocator*,_AllocatorProperties*) = 0;
    virtual long GetDeliveryBuffer(IMediaSample**,REFERENCE_TIME*,REFERENCE_TIME*,unsigned long);
    virtual long Deliver(IMediaSample*);
    virtual long InitAllocator(IMemAllocator**);
    long CheckConnect(IPin*);
    long BreakConnect();
    long Active();
    long Inactive();
    long __stdcall EndOfStream();
    virtual long DeliverEndOfStream();
    long __stdcall BeginFlush();
    long __stdcall EndFlush();
    virtual long DeliverBeginFlush();
    virtual long DeliverEndFlush();
    virtual long DeliverNewSegment(REFERENCE_TIME,REFERENCE_TIME,double);
};
// Layout: SDK amfilter.h plus native CAB770 allocator98/input9c stores.
FABLE_STATIC_ASSERT(sizeof(CBaseOutputPin)==0xa0);
#endif
#endif
