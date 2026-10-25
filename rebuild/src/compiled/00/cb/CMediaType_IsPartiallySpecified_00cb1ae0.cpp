// CMediaType::IsPartiallySpecified() -- DirectShow mtype.cpp: TRUE when majortype or formattype is GUID_NULL
// Leaf: ?IsPartiallySpecified@CMediaType@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB1AE0, 52 bytes (+ int3 padding). __fastcall const.
// Library: DirectShow base classes (strmbase mtype.cpp). The manifest label "AudioBank_MatchesSignature" is
// descriptive.
// Shape: `if ((majortype == GUID_NULL) || (formattype == GUID_NULL)) return TRUE; else return FALSE;` -- two
// inline memcmp intrinsics (`repe cmpsd` against GUID_NULL @ 0x012C0694) sharing the TRUE block.
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
    int IsPartiallySpecified() const;
};

int CMediaType::IsPartiallySpecified() const
{
    if ((majortype == GUID_NULL) ||
        (formattype == GUID_NULL)) {
        return 1;
    } else {
        return 0;
    }
}