#include <cstdio>
#include <cstring>
class CString {
public:
    char m_buf[64];
    void Append(double value) { char t[64]; std::sprintf(t, "%.2f", value); std::strcat(m_buf, t); }
    CString& __fastcall operator<<(double value) { Append(value); return *this; }
};
int main()
{
    CString a; a.m_buf[0] = 0;
    CString& r = (a << 1.5 << 2.25);
    if (&r != &a || std::strcmp(a.m_buf, "1.502.25") != 0) { std::printf("00c0ded0_TEST FAIL %s\n", a.m_buf); return 1; }
    std::printf("00c0ded0_TEST PASS\n");
    return 0;
}
