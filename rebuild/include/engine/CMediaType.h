#ifndef FABLE_ENGINE_CMEDIATYPE_PASS23_H
#define FABLE_ENGINE_CMEDIATYPE_PASS23_H
#ifdef FABLE_RETAIL_CDYNAMICOUTPUTPIN_PASS23
#include "engine/CBaseVideoRenderer.h"
// Layout: original DirectShow strmif.idl/mtype.h; native CBasePin media member at34, size48h.
struct _AMMediaType {
    GUID majortype;
    GUID subtype;
    int bFixedSizeSamples;
    int bTemporalCompression;
    unsigned long lSampleSize;
    GUID formattype;
    IUnknown* pUnk;
    unsigned long cbFormat;
    unsigned char* pbFormat;
};
class CMediaType : public _AMMediaType {
public:
    CMediaType();
    ~CMediaType();
    CMediaType(const CMediaType&,long* = 0);
    CMediaType(const _AMMediaType&,long* = 0);
    CMediaType(const GUID*);
    CMediaType& operator=(const CMediaType&);
    CMediaType& operator=(const _AMMediaType&);
};
FABLE_STATIC_ASSERT(sizeof(CMediaType)==0x48);
#endif
#endif
