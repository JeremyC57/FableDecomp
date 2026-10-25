#include <cstdio>
#include <cstring>
class CString {
public:
    char m_buf[64];
    void Append(long value) { char t[64]; std::sprintf(t, "%d", (int)value); std::strcat(m_buf, t); }
    CString& __fastcall operator<<(long value) { Append(value); return *this; }
};
int main()
{
    CString a; a.m_buf[0] = 0;
    CString& r = (a << 54L << -3L);
    if (&r != &a || std::strcmp(a.m_buf, "54-3") != 0) { std::printf("00c0df00_TEST FAIL %s\n", a.m_buf); return 1; }
    std::printf("00c0df00_TEST PASS\n");
    return 0;
}
