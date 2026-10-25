// GetInterface(LPUNKNOWN pUnk, void** ppv) -- DirectShow BaseClasses combase.cpp: hand out an interface pointer with a reference: E_POINTER for a null ppv, else *ppv = pUnk, pUnk->AddRef(), NOERROR
// Leaf: _GetInterface@8
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CACBE0, 33 bytes (+ int3 padding). STDAPI = extern "C" __stdcall, `ret 8`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 448 callee "0xCACBE0 'CanMoveResource' = combase GetInterface" and chunk 449
//   "0xCACBE0 AcquireInterfaceReference = extern "C" GetInterface (_GetInterface@8)"): its call sites are the strmbase
//   NonDelegatingQueryInterface implementations (0x00CA60AB / 0x00CA60DA / 0x00CA60EC in landed
//   CBaseVideoRenderer::NonDelegatingQueryInterface 0x00CA6080, 0x00CA7529 / 0x00CA753B / 0x00CA758C in landed
//   CBaseFilter::NonDelegatingQueryInterface 0x00CA7500, ...), which declare it as `extern "C" long __stdcall GetInterface`;
//   the body is combase.cpp's CheckPointer + store + AddRef (IUnknown slot 1) + NOERROR.
// Label: the landed AcquireInterfaceReference (C++ linkage) and the manifest's CMovableResourceMemoryPool::CanMoveResource are
//   descriptive / propagated; STDAPI gives the C name `_GetInterface@8` the callers bind.
typedef struct _GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; } GUID;
#define NOERROR 0L
#define E_POINTER ((long)0x80004003L)
#define CheckPointer(p, ret) { if ((p) == 0) return (ret); }

struct IUnknown {
    virtual long __stdcall QueryInterface(const GUID& riid, void** ppv) = 0;   // 0
    virtual unsigned long __stdcall AddRef() = 0;                              // 1 (+0x04)
    virtual unsigned long __stdcall Release() = 0;                             // 2 (+0x08)
};
typedef IUnknown* LPUNKNOWN;

extern "C" long __stdcall GetInterface(LPUNKNOWN pUnk, void** ppv)
{
    CheckPointer(ppv, E_POINTER);
    *ppv = pUnk;
    pUnk->AddRef();
    return NOERROR;
}
