// CMediaType::~CMediaType() -- DirectShow BaseClasses mtype.cpp: FreeMediaType(*this) inline: free the format block (CoTaskMemFree(pbFormat), cbFormat / pbFormat cleared) when cbFormat is non-zero, then release and clear pUnk
// Leaf: ??1CMediaType@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB1CC0, 56 bytes (+ int3 padding), thiscall, plain `ret`; CoTaskMemFree through the ole32
//   IAT (0x01440630); pUnk Release is slot 2 with the interface pushed.
// Fix (sweep1, 2026-10-25; FINDINGS chunk 450 "0xCB1CC0 CComHolder = ~CMediaType"): the landed row is filed as CMediaType but
//   modelled CComHolder_00cb1cc0::Release over a 0x3c-byte pad. It is the destructor: this sweep's CBasePin::~CBasePin
//   0x00CA7C80 calls it on m_mt (+0x34, 0x00CA7CB9) and the other callers / unwind funclets (0x00CAB253, 0x00CAB26A,
//   0x00CAB336, 0x00CAC4FB, 0x00CAC537, 0x01216C2E ...) destroy CMediaType locals and members. mtype.cpp's FreeMediaType is
//   an out-of-line WINAPI function that VC7.1 /O2 expands here (DeleteMediaType 0x00CB1F20 carries the same inline body); it
//   is spelled `inline` so this row does not also emit it.
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

extern "C" __declspec(dllimport) void __stdcall CoTaskMemFree(void* pv);   // ole32 IAT 0x01440630

inline void __stdcall FreeMediaType(AM_MEDIA_TYPE& mt)
{
    if (mt.cbFormat != 0) {
        CoTaskMemFree((void*)mt.pbFormat);

        // Strictly unnecessary but tidier
        mt.cbFormat = 0;
        mt.pbFormat = 0;
    }
    if (mt.pUnk != 0) {
        mt.pUnk->Release();
        mt.pUnk = 0;
    }
}

CMediaType::~CMediaType()
{
    FreeMediaType(*this);
}
