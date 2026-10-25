// Behaviour test for 0x004138D0 GFParseCommandLine: walks the "-option" words of the command line (case-insensitive):
// -staticmap "World" sets the world to load (and, when non-empty, the compile-defs flag and, after parsing, the start-game
// flags), -q N stores N, -build_retail_static_maps sets its four flags, -enable_map_optimizations its one; a missing
// opening quote after -staticmap stops parsing; a null line does nothing.
#include <cstdio>
#include <cstring>
#include <cctype>
#include <cstdlib>

static int icmp(const char* a, const char* b)
{
    for (;; ++a, ++b) {
        int x = std::tolower((unsigned char)*a), y = std::tolower((unsigned char)*b);
        if (x != y) return x - y;
        if (!x) return 0;
    }
}
struct CCharString {
    char s[128];
    CCharString(const char* t = "") { std::strcpy(s, t); }
    const char* GetString() const { return s; }
    bool EqualsNoCase(const CCharString& o) const { return icmp(s, o.s) == 0; }
};
struct CParsedItem { long Type; long Integer; CCharString String; };
struct CStringParser {                             // simplified tokenizer with the retail call surface
    const char* text; unsigned long pos;
    CStringParser(char* t, void*, const CCharString&, long) : text(t), pos(0) { }
    bool ReadAsStringUntilString(const CCharString& term, CParsedItem* item)
    {
        const char* f = std::strstr(text + pos, term.s);
        if (!f) return false;
        std::strncpy(item->String.s, text + pos, f - (text + pos)); item->String.s[f - (text + pos)] = 0;
        pos = (unsigned long)(f - text);
        return true;
    }
    CCharString ReadAsStringUntilString(const CCharString& term)
    {
        CParsedItem it; if (!ReadAsStringUntilString(term, &it)) { pos = (unsigned long)std::strlen(text); }
        return it.String;
    }
    CCharString ReadAsSymbol() { char c[2] = { text[pos], 0 }; if (text[pos]) ++pos; return CCharString(c); }
    CCharString ReadAsString()
    {
        while (text[pos] == ' ') ++pos;
        CCharString r; unsigned long n = 0;
        while (text[pos] && text[pos] != ' ' && text[pos] != '"') r.s[n++] = text[pos++];
        r.s[n] = 0; return r;
    }
    long ReadAsInteger() { while (text[pos] == ' ') ++pos; char* e; long v = std::strtol(text + pos, &e, 10); pos = (unsigned long)(e - text); return v; }
};

static char g_WorldToLoad[64];
static bool g_bCompileFrontendDefinitions, g_bBuildRetailStaticMaps, g_bEnableMapOptimisations, g_bRuntimeLevelFiles;
static bool g_bStartMainGame, g_bFrontEndFlag_137548F, g_bFrontEndFlag_137544A;
static long g_CommandLineQ;

// the function under test (shape of 0x004138D0)
static void GFParseCommandLine(char* commandLine)
{
    bool worldSpecified = false;
    if (commandLine != 0) {
        CStringParser parser(commandLine, 0, CCharString(""), 0);
        CParsedItem item;
        while (parser.ReadAsStringUntilString("-", &item)) {
            parser.ReadAsSymbol();
            CCharString option = parser.ReadAsString();
            if (option.EqualsNoCase("staticmap")) {
                if (!parser.ReadAsStringUntilString("\"", &item))
                    break;
                parser.ReadAsSymbol();
                std::strcpy(g_WorldToLoad, parser.ReadAsStringUntilString("\"").s);
                if (g_WorldToLoad[0] != 0) {
                    worldSpecified = true;
                    g_bCompileFrontendDefinitions = true;
                }
            }
            else if (option.EqualsNoCase("q"))
                g_CommandLineQ = parser.ReadAsInteger();
            else if (option.EqualsNoCase("build_retail_static_maps")) {
                g_bBuildRetailStaticMaps = true; g_bRuntimeLevelFiles = true;
                g_bCompileFrontendDefinitions = true; g_bEnableMapOptimisations = true;
            }
            else if (option.EqualsNoCase("enable_map_optimizations"))
                g_bEnableMapOptimisations = true;
        }
    }
    if (worldSpecified) {
        g_bFrontEndFlag_137548F = false;
        g_bStartMainGame = true;
        g_bFrontEndFlag_137544A = false;
    }
}

static void reset()
{
    g_WorldToLoad[0] = 0; g_bCompileFrontendDefinitions = g_bBuildRetailStaticMaps = g_bEnableMapOptimisations = false;
    g_bRuntimeLevelFiles = g_bStartMainGame = false; g_bFrontEndFlag_137548F = g_bFrontEndFlag_137544A = true; g_CommandLineQ = -7;
}

int main()
{
    int ok = 1;
    reset(); GFParseCommandLine(0);
    if (g_bStartMainGame || g_CommandLineQ != -7) ok = 0;
    char a[] = "fable.exe -StaticMap \"Oakvale\" -q 3";
    reset(); GFParseCommandLine(a);
    if (std::strcmp(g_WorldToLoad, "Oakvale") != 0 || !g_bCompileFrontendDefinitions || !g_bStartMainGame) ok = 0;
    if (g_bFrontEndFlag_137548F || g_bFrontEndFlag_137544A || g_CommandLineQ != 3 || g_bEnableMapOptimisations) ok = 0;
    char b[] = "-staticmap \"\" -ENABLE_MAP_OPTIMIZATIONS";
    reset(); GFParseCommandLine(b);
    if (g_bStartMainGame || g_bCompileFrontendDefinitions || !g_bEnableMapOptimisations || !g_bFrontEndFlag_137548F) ok = 0;
    char c[] = "-build_retail_static_maps -unknown 5";
    reset(); GFParseCommandLine(c);
    if (!g_bBuildRetailStaticMaps || !g_bRuntimeLevelFiles || !g_bCompileFrontendDefinitions || !g_bEnableMapOptimisations || g_bStartMainGame) ok = 0;
    char d[] = "-staticmap noquote -q 9";       // no opening quote: parsing stops before -q
    reset(); GFParseCommandLine(d);
    if (g_CommandLineQ != -7 || g_bStartMainGame) ok = 0;
    std::printf(ok ? "004138d0_TEST PASS\n" : "004138d0_TEST FAIL\n");
    return ok ? 0 : 1;
}