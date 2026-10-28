// Behaviour test for 00401067: WinMainCRTStartup's command-line walk -- skip the program name (a quoted name may hold
// spaces; a DBCS lead byte takes its trail byte along), then the white space, and pass nShowCmd = wShowWindow when
// STARTF_USESHOWWINDOW (dwFlags & 1) is set, else SW_SHOWDEFAULT (10).
#include <cstdio>
#include <cstring>

static int g_lead = 0x81;                                // model _ismbblead: one lead byte value
static bool ismbblead(unsigned char c) { return c == (unsigned char)g_lead; }

static const char* skip_program_name(const char* cmdline)
{
    const unsigned char* p = (const unsigned char*)cmdline;
    int inDoubleQuote = 0;
    while (*p > ' ' || (*p && inDoubleQuote)) {
        if (*p == '"')
            inDoubleQuote = !inDoubleQuote;
        if (ismbblead(*p)) {
            if (p)
                p++;
        }
        ++p;
    }
    while (*p && (*p <= ' '))
        p++;
    return (const char*)p;
}
static int show_cmd(unsigned long dwFlags, unsigned short wShowWindow) { return dwFlags & 1 ? wShowWindow : 10; }

int main()
{
    bool ok = true;
    ok = ok && std::strcmp(skip_program_name("Fable.exe -nointro"), "-nointro") == 0;
    ok = ok && std::strcmp(skip_program_name("\"C:\\Program Files\\Fable.exe\"   a b"), "a b") == 0;
    ok = ok && std::strcmp(skip_program_name("Fable.exe"), "") == 0;
    ok = ok && std::strcmp(skip_program_name("\"unterminated name x"), "") == 0;
    ok = ok && std::strcmp(skip_program_name("ab\x81 cd ef"), "ef") == 0;   // lead byte swallows the space after it
    ok = ok && std::strcmp(skip_program_name("x\t\t y"), "y") == 0;
    ok = ok && show_cmd(1, 3) == 3 && show_cmd(0, 3) == 10 && show_cmd(0x101, 0) == 0;
    if (!ok) { std::printf("00401067_TEST FAIL\n"); return 1; }
    std::printf("00401067_TEST PASS\n");
    return 0;
}