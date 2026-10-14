#include <cstdio>
#include <cstring>
#include <cstdlib>
struct CStringData { long nRefs; long nDataLength; long nAllocLength; };
static int g_allocs = 0;
class CString {
public:
    char* m_pchData;
    CString() : m_pchData(0) { }
    void AllocBuffer(int nLen) { ++g_allocs; CStringData* d = (CStringData*)std::malloc(sizeof(CStringData) + nLen + 1); d->nRefs = 1; d->nDataLength = nLen; d->nAllocLength = nLen; m_pchData = (char*)(d + 1); m_pchData[nLen] = 0; }
    void ConcatCopy(int nSrc1Len, const char* lpszSrc1Data, int nSrc2Len, const char* lpszSrc2Data)
    {
        int nNewLen = nSrc1Len + nSrc2Len;
        if (nNewLen != 0) {
            AllocBuffer(nNewLen);
            std::memcpy(m_pchData, lpszSrc1Data, nSrc1Len * sizeof(char));
            std::memcpy(m_pchData + nSrc1Len, lpszSrc2Data, nSrc2Len * sizeof(char));
        }
    }
};
int main()
{
    CString s; s.ConcatCopy(2, "abXX", 3, "cdeYY");
    if (g_allocs != 1 || std::strcmp(s.m_pchData, "abcde") != 0) { std::printf("00c0c220_TEST FAIL\n"); return 1; }
    CString t; t.ConcatCopy(0, "", 0, "");
    if (g_allocs != 1 || t.m_pchData != 0) { std::printf("00c0c220_TEST FAIL\n"); return 1; }
    CString u; u.ConcatCopy(0, "", 4, "wxyz");
    if (std::strcmp(u.m_pchData, "wxyz") != 0) { std::printf("00c0c220_TEST FAIL\n"); return 1; }
    std::printf("00c0c220_TEST PASS\n"); return 0;
}