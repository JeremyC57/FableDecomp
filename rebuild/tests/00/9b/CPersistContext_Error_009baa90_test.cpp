// Behaviour test for CPersistContext::Error (0x009BAA90): the callback receives "<file>(<line + 1>) : error : <message>".
#include <cstdio>
#include <cstring>
struct CCharString {
    char text[128];
    CCharString() { text[0] = 0; }
    CCharString(const char* t) { std::strcpy(text, t); }
    ~CCharString() {}
    static CCharString FromLong(long value) { CCharString r; std::sprintf(r.text, "%ld", value); return r; }
};
static CCharString operator+(const CCharString& l, const CCharString& r) { CCharString s(l.text); std::strcat(s.text, r.text); return s; }
static CCharString operator+(const CCharString& l, const char* r) { CCharString s(l.text); std::strcat(s.text, r); return s; }
struct CCursorPos { bool InQuotes; unsigned long StringPos; unsigned long Line; };
struct CStringParser { CCharString Filename; CCursorPos CursorPos; };
static char g_reported[128]; static int g_calls = 0;
static void ReportError(const CCharString& t) { ++g_calls; std::strcpy(g_reported, t.text); }
class CPersistContext {
public:
    void* PDefinitionManager; void* PSymbolMap; void* PParser;
    void (*ErrorCallback)(const CCharString& text);
    void Error(const CStringParser& parser, const CCharString& message);
};
void CPersistContext::Error(const CStringParser& parser, const CCharString& message)
{
    long line = parser.CursorPos.Line;
    CCharString text = parser.Filename + "(" + CCharString::FromLong(line + 1) + ")" + " : error : " + message;
    ErrorCallback(text);
}
int main()
{
    CPersistContext m; m.ErrorCallback = ReportError;
    CStringParser p; p.Filename = CCharString("defs\\objects.def"); p.CursorPos.Line = 41;
    m.Error(p, CCharString("unknown token"));
    int bad = g_calls != 1 || std::strcmp(g_reported, "defs\\objects.def(42) : error : unknown token");
    if (bad) { std::printf("009baa90_TEST FAIL\n"); return 1; }
    std::printf("009baa90_TEST PASS\n");
    return 0;
}