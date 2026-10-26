// Behaviour test for 0x009BA6F0: the reconstructed logic over a host model.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
// Host model of the parser pieces the reconstructed reader uses: a string type, the parsed item and a scripted ReadNextItem.
struct Str { char s[64]; Str() { s[0] = 0; } Str(const char* t) { std::strcpy(s, t); } bool eq(const char* t) const { return std::strcmp(s, t) == 0; } };
enum EParsedItemType { PIT_NONE = 0, PIT_SYMBOL = 1, PIT_STRING = 2, PIT_INTEGER = 3, PIT_FLOAT = 4 };
class CParsedItem { public: EParsedItemType Type; union { int Integer; float Float; }; Str String; };
struct CCursorPos { bool InQuotes; unsigned int StringPos; unsigned int Line; };
struct Parser {
    bool ok; CParsedItem next; Str lastError; int errors; CCursorPos CursorPos;
    Parser() : ok(false), errors(0) { CursorPos.InQuotes = false; CursorPos.StringPos = 0; CursorPos.Line = 0; }
    bool ReadNextItem(CParsedItem& item) { CursorPos.StringPos += 5; CursorPos.InQuotes = !CursorPos.InQuotes; if (!ok) return false; item.Type = next.Type; item.Integer = next.Integer; item.String = next.String; return true; }
    void Error(const Str& m) { lastError = m; ++errors; }
    Str ReadAsString()
    {
        CParsedItem item;
        if (ReadNextItem(item) && item.Type == PIT_STRING)
            return item.String;
        { Str message("Error parsing string"); Error(message); }
        return Str("");
    }
};
static void Run()
{
    Parser p; p.ok = true; p.next.Type = PIT_STRING; p.next.String = Str("hello");
    CHECK(p.ReadAsString().eq("hello") && p.errors == 0);
    p.next.Type = PIT_SYMBOL; CHECK(p.ReadAsString().eq("") && p.errors == 1 && p.lastError.eq("Error parsing string"));
    p.ok = false; CHECK(p.ReadAsString().eq("") && p.errors == 2);
}

int main()
{
    Run();
    if (g_fail) { std::printf("009ba6f0_TEST FAIL\n"); return 1; }
    std::printf("009ba6f0_TEST PASS\n");
    return 0;
}