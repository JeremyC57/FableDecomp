// CMediaType::MatchesPartial(const CMediaType* ppartial) -- DirectShow mtype.cpp: does this type match every field the partial type specifies (major/sub/format type, format block)
// Leaf: ?MatchesPartial@CMediaType@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB1B20, 170 bytes (+ int3 padding). __fastcall const, `ret 4`.
// Library: DirectShow base classes (strmbase mtype.cpp). The manifest label "AudioBank_CompareTransactionKeys" is
// descriptive.
// Shape: the SDK body verbatim -- each `!= GUID_NULL` and each GUID inequality is an inline memcmp (`repe cmpsd`),
// the format block compare is memcmp(pbFormat, ppartial->pbFormat, cbFormat) as `repe cmpsb`.
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
    int MatchesPartial(const CMediaType* ppartial) const;
};

int CMediaType::MatchesPartial(const CMediaType* ppartial) const
{
    if ((ppartial->majortype != GUID_NULL) &&
        (majortype != ppartial->majortype)) {
        return 0;
    }
    if ((ppartial->subtype != GUID_NULL) &&
        (subtype != ppartial->subtype)) {
        return 0;
    }

    if (ppartial->formattype != GUID_NULL) {
        // if the format block is specified then it must match exactly
        if (formattype != ppartial->formattype) {
            return 0;
        }
        if (cbFormat != ppartial->cbFormat) {
            return 0;
        }
        if ((cbFormat != 0) &&
            (memcmp(pbFormat, ppartial->pbFormat, cbFormat) != 0)) {
            return 0;
        }
    }

    return 1;
}