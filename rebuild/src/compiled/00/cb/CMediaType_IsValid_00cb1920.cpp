// CMediaType::IsValid() -- DirectShow mtype.cpp: TRUE when majortype is not GUID_NULL
// Leaf: ?IsValid@CMediaType@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB1920, 24 bytes. __fastcall const, no arguments.
// Library: DirectShow base classes (strmbase mtype.cpp). The manifest label "Std_MemCmp_Dwords" is descriptive.
// Shape: `return (majortype != GUID_NULL);` -- the guiddef.h operator!= over IsEqualGUID's !memcmp, which /O2
// inlines as `repe cmpsd` against GUID_NULL @ 0x012C0694 and a `setne al`.
#include <string.h>

struct IUnknown_mt {
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1 (+0x04)
    virtual unsigned long __stdcall Release();                               // slot 2 (+0x08)
};

struct GUID { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };

// AM_MEDIA_TYPE (strmif.h _AMMediaType), 0x48 bytes; named like the landed CMediaType rows 0x00CB19C0 / 0x00CB1E50.
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

extern "C" const GUID GUID_NULL;                  // 0x012C0694

// guiddef.h: IsEqualGUID is !memcmp (the intrinsic, `repe cmpsd`); operator== / operator!= wrap it.
inline int IsEqualGUID(const GUID& rguid1, const GUID& rguid2)
{
    return !memcmp(&rguid1, &rguid2, sizeof(GUID));
}
inline int operator==(const GUID& guidOne, const GUID& guidOther)
{
    return IsEqualGUID(guidOne, guidOther);
}
inline int operator!=(const GUID& guidOne, const GUID& guidOther)
{
    return !(guidOne == guidOther);
}

class CMediaType : public AM_MEDIA_TYPE {
public:
    int IsValid() const;
};

int CMediaType::IsValid() const
{
    return (majortype != GUID_NULL);
}