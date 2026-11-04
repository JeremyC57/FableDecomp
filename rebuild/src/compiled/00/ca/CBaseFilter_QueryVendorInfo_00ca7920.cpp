// CBaseFilter::QueryVendorInfo(LPWSTR* pVendorInfo) -- DirectShow BaseClasses amfilter.cpp: the base filter keeps no vendor information (E_NOTIMPL)
// Leaf: ?QueryVendorInfo@CBaseFilter@@UAGJPAPAG@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA7920, __stdcall COM method: `mov eax,80004001h ; ret 8` (this and the out pointer popped).
// Identity (vendor naming V2, proven): amfilter.cpp `STDMETHODIMP CBaseFilter::QueryVendorInfo(LPWSTR* pVendorInfo)
//   { UNREFERENCED_PARAMETER(pVendorInfo); return E_NOTIMPL; }` (IBaseFilter); the landed CBaseFilter models declare it
//   `HRESULT __stdcall QueryVendorInfo(WCHAR** pVendorInfo)`; V1 LABELS name 0x00CA7920 CBaseFilter::QueryVendorInfo.
//   Was ?Get@T@@QAEHHH@Z (gapscan stand-in).
typedef long HRESULT;
typedef long LONG;
typedef __int64 REFERENCE_TIME;
typedef unsigned short WCHAR;                     // UNICODE strmbase, wchar_t not a native type (LPWSTR mangles PAG)
#define NOERROR 0L
#define E_UNEXPECTED ((HRESULT)0x8000FFFFL)
#define E_NOTIMPL ((HRESULT)0x80004001L)
class CBaseFilter {                               // amfilter.h (only the member this row defines)
public:
    virtual HRESULT __stdcall QueryVendorInfo(WCHAR** pVendorInfo);
};

HRESULT __stdcall CBaseFilter::QueryVendorInfo(WCHAR** pVendorInfo)
{
    return E_NOTIMPL;
}
