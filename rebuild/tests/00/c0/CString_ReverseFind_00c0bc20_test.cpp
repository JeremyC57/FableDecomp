// behaviour test for 0x00C0BC20 CString::ReverseFind: index of the last occurrence of a character, -1 when absent
#include <cstdio>
#include <cstring>
#include <stddef.h>

static unsigned char* TestMbsrchr(const unsigned char* s, unsigned int c)   // _mbsrchr stand-in (single-byte text)
{
    const unsigned char* last = 0;
    for (;; ++s) {
        if (*s == c) last = s;
        if (*s == 0) break;
    }
    return (unsigned char*)last;
}

static char* _tcsrchr(const char* s, unsigned int c) { return (char*)TestMbsrchr((const unsigned char*)s, c); }

class CString {
public:
    char* m_pchData;
    int ReverseFind(char ch) const;
};

int CString::ReverseFind(char ch) const
{
    char* lpsz = _tcsrchr(m_pchData, (unsigned char)ch);
    return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

int main()
{
    char text[] = "a/b/c.txt";
    CString s; s.m_pchData = text;
    if (s.ReverseFind('/') != 3 || s.ReverseFind('a') != 0 || s.ReverseFind('t') != 8) { std::printf("00c0bc20_TEST FAIL\n"); return 1; }
    if (s.ReverseFind('z') != -1) { std::printf("00c0bc20_TEST FAIL\n"); return 1; }
    char hi[] = "x\xE9y\xE9";
    s.m_pchData = hi;
    if (s.ReverseFind('\xE9') != 3) { std::printf("00c0bc20_TEST FAIL\n"); return 1; }   // high byte goes in unsigned
    std::printf("00c0bc20_TEST PASS\n");
    return 0;
}