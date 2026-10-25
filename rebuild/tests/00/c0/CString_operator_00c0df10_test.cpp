#include <cstdio>
#include <cstring>
class CString {
public:
    char m_buf[64];
    void Append(unsigned long value) { char t[64]; std::sprintf(t, "%d", (int)value); std::strcat(m_buf, t); }
    CString& __fastcall operator<<(unsigned long value) { Append(value); return *this; }
};
int main()
{
    CString a; a.m_buf[0] = 0;
    CString& r = (a << 7UL << 12UL);
    if (&r != &a || std::strcmp(a.m_buf, "712") != 0) { std::printf("00c0df10_TEST FAIL %s\n", a.m_buf); return 1; }
    std::printf("00c0df10_TEST PASS\n");
    return 0;
}
