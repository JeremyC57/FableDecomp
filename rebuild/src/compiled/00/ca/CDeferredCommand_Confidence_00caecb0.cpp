// CDeferredCommand::Confidence(LONG* pConfidence) -- DirectShow BaseClasses ctlutil.cpp: a deferred command reports no confidence value (E_NOTIMPL)
// Leaf: ?Confidence@CDeferredCommand@@UAGJPAJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CAECB0, __stdcall COM method: `mov eax,80004001h ; ret 8`.
// Identity (vendor naming V2, proven): ctlutil.h CDeferredCommand `STDMETHODIMP Confidence(LONG* pConfidence);` (IDeferredCommand),
//   ctlutil.cpp body `return E_NOTIMPL;`; V1 LABELS name 0x00CAECB0 CDeferredCommand::Confidence. Was ?Get@T@@QAEHHH@Z.
typedef long HRESULT;
typedef long LONG;
typedef __int64 REFERENCE_TIME;
typedef unsigned short WCHAR;                     // UNICODE strmbase, wchar_t not a native type (LPWSTR mangles PAG)
#define NOERROR 0L
#define E_UNEXPECTED ((HRESULT)0x8000FFFFL)
#define E_NOTIMPL ((HRESULT)0x80004001L)
class CDeferredCommand {                          // ctlutil.h (only the member this row defines)
public:
    virtual HRESULT __stdcall Confidence(LONG* pConfidence);
};

HRESULT __stdcall CDeferredCommand::Confidence(LONG* pConfidence)
{
    return E_NOTIMPL;
}
