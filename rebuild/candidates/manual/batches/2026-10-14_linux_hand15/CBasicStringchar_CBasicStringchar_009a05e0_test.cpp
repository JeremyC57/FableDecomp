#include <stdio.h>
static const char* g_text; static unsigned long g_len; static int g_calls;
struct CBasicString_char {
    char* PData; unsigned long StringLength; unsigned long DataLength : 31; unsigned long m_reserved : 1; unsigned char UseFastExtend : 1;
    void AssignFromData(const char* text, unsigned long length) { g_text = text; g_len = length; ++g_calls; PData = (char*)text; StringLength = length; }
    CBasicString_char(const char* text, unsigned long length) : PData(0), StringLength(0), DataLength(0), UseFastExtend(1) { AssignFromData(text, length); }
};
int main()
{
    CBasicString_char s("hello", 5);
    bool ok = g_calls == 1 && g_len == 5 && g_text == s.PData && s.DataLength == 0 && s.UseFastExtend == 1 && s.StringLength == 5;
    if (!ok) { printf("009a05e0_TEST FAIL\n"); return 1; }
    printf("009a05e0_TEST PASS\n"); return 0;
}
