// CMediaType::ReallocFormatBuffer(unsigned long length) -- DirectShow mtype.cpp: reallocate the format block, copying min(length, cbFormat) bytes of the old one before freeing it
// Leaf: ?ReallocFormatBuffer@CMediaType@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB1A40, 119 bytes (+ int3 padding). __fastcall, `ret 4`.
// Library: DirectShow base classes (strmbase mtype.cpp). The manifest label "COM_AllocAndCacheBuffer_WithCopy" is
// descriptive.
// Callees: ole32 CoTaskMemAlloc (IAT 0x01440638), CoTaskMemFree (IAT 0x01440630); CopyMemory is the memcpy
// intrinsic.
// Shape: the SDK body verbatim; min() is the windef.h macro `((a) < (b) ? (a) : (b))` (`cmp ; jae ; mov`).
#include <string.h>

extern "C" __declspec(dllimport) void* __stdcall CoTaskMemAlloc(unsigned long cb);        // IAT 0x01440638
extern "C" __declspec(dllimport) void __stdcall CoTaskMemFree(void* pv);                  // IAT 0x01440630

#define min(a,b) (((a) < (b)) ? (a) : (b))

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

class CMediaType : public AM_MEDIA_TYPE {
public:
    unsigned char* ReallocFormatBuffer(unsigned long length);
};

unsigned char* CMediaType::ReallocFormatBuffer(unsigned long length)
{
    // do the types have the same buffer size
    if (cbFormat == length) {
        return pbFormat;
    }

    // allocate the new format buffer
    unsigned char* pNewFormat = (unsigned char*)CoTaskMemAlloc(length);
    if (pNewFormat == 0) {
        if (length <= cbFormat) return pbFormat;   // reuse the old block anyway.
        return 0;
    }

    // copy any previous format (or part of if new is smaller)
    // delete the old format and replace with the new one
    if (cbFormat != 0) {
        memcpy(pNewFormat, pbFormat, min(length, cbFormat));
        CoTaskMemFree((void*)pbFormat);
    }

    cbFormat = length;
    pbFormat = pNewFormat;
    return pNewFormat;
}