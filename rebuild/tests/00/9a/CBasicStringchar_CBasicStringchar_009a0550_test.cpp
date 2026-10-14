#include <stdio.h>
static unsigned long g_count; static char g_fill; static int g_calls;
struct CBasicString_char {
    char* PData; unsigned long StringLength; unsigned long DataLength : 31; unsigned long m_reserved : 1; unsigned char UseFastExtend : 1;
    void SetString(unsigned long count, char fill) { g_count = count; g_fill = fill; ++g_calls; PData = (char*)1; StringLength = count; }
    CBasicString_char(unsigned long count, char fill) : PData(0), StringLength(0), DataLength(0), UseFastExtend(1) { SetString(count, fill); }
};
int main()
{
    CBasicString_char s(7, 'x');
    bool ok = g_calls == 1 && g_count == 7 && g_fill == 'x' && s.DataLength == 0 && s.UseFastExtend == 1 && s.StringLength == 7 && s.PData == (char*)1;
    if (!ok) { printf("009a0550_TEST FAIL\n"); return 1; }
    printf("009a0550_TEST PASS\n"); return 0;
}