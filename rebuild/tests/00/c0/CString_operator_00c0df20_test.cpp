#include <cstdio>
#include <cstring>
class CString {
public:
    char m_buf[64];
    void Append(const void* pointer) { char t[64]; std::sprintf(t, "%p", pointer); std::strcat(m_buf, t); }
    CString& __fastcall operator<<(const void* pointer) { Append(pointer); return *this; }
};
int main()
{
    CString a; a.m_buf[0] = 0;
    char expect[64]; std::sprintf(expect, "%p%p", (const void*)0x10, (const void*)0x20);
    CString& r = (a << (const void*)0x10 << (const void*)0x20);
    if (&r != &a || std::strcmp(a.m_buf, expect) != 0) { std::printf("00c0df20_TEST FAIL %s\n", a.m_buf); return 1; }
    std::printf("00c0df20_TEST PASS\n");
    return 0;
}
