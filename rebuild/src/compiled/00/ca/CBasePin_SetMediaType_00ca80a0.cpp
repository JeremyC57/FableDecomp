// CBasePin::SetMediaType(const CMediaType*) -- the DirectShow base-class member: copy the media type into
// m_mt (CMediaType::Set @ 0x00CB2020 -> 0x00CB1E50 frees the old format block / pUnk and CopyMediaType's,
// E_OUTOFMEMORY on failure) and return S_OK unless that failed. VC7.1 folds `if (FAILED(hr)) return hr;
// return NOERROR;` into the branchless setge/dec/and.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA80A0, length 24 bytes.
// Callees: CMediaType::Set(const CMediaType&) @ 0x00CB2020. m_mt sits at +0x34 of the pin (the AM_MEDIA_TYPE
// fields pUnk +0x3c / cbFormat +0x40 / pbFormat +0x44 of the callee confirm the type). Manifest label
// (CChecksumCacheFile::IsOpen) is propagated garbage.
class CMediaType {                              // AM_MEDIA_TYPE (0x48 bytes) with the base-class helpers
public:
    unsigned char m_mediaType[0x48];
    long Set(const CMediaType& other);          // 0x00CB2020 (HRESULT)
};
class CBasePin {
public:
    unsigned char _pad_0x00[0x34];
    CMediaType    m_mt;                         // +0x34
    long SetMediaType(const CMediaType* pmt);
};

long CBasePin::SetMediaType(const CMediaType* pmt)
{
    long hr = m_mt.Set(*pmt);
    if (hr < 0)
        return hr;
    return 0;
}