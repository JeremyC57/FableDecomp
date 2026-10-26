// Behaviour test for CSymbolMap::Error (0x009E4F80): the compiler-style error text it builds.
#include <cstdio>
#include <cstring>
#include <cstdarg>

class CCharString { public:
    char text[512];
    CCharString() { text[0] = 0; }
    CCharString(const char* t, long) { std::strcpy(text, t); }
    CCharString& operator=(const CCharString& o) { std::strcpy(text, o.text); return *this; }
};
static CCharString operator+(const CCharString& a, const CCharString& b) { CCharString r(a); std::strcat(r.text, b.text); return r; }
static CCharString operator+(const CCharString& a, const char* b) { CCharString r(a); std::strcat(r.text, b); return r; }
class CWideString { public:
    const char* narrow;
    CCharString ConvertToCharString() const { return CCharString(narrow, -1); }
};
static char g_last[512];
class CSymbolMap {
public:
    void Error(const CWideString& file, long line, const char* format, ...)
    {
        CCharString errorText;
        char lineText[12];
        char message[256];
        std::sprintf(lineText, "(%d)", (int)(line + 1));   // _snprintf(lineText, 9, ...) in the retail body
        lineText[9] = 0;
        va_list args;
        va_start(args, format);
        std::vsprintf(message, format, args);
        va_end(args);
        errorText = CCharString("", -1) + file.ConvertToCharString() + lineText + " : error C2065 : " + message;
        std::strcpy(g_last, errorText.text);      // test hook: retail drops the text
    }
};

int main()
{
    CSymbolMap map;
    CWideString file; file.narrow = "defs\\thing.def";
    map.Error(file, 41, "symbol '%s' (%d) undefined", "FOO", 7);
    if (std::strcmp(g_last, "defs\\thing.def(42) : error C2065 : symbol 'FOO' (7) undefined") != 0) {
        std::printf("009e4f80_TEST FAIL\n");
        return 1;
    }
    std::printf("009e4f80_TEST PASS\n");
    return 0;
}