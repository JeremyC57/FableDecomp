// CMediaType::operator==(const CMediaType& rt) const -- DirectShow mtype.cpp: two media types are equal when major type, subtype, format type, format size and format block match
// Leaf: ??8CMediaType@@QBEHABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB1DC0, 101 bytes, thiscall, `ret 4`, BOOL result.
// Identity (proven by source): DirectShow base classes (strmbase mtype.cpp) CMediaType::operator== -- IsEqualGUID on majortype (+0),
//   subtype (+0x10) and formattype (+0x2c) (each an inline memcmp, `repe cmpsd`), cbFormat (+0x40) equal, then cbFormat == 0 or
//   memcmp(pbFormat (+0x44), rt.pbFormat, cbFormat) == 0 (`repe cmpsb`). It sits among the landed CMediaType rows 0x00CB19C0..0x00CB1E50
//   (model reused from 0x00CB1B20).
// Callees: none (all compares are intrinsic memcmp).
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
    int operator==(const CMediaType& rt) const;
};

int CMediaType::operator==(const CMediaType& rt) const
{
    return ((IsEqualGUID(majortype, rt.majortype) == 1) &&
            (IsEqualGUID(subtype, rt.subtype) == 1) &&
            (IsEqualGUID(formattype, rt.formattype) == 1) &&
            (cbFormat == rt.cbFormat) &&
            ((cbFormat == 0) ||
             (memcmp(pbFormat, rt.pbFormat, cbFormat) == 0)));
}