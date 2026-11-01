// Test for CString::Replace(const char*, const char*) -- the MFC strex.cpp algorithm: count matches, grow if needed, replace in place, return the count.
#include <cstdio>
#include <cstring>
#include <cstdlib>
struct CString {
    char* m_pchData;
    explicit CString(const char* s) { m_pchData = (char*)std::malloc(std::strlen(s) + 1); std::strcpy(m_pchData, s); }
    ~CString() { std::free(m_pchData); }
    int Replace(const char* lpszOld, const char* lpszNew)
    {
        int nSourceLen = (int)std::strlen(lpszOld);
        if (nSourceLen == 0) return 0;
        int nReplacementLen = lpszNew ? (int)std::strlen(lpszNew) : 0;
        int nCount = 0;
        char* lpszStart = m_pchData;
        char* lpszEnd = m_pchData + std::strlen(m_pchData);
        char* lpszTarget;
        while (lpszStart < lpszEnd) {
            while ((lpszTarget = std::strstr(lpszStart, lpszOld)) != 0) { nCount++; lpszStart = lpszTarget + nSourceLen; }
            lpszStart += std::strlen(lpszStart) + 1;
        }
        if (nCount > 0) {
            int nOldLength = (int)std::strlen(m_pchData);
            int nNewLength = nOldLength + (nReplacementLen - nSourceLen) * nCount;
            char* p = (char*)std::malloc((nNewLength > nOldLength ? nNewLength : nOldLength) + 1);
            std::strcpy(p, m_pchData); std::free(m_pchData); m_pchData = p;
            lpszStart = m_pchData; lpszEnd = m_pchData + nOldLength;
            while (lpszStart < lpszEnd) {
                while ((lpszTarget = std::strstr(lpszStart, lpszOld)) != 0) {
                    int nBalance = nOldLength - (int)(lpszTarget - m_pchData + nSourceLen);
                    std::memmove(lpszTarget + nReplacementLen, lpszTarget + nSourceLen, nBalance);
                    std::memcpy(lpszTarget, lpszNew, nReplacementLen);
                    lpszStart = lpszTarget + nReplacementLen;
                    lpszStart[nBalance] = '\0';
                    nOldLength += (nReplacementLen - nSourceLen);
                }
                lpszStart += std::strlen(lpszStart) + 1;
            }
            if (m_pchData[nNewLength] != '\0') return -1;
        }
        return nCount;
    }
};
int main()
{
    CString a("aXbXc");
    int n = a.Replace("X", "YY");
    CString b("hello world");
    int m = b.Replace("o", "");
    CString c("abc");
    int k = c.Replace("z", "q");
    if (n != 2 || std::strcmp(a.m_pchData, "aYYbYYc") != 0 || m != 2 || std::strcmp(b.m_pchData, "hell wrld") != 0 || k != 0 || std::strcmp(c.m_pchData, "abc") != 0)
    { std::printf("00c0ce50_TEST FAIL\n"); return 1; }
    std::printf("00c0ce50_TEST PASS\n");
    return 0;
}