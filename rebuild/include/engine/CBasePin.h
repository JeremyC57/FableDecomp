#ifndef FABLE_ENGINE_CBASEPIN_PASS23_H
#define FABLE_ENGINE_CBASEPIN_PASS23_H
#ifdef FABLE_RETAIL_CDYNAMICOUTPUTPIN_PASS23
#define FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CBASEVIDEORENDERER_PORTABLE_SDK_PASS23
#define FABLE_RETAIL_CUNKNOWN_SDK_REFCOUNT_PASS23
#include "engine/CBaseVideoRenderer.h"
#include "engine/CMediaType.h"
enum _PinDirection { PINDIR_INPUT,PINDIR_OUTPUT };
typedef _PinDirection PIN_DIRECTION;
struct _PinInfo { IBaseFilter* pFilter; PIN_DIRECTION dir; unsigned short achName[128]; };
struct IEnumMediaTypes;
struct IPin : IUnknown {
    virtual long __stdcall Connect(IPin*,const _AMMediaType*) = 0;
    virtual long __stdcall ReceiveConnection(IPin*,const _AMMediaType*) = 0;
    virtual long __stdcall Disconnect() = 0;
    virtual long __stdcall ConnectedTo(IPin**) = 0;
    virtual long __stdcall ConnectionMediaType(_AMMediaType*) = 0;
    virtual long __stdcall QueryPinInfo(_PinInfo*) = 0;
    virtual long __stdcall QueryDirection(PIN_DIRECTION*) = 0;
    virtual long __stdcall QueryId(unsigned short**) = 0;
    virtual long __stdcall QueryAccept(const _AMMediaType*) = 0;
    virtual long __stdcall EnumMediaTypes(IEnumMediaTypes**) = 0;
    virtual long __stdcall QueryInternalConnections(IPin**,unsigned long*) = 0;
    virtual long __stdcall EndOfStream() = 0;
    virtual long __stdcall BeginFlush() = 0;
    virtual long __stdcall EndFlush() = 0;
    virtual long __stdcall NewSegment(REFERENCE_TIME,REFERENCE_TIME,double) = 0;
};
// Layout: SDK amfilter.h; native CA97A0 IPin+0c, connected+18, lock+20,
// BreakConnect primary slot11. Native CAB770 confirms the downstream layout.
class CBasePin : public CUnknown, public IPin, public IQualityControl {
protected:
    unsigned short* m_pName;
    IPin* m_Connected;
    PIN_DIRECTION m_dir;
    CCritSec* m_pLock;
    bool m_bRunTimeError;
    bool m_bCanReconnectWhenActive;
    bool m_bTryMyTypesFirst;
    CBaseFilter* m_pFilter;
    IQualityControl* m_pQSink;
    long m_TypeVersion;
    CMediaType m_mt;
    CRefTime m_tStart;
    CRefTime m_tStop;
    double m_dRate;
    long __stdcall DisconnectInternal();
public:
    CBasePin(const unsigned short*,CBaseFilter*,CCritSec*,long*,const unsigned short*,PIN_DIRECTION);
    virtual ~CBasePin();
    long __stdcall QueryInterface(const GUID&,void**);
    unsigned long __stdcall AddRef();
    unsigned long __stdcall Release();
    long __stdcall NonDelegatingQueryInterface(const GUID&,void**);
    unsigned long __stdcall NonDelegatingRelease();
    unsigned long __stdcall NonDelegatingAddRef();
    long __stdcall Connect(IPin*,const _AMMediaType*);
    long __stdcall ReceiveConnection(IPin*,const _AMMediaType*);
    long __stdcall Disconnect();
    long __stdcall ConnectedTo(IPin**);
    long __stdcall ConnectionMediaType(_AMMediaType*);
    long __stdcall QueryPinInfo(_PinInfo*);
    long __stdcall QueryDirection(PIN_DIRECTION*);
    long __stdcall QueryId(unsigned short**);
    long __stdcall QueryAccept(const _AMMediaType*);
    long __stdcall EnumMediaTypes(IEnumMediaTypes**);
    long __stdcall QueryInternalConnections(IPin**,unsigned long*);
    long __stdcall EndOfStream();
    long __stdcall NewSegment(REFERENCE_TIME,REFERENCE_TIME,double);
    long __stdcall Notify(IBaseFilter*,Quality);
    long __stdcall SetSink(IQualityControl*);
    virtual long GetMediaTypeVersion();
    virtual long Active();
    virtual long Inactive();
    virtual long Run(REFERENCE_TIME);
    virtual long CheckMediaType(const CMediaType*) = 0;
    virtual long SetMediaType(const CMediaType*);
    virtual long CheckConnect(IPin*);
    virtual long BreakConnect();
    virtual long CompleteConnect(IPin*);
    virtual long GetMediaType(int,CMediaType*);
};
FABLE_STATIC_ASSERT(sizeof(CBasePin)==0x98);
#endif
#endif
