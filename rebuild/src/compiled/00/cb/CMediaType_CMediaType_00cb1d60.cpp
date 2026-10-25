// CMediaType::CMediaType(const AM_MEDIA_TYPE& rt, long* phr) -- DirectShow mtype.cpp: construct by CopyMediaType(this, &rt), storing a failure HRESULT through phr when given
// Leaf: ??0CMediaType@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB1D60, 34 bytes. __fastcall constructor, `ret 8`, returns this.
// Library: DirectShow base classes (strmbase mtype.cpp). The manifest label "COM_CopyVariantSafe" is descriptive.
// Callee: CopyMediaType @ 0x00CB1BD0 (WINAPI, `push &rt ; push this`, callee-clean).
// Shape: `HRESULT hr = CopyMediaType(this, &rt); if (FAILED(hr) && (NULL != phr)) *phr = hr;` -- FAILED is the `jge`.
struct IUnknown_mt {
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1 (+0x04)
    virtual unsigned long __stdcall Release();                               // slot 2 (+0x08)
};

struct GUID { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };

// AM_MEDIA_TYPE (strmif.h _AMMediaType), 0x48 bytes; named like the landed CMediaType rows 0x00CB19C0 / 0x00CB1E50.
struct AM_MEDIA_TYPE {
    GUID           majortype;                       // +0x00
    GUID           subtype;                         // +0x10
    int            bFixedSizeSamples;               // +0x20
    int            bTemporalCompression;            // +0x24
    unsigned long  lSampleSize;                     // +0x28
    GUID           formattype;                      // +0x2c
    IUnknown_mt*   pUnk;                            // +0x3c
    unsigned long  cbFormat;                        // +0x40
    unsigned char* pbFormat;                        // +0x44
};

long __stdcall CopyMediaType(AM_MEDIA_TYPE* pmtTarget, const AM_MEDIA_TYPE* pmtSource);   // 0x00CB1BD0

class CMediaType : public AM_MEDIA_TYPE {
public:
    CMediaType(const AM_MEDIA_TYPE& rt, long* phr = 0);
};

CMediaType::CMediaType(const AM_MEDIA_TYPE& rt, long* phr)
{
    long hr = CopyMediaType(this, &rt);
    if ((hr < 0) && (0 != phr)) {
        *phr = hr;
    }
}