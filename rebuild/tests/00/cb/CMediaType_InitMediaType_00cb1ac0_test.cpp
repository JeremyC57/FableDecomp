// Behaviour test for 00cb1ac0: CMediaType::InitMediaType -- all 0x48 bytes zeroed, lSampleSize = 1, bFixedSizeSamples = TRUE
#include <cstdio>
#include <cstring>
struct GUID { unsigned long Data1; unsigned short Data2, Data3; unsigned char Data4[8]; };
struct AM_MEDIA_TYPE { GUID majortype; GUID subtype; int bFixedSizeSamples; int bTemporalCompression; unsigned long lSampleSize;
                       GUID formattype; void* pUnk; unsigned long cbFormat; unsigned char* pbFormat; };
struct CMediaType : AM_MEDIA_TYPE { void InitMediaType() { std::memset(this, 0, sizeof(*this)); lSampleSize = 1; bFixedSizeSamples = 1; } };
int main()
{
    CMediaType mt; std::memset(&mt, 0xAB, sizeof mt); mt.InitMediaType();
    const unsigned char* b = (const unsigned char*)&mt; int zero = 0;
    for (unsigned int i = 0; i < sizeof mt; ++i) if (b[i] == 0) ++zero;
    int ok = sizeof mt == 0x48 && mt.lSampleSize == 1 && mt.bFixedSizeSamples == 1 && mt.bTemporalCompression == 0 && mt.pUnk == 0 && mt.cbFormat == 0 && zero == 0x48 - 2;
    std::printf(ok ? "00cb1ac0_TEST PASS\n" : "00cb1ac0_TEST FAIL\n");
    return ok ? 0 : 1;
}
