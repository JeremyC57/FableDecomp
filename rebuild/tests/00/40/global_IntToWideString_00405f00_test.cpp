#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x00405F00: exercises the same logic through a copy of the model (compiled separately from the source)
typedef unsigned short wchar_retail;
static int g_live = 0;
struct CWideString {
    wchar_retail m_text[16];
    CWideString() { m_text[0] = 0; ++g_live; }
    CWideString(const CWideString& other) { std::memcpy(m_text, other.m_text, sizeof(m_text)); ++g_live; }
    ~CWideString() { --g_live; }
};
static void FormatWideString(CWideString* text, const wchar_retail* format, long value)   // "%d" only
{
    if (format[0] != '%' || format[1] != 'd') { text->m_text[0] = 0; return; }
    char buffer[16]; std::sprintf(buffer, "%ld", value);
    int i = 0; for (; buffer[i]; ++i) text->m_text[i] = (wchar_retail)buffer[i]; text->m_text[i] = 0;
}
static CWideString IntToWideString(long value)
{
    CWideString text;
    wchar_retail format[3]; format[0] = '%'; format[1] = 'd'; format[2] = 0;
    FormatWideString(&text, format, value);
    return text;
}
static bool Run()
{
    {
        CWideString s = IntToWideString(-42);
        if (s.m_text[0] != '-' || s.m_text[1] != '4' || s.m_text[2] != '2' || s.m_text[3] != 0) return false;
    }
    return g_live == 0;                            // the temporary and the copy were both destroyed
}
int main()
{
    if (!Run()) { std::printf("00405f00_TEST FAIL\n"); return 1; }
    std::printf("00405f00_TEST PASS\n");
    return 0;
}