// CMediaType::Set(const CMediaType&) -- DirectShow media-type copy: unless self, free this type's format block and IUnknown, then CopyMediaType; E_OUTOFMEMORY on failure
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB1E50, length 91 bytes (catalogued as the propagated label
// "CTCObstacle::OnKill"; its callers @ 0x00CB1FE8 / 0x00CB2008 are the two `CMediaType::operator=` forwarders and
// 0x00CB2020 is a jmp thunk to it). The layout is AM_MEDIA_TYPE: pUnk +0x3c, cbFormat +0x40, pbFormat +0x44.
// Leaf: Set
// Callees: ole32 CoTaskMemFree (IAT 0x01440630), IUnknown::Release (virtual slot 2, __stdcall), CopyMediaType(AM_MEDIA_TYPE*,
// const AM_MEDIA_TYPE*) @ 0x00CB1BD0 (WINAPI: `push src ; push this`, no cleanup).
// Shape: the DirectShow base-class `Set`: `&rt != this` (`cmp ebx,esi`), FreeMediaType inlined (the landed out-of-line copy is
// 0x00CB1CC0), FAILED(hr) as `jge` to the shared S_OK tail.
extern "C" __declspec(dllimport) void __stdcall CoTaskMemFree(void* block);   // IAT 0x01440630

struct IUnknown_00cb1e50 {
    virtual long __stdcall QueryInterface(const void* iid, void** object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();      // slot 2 (+0x08)
};

struct GUID_00cb1e50 { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };

struct AM_MEDIA_TYPE {
    GUID_00cb1e50      majortype;                   // +0x00
    GUID_00cb1e50      subtype;                     // +0x10
    long               bFixedSizeSamples;           // +0x20
    long               bTemporalCompression;        // +0x24
    unsigned long      lSampleSize;                 // +0x28
    GUID_00cb1e50      formattype;                  // +0x2c
    IUnknown_00cb1e50* pUnk;                        // +0x3c
    unsigned long      cbFormat;                    // +0x40
    unsigned char*     pbFormat;                    // +0x44
};

long __stdcall CopyMediaType(AM_MEDIA_TYPE* target, const AM_MEDIA_TYPE* source);   // 0x00CB1BD0

inline void FreeMediaType(AM_MEDIA_TYPE& mt)
{
    if (mt.cbFormat != 0) {
        CoTaskMemFree(mt.pbFormat);
        mt.cbFormat = 0;
        mt.pbFormat = 0;
    }
    if (mt.pUnk != 0) {
        mt.pUnk->Release();
        mt.pUnk = 0;
    }
}

struct CMediaType : AM_MEDIA_TYPE {
    long Set(const CMediaType& rt);
};

long CMediaType::Set(const CMediaType& rt)
{
    if (&rt != this) {
        FreeMediaType(*this);
        long hr = CopyMediaType(this, &rt);
        if (hr < 0)
            return 0x8007000EL;                     // E_OUTOFMEMORY
    }
    return 0;                                       // S_OK
}