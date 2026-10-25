// Behaviour test for CString::GetHash @ 0x00c0dbb0: hash(text, SafeStrlen(text), 0).
#include <cstdio>
#include <cstring>
unsigned long __fastcall JenkinsHashBuffer(const char* data, unsigned long length, unsigned long seed)
{ unsigned long h = seed; for (unsigned long i = 0; i < length; ++i) h = h * 31 + (unsigned char)data[i]; return h * 7 + length; }
class CString {
public:
    char* m_pchData;
    static int __stdcall SafeStrlen(const char* lpsz) { return lpsz ? (int)std::strlen(lpsz) : 0; }
    unsigned long GetHash() const { return JenkinsHashBuffer(m_pchData, SafeStrlen(m_pchData), 0); }
};
int main()
{
    char abc[] = "abc", empty[] = "";
    CString r; r.m_pchData = abc;
    unsigned long expect = (((('a' * 31) + 'b') * 31) + 'c') * 7 + 3;
    bool ok = r.GetHash() == expect;
    r.m_pchData = empty; if (r.GetHash() != 0) ok = false;
    r.m_pchData = 0; if (r.GetHash() != 0) ok = false;
    if (!ok) { std::printf("00c0dbb0_TEST FAIL\n"); return 1; }
    std::printf("00c0dbb0_TEST PASS\n");
    return 0;
}
