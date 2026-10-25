// CMediaType::InitMediaType() -- DirectShow BaseClasses mtype.cpp: zero the whole AM_MEDIA_TYPE (0x48 bytes, `rep stosd`), then lSampleSize = 1 and bFixedSizeSamples = TRUE
// Leaf: ?InitMediaType@CMediaType@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB1AC0, 27 bytes (+ int3 padding), thiscall, plain `ret` (no `this` return).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 450 "0xCB1AC0 CCodecState::Reset = CMediaType::InitMediaType"): the stores are
//   mtype.cpp's order (+0x28 lSampleSize, then +0x20 bFixedSizeSamples); the body is CMediaType() 0x00CB1D00 without its
//   `mov eax,edx`; its caller 0x00CA83F3 is in the strmbase pin code (agent450's README lists it as a landed-name error).
//   AM_MEDIA_TYPE fields: bFixedSizeSamples +0x20, lSampleSize +0x28, pUnk +0x3c, cbFormat +0x40, pbFormat +0x44 (the landed
//   CMediaType rows IsValid 0x00CB1920 / AllocFormatBuffer 0x00CB19E0 / CMediaType(const AM_MEDIA_TYPE&, HRESULT*) 0x00CB1D60).
// Label: the landed name is a propagated manifest label / by-address placeholder; the real name is the strmbase member.
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

void CMediaType::InitMediaType()
{
    ZeroMemory((void*)this, sizeof(*this));
    lSampleSize = 1;
    bFixedSizeSamples = 1;
}
