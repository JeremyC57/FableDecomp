// CTextureRenderer::CheckMediaType(const CMediaType* pmt) -- accepts only uncompressed RGB24 video: E_INVALIDARG unless the format block is FORMAT_VideoInfo, then S_OK for MEDIATYPE_Video / MEDIASUBTYPE_RGB24, else E_FAIL
// Leaf: ?CheckMediaType@CTextureRenderer
// Retail: Fable.exe @ 0x00A3B590, 87 bytes (+ int3 padding). CTextureRenderer primary vtable 0x0129D08C slot 44
// (CBaseRenderer's pure CheckMediaType). No callees; the three GUIDs are .rdata constants (FORMAT_VideoInfo 0x012AABB4,
// MEDIATYPE_Video 0x012AB884, MEDIASUBTYPE_RGB24 0x012AB604).
// Source: the DirectX 9 SDK Texture3D9 sample's CTextureRenderer::CheckMediaType without its CheckPointer (retail has
// no null test). AM_MEDIA_TYPE: majortype +0, subtype +0x10, formattype +0x2c.
// Shape: each GUID (in)equality is the 16-byte intrinsic memcmp (`mov ecx,4 ; repe cmpsd`); `hr = E_FAIL` is loaded
// before the first branch, the RGB24 test falls into `xor eax,eax`.
#include <string.h>
#pragma intrinsic(memcmp)

struct GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
};
inline int IsEqualGUID(const GUID& a, const GUID& b) { return !memcmp(&a, &b, sizeof(GUID)); }
inline int operator==(const GUID& a, const GUID& b) { return IsEqualGUID(a, b); }
inline int operator!=(const GUID& a, const GUID& b) { return !(a == b); }

extern const GUID FORMAT_VideoInfo;                // 0x012AABB4
extern const GUID MEDIATYPE_Video;                 // 0x012AB884
extern const GUID MEDIASUBTYPE_RGB24;              // 0x012AB604

struct AM_MEDIA_TYPE {
    GUID           majortype;                      // +0x00
    GUID           subtype;                        // +0x10
    int            bFixedSizeSamples;              // +0x20
    int            bTemporalCompression;           // +0x24
    unsigned long  lSampleSize;                    // +0x28
    GUID           formattype;                     // +0x2c
    void*          pUnk;                           // +0x3c
    unsigned long  cbFormat;                       // +0x40
    unsigned char* pbFormat;                       // +0x44
};

class CMediaType : public AM_MEDIA_TYPE {
public:
    const GUID* Type() const { return &majortype; }
    const GUID* Subtype() const { return &subtype; }
    const GUID* FormatType() const { return &formattype; }
    unsigned char* Format() const { return pbFormat; }
};

class CTextureRenderer {
public:
    virtual long CheckMediaType(const CMediaType* pmt);
};

#define S_OK         0L
#define E_FAIL       ((long)0x80004005L)
#define E_INVALIDARG ((long)0x80070057L)

long CTextureRenderer::CheckMediaType(const CMediaType* pmt)
{
    long hr = E_FAIL;

    // Reject the connection if this is not a video type
    if (*pmt->FormatType() != FORMAT_VideoInfo)
        return E_INVALIDARG;

    // Only accept RGB24 video
    if (IsEqualGUID(*pmt->Type(), MEDIATYPE_Video) && IsEqualGUID(*pmt->Subtype(), MEDIASUBTYPE_RGB24))
        hr = S_OK;
    return hr;
}