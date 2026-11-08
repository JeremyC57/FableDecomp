// CBasePin::GetMediaType(int iPosition, CMediaType* pMediaType) -- DirectShow BaseClasses amfilter.cpp: the base pin offers no preferred media types (E_UNEXPECTED)
// Vendor-proof: strmbase-dxsdk-sum2004(BaseClasses/amfilter.cpp) ?GetMediaType@CBasePin@@UAEJHPAVCMediaType@@@Z -- 8 bytes verified against Fable.exe 0x00CA84F0
// Leaf: ?GetMediaType@CBasePin@@UAEJHPAVCMediaType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA84F0, thiscall: `mov eax,8000FFFFh ; ret 8`.
// Identity (vendor naming V2, proven): amfilter.cpp `HRESULT CBasePin::GetMediaType(int iPosition, CMediaType *pMediaType)
//   { return E_UNEXPECTED; }`; V1 LABELS (vtable slot + declaration order) name 0x00CA84F0 CBasePin::GetMediaType.
//   Was ?Get@T@@QAEHHH@Z (gapscan stand-in).
typedef long HRESULT;
typedef long LONG;
typedef __int64 REFERENCE_TIME;
typedef unsigned short WCHAR;                     // UNICODE strmbase, wchar_t not a native type (LPWSTR mangles PAG)
#define NOERROR 0L
#define E_UNEXPECTED ((HRESULT)0x8000FFFFL)
#define E_NOTIMPL ((HRESULT)0x80004001L)
class CMediaType;                                 // mtype.h
class CBasePin {                                  // amfilter.h (only the member this row defines)
public:
    virtual HRESULT GetMediaType(int iPosition, CMediaType* pMediaType);
};

HRESULT CBasePin::GetMediaType(int iPosition, CMediaType* pMediaType)
{
    return E_UNEXPECTED;
}
