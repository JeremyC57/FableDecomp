#include <cstdio>
#include <cstring>
class CString {
public:
    char m_buf[32];
    void Append(const CString& string) { std::strcat(m_buf, string.m_buf); }
    CString& __fastcall operator<<(const CString& string) { Append(string); return *this; }
};
int main()
{
    CString a, b, c; std::strcpy(a.m_buf, "Bank"); std::strcpy(b.m_buf, "Event"); std::strcpy(c.m_buf, "!");
    CString& r = (a << b << c);
    if (&r != &a || std::strcmp(a.m_buf, "BankEvent!") != 0) { std::printf("00c0dea0_TEST FAIL\n"); return 1; }
    std::printf("00c0dea0_TEST PASS\n");
    return 0;
}
