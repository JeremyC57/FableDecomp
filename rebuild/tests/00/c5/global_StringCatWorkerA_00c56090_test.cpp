#include <cstdio>
#include <cstring>
#define STRSAFE_E_INSUFFICIENT_BUFFER ((int)0x8007007AL)
#define STRSAFE_E_INVALID_PARAMETER   ((int)0x80070057L)
static int StringCopyWorkerA(char* pszDest, unsigned int cchDest, const char* pszSrc)
{
    int hr = 0;
    if (cchDest == 0)
        hr = STRSAFE_E_INVALID_PARAMETER;
    else {
        while (cchDest && (*pszSrc != '\0')) { *pszDest++ = *pszSrc++; cchDest--; }
        if (cchDest == 0) { pszDest--; hr = STRSAFE_E_INSUFFICIENT_BUFFER; }
        *pszDest = '\0';
    }
    return hr;
}
static int StringLengthWorkerA(const char* psz, unsigned int cchMax, unsigned int* pcch)
{
    int hr = 0;
    unsigned int cchMaxPrev = cchMax;
    while (cchMax && (*psz != '\0')) { psz++; cchMax--; }
    if (cchMax == 0) hr = STRSAFE_E_INVALID_PARAMETER;
    if (hr >= 0 && pcch) *pcch = cchMaxPrev - cchMax;
    return hr;
}
static int StringCatWorkerA(char* pszDest, unsigned int cchDest, const char* pszSrc)
{
    int hr;
    unsigned int cchDestCurrent;
    hr = StringLengthWorkerA(pszDest, cchDest, &cchDestCurrent);
    if (hr >= 0)
        hr = StringCopyWorkerA(pszDest + cchDestCurrent, cchDest - cchDestCurrent, pszSrc);
    return hr;
}
#define CHECK(x) do { if (!(x)) { std::printf("00c56090_TEST FAIL\n"); return 1; } } while (0)
int main()
{
    char buf[8];
    std::strcpy(buf, "ab");
    CHECK(StringCatWorkerA(buf, 8, "cd") == 0 && std::strcmp(buf, "abcd") == 0);
    CHECK(StringCatWorkerA(buf, 8, "efghij") == STRSAFE_E_INSUFFICIENT_BUFFER && std::strcmp(buf, "abcdefg") == 0);
    std::memcpy(buf, "12345678", 8);   // no terminator inside the buffer
    CHECK(StringCatWorkerA(buf, 8, "x") == STRSAFE_E_INVALID_PARAMETER && buf[7] == '8');
    CHECK(StringCatWorkerA(buf, 0, "x") == STRSAFE_E_INVALID_PARAMETER);
    buf[0] = 0;
    CHECK(StringCatWorkerA(buf, 3, "xy") == 0 && std::strcmp(buf, "xy") == 0);
    std::printf("00c56090_TEST PASS\n");
    return 0;
}