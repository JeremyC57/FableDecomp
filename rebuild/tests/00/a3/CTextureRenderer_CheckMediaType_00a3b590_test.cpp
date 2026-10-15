#include <cstdio>
#include <cstring>
struct GUID { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
static const GUID FORMAT_VideoInfo   = { 0x05589f80, 0xc356, 0x11ce, { 0xbf, 0x01, 0x00, 0xaa, 0x00, 0x55, 0x59, 0x5a } };
static const GUID MEDIATYPE_Video    = { 0x73646976, 0x0000, 0x0010, { 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 } };
static const GUID MEDIASUBTYPE_RGB24 = { 0xe436eb7d, 0x524f, 0x11ce, { 0x9f, 0x53, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70 } };
static int Eq(const GUID& a, const GUID& b) { return !std::memcmp(&a, &b, sizeof(GUID)); }
struct CMediaType { GUID majortype, subtype, formattype; };
static long CheckMediaType(const CMediaType* pmt)
{
    long hr = (long)0x80004005L;
    if (!Eq(pmt->formattype, FORMAT_VideoInfo)) return (long)0x80070057L;
    if (Eq(pmt->majortype, MEDIATYPE_Video) && Eq(pmt->subtype, MEDIASUBTYPE_RGB24)) hr = 0;
    return hr;
}
int main()
{
    CMediaType t; t.majortype = MEDIATYPE_Video; t.subtype = MEDIASUBTYPE_RGB24; t.formattype = FORMAT_VideoInfo;
    bool ok = CheckMediaType(&t) == 0;
    t.subtype = MEDIATYPE_Video; ok = ok && CheckMediaType(&t) == (long)0x80004005L;
    t.formattype = MEDIATYPE_Video; ok = ok && CheckMediaType(&t) == (long)0x80070057L;
    if (!ok) { std::printf("00a3b590_TEST FAIL\n"); return 1; }
    std::printf("00a3b590_TEST PASS\n");
    return 0;
}