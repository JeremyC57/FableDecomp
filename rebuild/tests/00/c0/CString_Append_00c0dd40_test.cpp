#include <cstdio>
#include <cstring>
class CString {
public:
    char m_buf[64];
    static int __stdcall SafeStrlen(const char* lpsz) { return lpsz == 0 ? 0 : (int)std::strlen(lpsz); }
    void ConcatInPlace(int nSrcLen, const char* lpszSrcData) { std::strncat(m_buf, lpszSrcData ? lpszSrcData : "", nSrcLen); }
    void Append(const char* text) { ConcatInPlace(SafeStrlen(text), text); }
};
int main()
{
    CString a; std::strcpy(a.m_buf, "Bank");
    a.Append("Event"); a.Append(0); a.Append("");
    if (std::strcmp(a.m_buf, "BankEvent") != 0) { std::printf("00c0dd40_TEST FAIL %s\n", a.m_buf); return 1; }
    std::printf("00c0dd40_TEST PASS\n");
    return 0;
}
