// CMediaType::ResetFormatBuffer() -- DirectShow strmbase: free the format block (if any) with CoTaskMemFree and clear cbFormat / pbFormat
// Leaf: ResetFormatBuffer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB19C0, 32 bytes.
// Callee: ole32 CoTaskMemFree (IAT 0x01440630, `call dword ptr [__imp]`).  No direct caller is
// recorded (it sits among the landed strmbase CMediaType members 0x00CB1CC0 / 0x00CB1D00 /
// 0x00CB1E50, whose AM_MEDIA_TYPE model this reuses).  The manifest label Stack<...>::~Stack is
// propagated.
// strmbase mtype.cpp:
//   void CMediaType::ResetFormatBuffer() { if (cbFormat) { CoTaskMemFree((PVOID)pbFormat); }
//                                          cbFormat = 0; pbFormat = NULL; }
// AM_MEDIA_TYPE: pUnk +0x3c, cbFormat +0x40, pbFormat +0x44.
extern "C" __declspec(dllimport) void __stdcall CoTaskMemFree(void* block);   // IAT 0x01440630

struct IUnknown_00cb19c0 {
    virtual long __stdcall QueryInterface(const void* iid, void** object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
};

struct GUID_00cb19c0 { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };

struct AM_MEDIA_TYPE {
    GUID_00cb19c0      majortype;                   // +0x00
    GUID_00cb19c0      subtype;                     // +0x10
    long               bFixedSizeSamples;           // +0x20
    long               bTemporalCompression;        // +0x24
    unsigned long      lSampleSize;                 // +0x28
    GUID_00cb19c0      formattype;                  // +0x2c
    IUnknown_00cb19c0* pUnk;                        // +0x3c
    unsigned long      cbFormat;                    // +0x40
    unsigned char*     pbFormat;                    // +0x44
};

struct CMediaType : AM_MEDIA_TYPE {
    void ResetFormatBuffer();
};

void CMediaType::ResetFormatBuffer()
{
    if (cbFormat) {
        CoTaskMemFree((void*)pbFormat);
    }
    cbFormat = 0;
    pbFormat = 0;
}