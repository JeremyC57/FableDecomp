// CBasePin::GetMediaTypeVersion() -- DirectShow BaseClasses amfilter.cpp: return the pin's media type version m_TypeVersion (+0x30), which CEnumMediaTypes compares to detect a stale enumerator
// Leaf: ?GetMediaTypeVersion@CBasePin@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA8500, thiscall: `mov eax,[ecx+30h] ; ret`.
// Identity (vendor naming V2, proven): amfilter.h declares `virtual LONG GetMediaTypeVersion();` on CBasePin and amfilter.cpp's
//   body is `return m_TypeVersion;`; m_TypeVersion is +0x30 in the landed CBasePin models (e.g. CBaseAllocator_Commit_00caa0a0);
//   V1 LABELS (vtable slot + declaration order) name 0x00CA8500 CBasePin::GetMediaTypeVersion. Was ?Get@T@@QAEHXZ (gapscan stand-in).
typedef long HRESULT;
typedef long LONG;
typedef __int64 REFERENCE_TIME;
typedef unsigned short WCHAR;                     // UNICODE strmbase, wchar_t not a native type (LPWSTR mangles PAG)
#define NOERROR 0L
#define E_UNEXPECTED ((HRESULT)0x8000FFFFL)
#define E_NOTIMPL ((HRESULT)0x80004001L)
class CBasePin {                                  // amfilter.h; only the member this body reads (the landed CBasePin layout)
    unsigned char _base[0x2c];                    // +0x04 CUnknown / IPin / IQualityControl subobjects, name, filter, lock, connection
protected:
    LONG m_TypeVersion;                           // +0x30
public:
    virtual LONG GetMediaTypeVersion();
};

LONG CBasePin::GetMediaTypeVersion()
{
    return m_TypeVersion;
}
