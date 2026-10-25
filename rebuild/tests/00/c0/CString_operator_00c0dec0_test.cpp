#include <cstdio>
#include <cstring>
class CString {
public:
    char m_buf[64];
    void Append(const char* text) { std::strcat(m_buf, text); }
    CString& __fastcall operator<<(const char* text) { Append(text); return *this; }
};
int main()
{
    CString a; a.m_buf[0] = 0;
    CString& r = (a << "Bank" << "Event");
    if (&r != &a || std::strcmp(a.m_buf, "BankEvent") != 0) { std::printf("00c0dec0_TEST FAIL %s\n", a.m_buf); return 1; }
    std::printf("00c0dec0_TEST PASS\n");
    return 0;
}
