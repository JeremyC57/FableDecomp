// Behaviour test for CThingManager::LoadHeroStateScript (0x00520570): skip past "NewThing", take the text up to and
// including "END_THING_LEVEL", parse it (whitespace skipped) and load it as one thing in mode 1.
#include <cstdio>
#include <cstring>

struct CCharString {
    char Text[128];
    CCharString(const char* t, long length = -1) { (void)length; std::strcpy(Text, t); }
};
struct CStringParser {
    const char* Cursor;
    CStringParser(const char* text) : Cursor(text) { }
    CStringParser(const CCharString& text, void (*)(CCharString*), const CCharString&, long) : Cursor(text.Text) { }
    CCharString ReadAsStringUntilString_009ba3e0(const CCharString& term)
    {
        const char* hit = std::strstr(Cursor, term.Text);
        CCharString out("");
        if (hit) { std::strncpy(out.Text, Cursor, hit - Cursor); out.Text[hit - Cursor] = 0; Cursor = hit; }
        return out;
    }
    CCharString ReadAsStringUntilStringInclusive(const CCharString& term)
    {
        const char* hit = std::strstr(Cursor, term.Text);
        CCharString out("");
        if (hit) { long n = (long)(hit - Cursor) + (long)std::strlen(term.Text); std::strncpy(out.Text, Cursor, n); out.Text[n] = 0; Cursor += n; }
        return out;
    }
    long SkipWhitespace() { while (*Cursor == ' ') ++Cursor; return 0; }
};
struct CPersistContext {
    CStringParser* Parser;
    CPersistContext(CStringParser* p, void (*)(CCharString*), bool, void*, void*, CCharString*) : Parser(p) { }
};
static char g_loaded[128];
static long g_mode;
struct CThingManager {
    void LoadSingleThing(CPersistContext& c, long mode) { std::strcpy(g_loaded, c.Parser->Cursor); g_mode = mode; }
    void LoadHeroStateScript(CStringParser& parser)
    {
        parser.ReadAsStringUntilString_009ba3e0("NewThing");
        CCharString heroState = parser.ReadAsStringUntilStringInclusive("END_THING_LEVEL");
        CStringParser heroParser(heroState, 0, CCharString("", -1), 0);
        heroParser.SkipWhitespace();
        CPersistContext context(&heroParser, 0, true, 0, 0, 0);
        LoadSingleThing(context, 1);
    }
};

int main()
{
    CStringParser p("junk NewThing Hero; END_THING_LEVEL tail");
    CThingManager m;
    m.LoadHeroStateScript(p);
    bool ok = std::strcmp(g_loaded, "NewThing Hero; END_THING_LEVEL") == 0 && g_mode == 1 && std::strcmp(p.Cursor, " tail") == 0;
    if (!ok) { std::printf("00520570_TEST FAIL\n"); return 1; }
    std::printf("00520570_TEST PASS\n");
    return 0;
}