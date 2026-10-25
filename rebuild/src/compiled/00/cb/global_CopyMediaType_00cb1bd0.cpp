// CopyMediaType(AM_MEDIA_TYPE* pmtTarget, const AM_MEDIA_TYPE* pmtSource) -- DirectShow mtype.cpp: struct copy plus a CoTaskMemAlloc'd copy of the format block and an AddRef of pUnk
// Leaf: ?CopyMediaType@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB1BD0, 103 bytes. WINAPI (__stdcall, C++ linkage), `ret 8`.
// Library: DirectShow base classes (strmbase mtype.cpp). The manifest label "COM_CopyVariant" is descriptive. It is
// called by the landed CMediaType::Set 0x00CB1E50 (same `CopyMediaType` declaration, same symbol), by the
// CMediaType(const AM_MEDIA_TYPE&, HRESULT*) constructor 0x00CB1D60 and by CreateMediaType 0x00CB1F70.
// Callees: ole32 CoTaskMemAlloc (IAT 0x01440638); IUnknown::AddRef (slot 1, __stdcall).
// Shape: the SDK body verbatim -- `*pmtTarget = *pmtSource` is `rep movsd` x0x12, the pbFormat store precedes its
// null test, cbFormat = 0 on failure reuses the null eax, CopyMemory is the memcpy intrinsic.
#include <string.h>

extern "C" __declspec(dllimport) void* __stdcall CoTaskMemAlloc(unsigned long cb);        // IAT 0x01440638

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

long __stdcall CopyMediaType(AM_MEDIA_TYPE* pmtTarget, const AM_MEDIA_TYPE* pmtSource)
{
    //  We'll leak if we copy onto one that already exists - there's one
    //  case we can check like that - copying to itself.
    *pmtTarget = *pmtSource;
    if (pmtSource->cbFormat != 0) {
        pmtTarget->pbFormat = (unsigned char*)CoTaskMemAlloc(pmtSource->cbFormat);
        if (pmtTarget->pbFormat == 0) {
            pmtTarget->cbFormat = 0;
            return 0x8007000EL;                     // E_OUTOFMEMORY
        } else {
            memcpy((void*)pmtTarget->pbFormat, (void*)pmtSource->pbFormat,
                   pmtTarget->cbFormat);
        }
    }
    if (pmtTarget->pUnk != 0) {
        pmtTarget->pUnk->AddRef();
    }

    return 0;                                       // S_OK
}