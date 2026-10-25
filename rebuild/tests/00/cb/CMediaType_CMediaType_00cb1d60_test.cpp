// Behaviour test for 0x00CB1D60 CMediaType::CMediaType(const AM_MEDIA_TYPE&, HRESULT*): CopyMediaType into this, failure stored through phr.
#include <cstdio>
#include <cstdlib>
#include <cstring>

static int g_failAlloc = 0, g_allocs = 0, g_frees = 0;
static void* CoTaskMemAlloc_(unsigned int cb) { if (g_failAlloc) return 0; ++g_allocs; return std::malloc(cb ? cb : 1); }
static void CoTaskMemFree_(void* pv) { ++g_frees; std::free(pv); }

struct IUnknown_mt {
    int refs;
    IUnknown_mt() : refs(1) {}
    unsigned int AddRef() { return (unsigned int)++refs; }
    unsigned int Release() { return (unsigned int)--refs; }
};

struct GUID { unsigned int Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };

struct AM_MEDIA_TYPE {
    GUID           majortype;
    GUID           subtype;
    int            bFixedSizeSamples;
    int            bTemporalCompression;
    unsigned int   lSampleSize;
    GUID           formattype;
    IUnknown_mt*   pUnk;
    unsigned int   cbFormat;
    unsigned char* pbFormat;
};

static const GUID GUID_NULL = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
static const GUID GUID_A = { 0x73646976, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71 } };
static const GUID GUID_B = { 0x73647561, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71 } };
static const GUID GUID_C = { 0x05589F80, 0xC356, 0x11CE, { 0xBF, 0x01, 0x00, 0xAA, 0x00, 0x55, 0x59, 0x5A } };

inline int IsEqualGUID(const GUID& a, const GUID& b) { return !std::memcmp(&a, &b, sizeof(GUID)); }
inline int operator==(const GUID& a, const GUID& b) { return IsEqualGUID(a, b); }
inline int operator!=(const GUID& a, const GUID& b) { return !(a == b); }

static int CopyMediaType(AM_MEDIA_TYPE* pmtTarget, const AM_MEDIA_TYPE* pmtSource)
{
    *pmtTarget = *pmtSource;
    if (pmtSource->cbFormat != 0) {
        pmtTarget->pbFormat = (unsigned char*)CoTaskMemAlloc_(pmtSource->cbFormat);
        if (pmtTarget->pbFormat == 0) {
            pmtTarget->cbFormat = 0;
            return (int)0x8007000E;
        } else {
            std::memcpy(pmtTarget->pbFormat, pmtSource->pbFormat, pmtTarget->cbFormat);
        }
    }
    if (pmtTarget->pUnk != 0)
        pmtTarget->pUnk->AddRef();
    return 0;
}

static AM_MEDIA_TYPE* CreateMediaType(const AM_MEDIA_TYPE* pSrc)
{
    AM_MEDIA_TYPE* pMediaType = (AM_MEDIA_TYPE*)CoTaskMemAlloc_(sizeof(AM_MEDIA_TYPE));
    if (pMediaType == 0)
        return 0;
    int hr = CopyMediaType(pMediaType, pSrc);
    if (hr < 0) {
        CoTaskMemFree_(pMediaType);
        return 0;
    }
    return pMediaType;
}

#define min_(a,b) (((a) < (b)) ? (a) : (b))

class CMediaType : public AM_MEDIA_TYPE {
public:
    CMediaType() { std::memset((AM_MEDIA_TYPE*)this, 0, sizeof(AM_MEDIA_TYPE)); }
    CMediaType(const AM_MEDIA_TYPE& rt, int* phr = 0)
    {
        int hr = CopyMediaType(this, &rt);
        if ((hr < 0) && (0 != phr))
            *phr = hr;
    }
    int IsValid() const { return (majortype != GUID_NULL); }
    int IsPartiallySpecified() const
    {
        if ((majortype == GUID_NULL) || (formattype == GUID_NULL))
            return 1;
        else
            return 0;
    }
    int MatchesPartial(const CMediaType* ppartial) const
    {
        if ((ppartial->majortype != GUID_NULL) && (majortype != ppartial->majortype))
            return 0;
        if ((ppartial->subtype != GUID_NULL) && (subtype != ppartial->subtype))
            return 0;
        if (ppartial->formattype != GUID_NULL) {
            if (formattype != ppartial->formattype)
                return 0;
            if (cbFormat != ppartial->cbFormat)
                return 0;
            if ((cbFormat != 0) && (std::memcmp(pbFormat, ppartial->pbFormat, cbFormat) != 0))
                return 0;
        }
        return 1;
    }
    unsigned char* AllocFormatBuffer(unsigned int length)
    {
        if (cbFormat == length)
            return pbFormat;
        unsigned char* pNewFormat = (unsigned char*)CoTaskMemAlloc_(length);
        if (pNewFormat == 0) {
            if (length <= cbFormat) return pbFormat;
            return 0;
        }
        if (cbFormat != 0)
            CoTaskMemFree_(pbFormat);
        cbFormat = length;
        pbFormat = pNewFormat;
        return pbFormat;
    }
    unsigned char* ReallocFormatBuffer(unsigned int length)
    {
        if (cbFormat == length)
            return pbFormat;
        unsigned char* pNewFormat = (unsigned char*)CoTaskMemAlloc_(length);
        if (pNewFormat == 0) {
            if (length <= cbFormat) return pbFormat;
            return 0;
        }
        if (cbFormat != 0) {
            std::memcpy(pNewFormat, pbFormat, min_(length, cbFormat));
            CoTaskMemFree_(pbFormat);
        }
        cbFormat = length;
        pbFormat = pNewFormat;
        return pNewFormat;
    }
};

int main()
{
    bool ok = true;

    unsigned char fmt[2] = { 5, 6 };
    CMediaType src;
    src.majortype = GUID_A; src.cbFormat = 2; src.pbFormat = fmt;
    int hr = 123;
    CMediaType a(src, &hr);
    ok = ok && hr == 123 && a.majortype == GUID_A && a.cbFormat == 2 && a.pbFormat != fmt && a.pbFormat[1] == 6;
    CoTaskMemFree_(a.pbFormat);
    g_failAlloc = 1;
    CMediaType b(src, &hr);
    ok = ok && hr == (int)0x8007000E && b.cbFormat == 0;
    CMediaType c((const AM_MEDIA_TYPE&)src);   // no phr: nothing stored
    ok = ok && c.cbFormat == 0;
    g_failAlloc = 0;
    if (!ok) { std::printf("00cb1d60_TEST FAIL\n"); return 1; }
    std::printf("00cb1d60_TEST PASS\n");
    return 0;
}