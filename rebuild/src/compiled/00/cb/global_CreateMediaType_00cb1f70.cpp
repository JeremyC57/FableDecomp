// CreateMediaType(const AM_MEDIA_TYPE* pSrc) -- DirectShow mtype.cpp: CoTaskMemAlloc a new AM_MEDIA_TYPE and CopyMediaType into it; NULL (block freed) on failure
// Leaf: ?CreateMediaType@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB1F70, 47 bytes. WINAPI (__stdcall, C++ linkage), `ret 4`.
// Library: DirectShow base classes (strmbase mtype.cpp). The manifest label "ImmContext_Create" is descriptive.
// Callees: ole32 CoTaskMemAlloc (IAT 0x01440638), CoTaskMemFree (IAT 0x01440630), CopyMediaType @ 0x00CB1BD0.
// Shape: the SDK body verbatim (sizeof(AM_MEDIA_TYPE) = 0x48, FAILED(hr) as `jge`).
extern "C" __declspec(dllimport) void* __stdcall CoTaskMemAlloc(unsigned long cb);        // IAT 0x01440638
extern "C" __declspec(dllimport) void __stdcall CoTaskMemFree(void* pv);                  // IAT 0x01440630

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

AM_MEDIA_TYPE* __stdcall CreateMediaType(const AM_MEDIA_TYPE* pSrc)
{
    // Allocate a block of memory for the media type
    AM_MEDIA_TYPE* pMediaType =
        (AM_MEDIA_TYPE*)CoTaskMemAlloc(sizeof(AM_MEDIA_TYPE));

    if (pMediaType == 0) {
        return 0;
    }
    // Copy the variable length format block
    long hr = CopyMediaType(pMediaType, pSrc);
    if (hr < 0) {
        CoTaskMemFree((void*)pMediaType);
        return 0;
    }

    return pMediaType;
}