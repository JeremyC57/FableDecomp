#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model.
struct GUID { unsigned int Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
inline int IsEqualGUID(const GUID& a, const GUID& b) { return !std::memcmp(&a, &b, sizeof(GUID)); }
struct CMediaType {
    GUID majortype, subtype; int bFixed, bTemporal; unsigned int lSampleSize; GUID formattype; void* pUnk; unsigned int cbFormat; unsigned char* pbFormat;
    int operator==(const CMediaType& rt) const
    {
        return ((IsEqualGUID(majortype, rt.majortype) == 1) &&
                (IsEqualGUID(subtype, rt.subtype) == 1) &&
                (IsEqualGUID(formattype, rt.formattype) == 1) &&
                (cbFormat == rt.cbFormat) &&
                ((cbFormat == 0) || (std::memcmp(pbFormat, rt.pbFormat, cbFormat) == 0)));
    }
};
int main()
{
    int ok = 1;
    unsigned char f1[4] = { 1, 2, 3, 4 }, f2[4] = { 1, 2, 3, 4 }, f3[4] = { 1, 2, 9, 4 };
    CMediaType a; std::memset(&a, 0, sizeof a); a.majortype.Data1 = 1; a.subtype.Data1 = 2; a.formattype.Data1 = 3; a.cbFormat = 4; a.pbFormat = f1;
    CMediaType b = a; b.pbFormat = f2; b.lSampleSize = 99;
    ok = (a == b) == 1;
    b.pbFormat = f3; ok = ok && (a == b) == 0;
    b.pbFormat = f2; b.subtype.Data4[7] = 1; ok = ok && (a == b) == 0;
    b.subtype.Data4[7] = 0; b.cbFormat = 3; ok = ok && (a == b) == 0;
    a.cbFormat = b.cbFormat = 0; a.pbFormat = 0; b.pbFormat = f3; ok = ok && (a == b) == 1;
    if (!ok) { std::printf("00cb1dc0_TEST FAIL\n"); return 1; }
    std::printf("00cb1dc0_TEST PASS\n");
    return 0;
}