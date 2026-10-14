// Behaviour test for JenkinsHashOfName @ 0x00c0dbb0: hash(name, SafeStringLength(name), 0).
#include <cstdio>
#include <cstring>
unsigned long __stdcall SafeStringLength(const char* text) { return text ? (unsigned long)std::strlen(text) : 0; }
unsigned long __fastcall JenkinsHashBuffer(const char* data, unsigned long length, unsigned long seed)
{ unsigned long h = seed; for (unsigned long i = 0; i < length; ++i) h = h * 31 + (unsigned char)data[i]; return h * 7 + length; }
struct CNamedRecord_00c0dbb0 { const char* m_pName; unsigned long JenkinsHashOfName() const { return JenkinsHashBuffer(m_pName, SafeStringLength(m_pName), 0); } };
int main()
{
    CNamedRecord_00c0dbb0 r; r.m_pName = "abc";
    unsigned long expect = (((('a' * 31) + 'b') * 31) + 'c') * 7 + 3;
    bool ok = r.JenkinsHashOfName() == expect;
    r.m_pName = ""; if (r.JenkinsHashOfName() != 0) ok = false;
    r.m_pName = 0; if (r.JenkinsHashOfName() != 0) ok = false;
    if (!ok) { std::printf("00c0dbb0_TEST FAIL\n"); return 1; }
    std::printf("00c0dbb0_TEST PASS\n");
    return 0;
}
