// CMediaType::CMediaType() -- DirectShow BaseClasses mtype.cpp: the default constructor, InitMediaType() inline: zero the AM_MEDIA_TYPE, lSampleSize = 1, bFixedSizeSamples = TRUE; returns this
// Leaf: ??0CMediaType@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB1D00, 29 bytes (+ int3 padding), thiscall, plain `ret`, `mov eax,edx` returns `this`.
// Fix (sweep1, 2026-10-25; FINDINGS chunk 450 "0xCB1D00 = CMediaType() (comment says libpk)"): the landed row is filed as
//   CMediaType::CMediaType but modelled a by-address "libpk record" CCodecState_00cb1d00 with words m_20 / m_28. Its callers
//   0x00CAB1E7 / 0x00CAB303 / 0x00CAB3D1 / 0x00CAC0D1 are strmbase pin / enumerator code (CEnumMediaTypes::Next builds a
//   CMediaType per type), the stores are lSampleSize (+0x28) then bFixedSizeSamples (+0x20) and the twin 0x00CB1AC0 is
//   InitMediaType. mtype.cpp's InitMediaType is an out-of-line member (0x00CB1AC0) that VC7.1 /O2 expands here; it is
//   spelled `inline` so this row does not also emit it.
//   AM_MEDIA_TYPE fields: bFixedSizeSamples +0x20, lSampleSize +0x28, pUnk +0x3c, cbFormat +0x40, pbFormat +0x44 (the landed
//   CMediaType rows IsValid 0x00CB1920 / AllocFormatBuffer 0x00CB19E0 / CMediaType(const AM_MEDIA_TYPE&, HRESULT*) 0x00CB1D60).
// ---- strmbase model (DirectShow BaseClasses mtype.h, DX9 SDK era, release build) ----
// AM_MEDIA_TYPE (strmif.h _AMMediaType, 0x48 bytes), spelled like the landed CMediaType rows 0x00CB19C0 / 0x00CB1D60 / 0x00CB1E50.
struct IUnknown_mt {
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1 (+0x04)
    virtual unsigned long __stdcall Release();                               // slot 2 (+0x08)
};

struct GUID { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };

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
    ~CMediaType();
    CMediaType();
    void InitMediaType();
};

extern "C" void* __cdecl memset(void* dst, int value, unsigned int count);
#pragma intrinsic(memset)
#define ZeroMemory(Destination, Length) memset((Destination), 0, (Length))

inline void CMediaType::InitMediaType()
{
    ZeroMemory((void*)this, sizeof(*this));
    lSampleSize = 1;
    bFixedSizeSamples = 1;
}

CMediaType::CMediaType()
{
    InitMediaType();
}
