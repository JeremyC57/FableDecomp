#include <cstdio>
#include <cstring>
class CString {
public:
    char m_buf[64];
    int m_calls;
    static int __stdcall SafeStrlen(const char* lpsz) { return lpsz == 0 ? 0 : (int)std::strlen(lpsz); }
    void AssignCopy(int nSrcLen, const char* lpszSrcData)
    {
        ++m_calls;
        if (nSrcLen > 0) std::memcpy(m_buf, lpszSrcData, nSrcLen);
        m_buf[nSrcLen] = 0;
    }
    const CString& operator=(const char* lpsz) { AssignCopy(SafeStrlen(lpsz), lpsz); return *this; }
};
int main()
{
    CString a; a.m_calls = 0; std::strcpy(a.m_buf, "old");
    const CString& r = (a = "Sound");
    if (&r != &a || std::strcmp(a.m_buf, "Sound") != 0) { std::printf("00c0c200_TEST FAIL 1\n"); return 1; }
    a = 0;
    if (a.m_buf[0] != 0 || a.m_calls != 2) { std::printf("00c0c200_TEST FAIL 2\n"); return 1; }
    std::printf("00c0c200_TEST PASS\n");
    return 0;
}
