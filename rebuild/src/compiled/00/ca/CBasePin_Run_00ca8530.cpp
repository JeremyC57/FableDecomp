// CBasePin::Run(REFERENCE_TIME tStart) -- DirectShow BaseClasses amfilter.cpp: the base pin's run notification does nothing (NOERROR)
// Leaf: ?Run@CBasePin@@UAEJ_J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CA8530, thiscall: `xor eax,eax ; ret 8` (the REFERENCE_TIME argument popped).
// Identity (vendor naming V2, proven): amfilter.h CBasePin `virtual HRESULT Run(REFERENCE_TIME tStart);`, amfilter.cpp body `return NOERROR;`;
//   V1 LABELS (vtable slot + declaration order) name 0x00CA8530 CBasePin::Run. Was ?Get@T@@QAEHHH@Z (gapscan stand-in).
typedef long HRESULT;
typedef long LONG;
typedef __int64 REFERENCE_TIME;
typedef unsigned short WCHAR;                     // UNICODE strmbase, wchar_t not a native type (LPWSTR mangles PAG)
#define NOERROR 0L
#define E_UNEXPECTED ((HRESULT)0x8000FFFFL)
#define E_NOTIMPL ((HRESULT)0x80004001L)
class CBasePin {                                  // amfilter.h (only the member this row defines)
public:
    virtual HRESULT Run(REFERENCE_TIME tStart);
};

HRESULT CBasePin::Run(REFERENCE_TIME tStart)
{
    return NOERROR;
}
