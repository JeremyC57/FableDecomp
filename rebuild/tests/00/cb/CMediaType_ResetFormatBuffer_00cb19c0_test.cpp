// behaviour test for 00cb19c0: CMediaType::ResetFormatBuffer -- CoTaskMemFree the format block only
// when cbFormat is non-zero, then clear cbFormat and pbFormat in every case.
#include <cstdio>
static int g_frees = 0;
static void* g_lastFreed = 0;
static void CoTaskMemFree(void* block) { ++g_frees; g_lastFreed = block; }
struct AM_MEDIA_TYPE {
    unsigned char majortype[16];
    unsigned char subtype[16];
    long bFixedSizeSamples;
    long bTemporalCompression;
    unsigned long lSampleSize;
    unsigned char formattype[16];
    void* pUnk;
    unsigned long cbFormat;
    unsigned char* pbFormat;
};
struct CMediaType : AM_MEDIA_TYPE {
    void ResetFormatBuffer()
    {
        if (cbFormat) {
            CoTaskMemFree((void*)pbFormat);
        }
        cbFormat = 0;
        pbFormat = 0;
    }
};
int main()
{
    int ok = 1;
    unsigned char block[8];
    CMediaType mt;
    mt.cbFormat = 8;
    mt.pbFormat = block;
    mt.ResetFormatBuffer();
    if (g_frees != 1 || g_lastFreed != block || mt.cbFormat != 0 || mt.pbFormat != 0) ok = 0;
    mt.cbFormat = 0;
    mt.pbFormat = block;
    mt.ResetFormatBuffer();
    if (g_frees != 1 || mt.pbFormat != 0) ok = 0;
    if (!ok) { std::printf("00cb19c0_TEST FAIL\n"); return 1; }
    std::printf("00cb19c0_TEST PASS\n");
    return 0;
}