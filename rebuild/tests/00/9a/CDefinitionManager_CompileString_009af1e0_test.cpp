// Behaviour test for 0x009AF1E0 CDefinitionManager::CompileString: every "#definition_template <class> <name>" block is
// compiled before any "#definition <class> <name>" block (the parser is rewound between the passes), "#definition" does
// not match inside "#definition_template" (whole-string skip), and a non-empty class filter keeps only blocks whose
// class matches it case-insensitively.
#include <cstdio>
#include <cstring>
#include <cctype>
#include <string>
#include <vector>

static int CompareNoCase(const char* a, const char* b)
{
    for (;; ++a, ++b) {
        int ca = std::tolower((unsigned char)*a), cb = std::tolower((unsigned char)*b);
        if (ca != cb) return ca - cb;
        if (ca == 0) return 0;
    }
}
static bool IsWordChar(char c) { return std::isalnum((unsigned char)c) || c == '_' || c == '#'; }

struct Parser {
    std::string text; size_t pos;
    Parser(const char* t) : text(t), pos(0) {}
    bool SkipPastWholeString(const std::string& word)
    {
        size_t at = pos;
        for (;;) {
            at = text.find(word, at);
            if (at == std::string::npos) return false;
            bool startOk = at == 0 || !IsWordChar(text[at - 1]);
            bool endOk = at + word.size() >= text.size() || !IsWordChar(text[at + word.size()]);
            if (startOk && endOk) { pos = at + word.size(); return true; }
            ++at;
        }
    }
    std::string ReadAsString()
    {
        while (pos < text.size() && std::isspace((unsigned char)text[pos])) ++pos;
        size_t start = pos;
        while (pos < text.size() && !std::isspace((unsigned char)text[pos])) ++pos;
        return text.substr(start, pos - start);
    }
    void SetStringPos(size_t p) { pos = p; }
};

static std::vector<std::string> g_compiled;
static void CompileDefinition(Parser&, const std::string& type, const std::string& name) { g_compiled.push_back(type + ":" + name); }

static void CompileString(const char* text, const std::string& defType)
{
    Parser parser(text);
    while (parser.SkipPastWholeString("#definition_template")) {
        std::string type = parser.ReadAsString();
        std::string name = parser.ReadAsString();
        if (defType == "" || CompareNoCase(defType.c_str(), type.c_str()) == 0)
            CompileDefinition(parser, type, name);
    }
    parser.SetStringPos(0);
    while (parser.SkipPastWholeString("#definition")) {
        std::string type = parser.ReadAsString();
        std::string name = parser.ReadAsString();
        if (defType == "" || CompareNoCase(defType.c_str(), type.c_str()) == 0)
            CompileDefinition(parser, type, name);
    }
}

static int g_fail = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("FAIL: %s\n", what); g_fail = 1; } }

int main()
{
    const char* defs =
        "#definition CWeaponDef OBJECT_SWORD\nend\n"
        "#definition_template CWeaponDef TEMPLATE_BLADE\nend\n"
        "#definition CArmourDef OBJECT_PLATE\nend\n"
        "#definition_template CArmourDef TEMPLATE_ARMOUR\nend\n";
    g_compiled.clear();
    CompileString(defs, "");
    Check(g_compiled.size() == 4, "all four blocks");
    Check(g_compiled.size() == 4 && g_compiled[0] == "CWeaponDef:TEMPLATE_BLADE" && g_compiled[1] == "CArmourDef:TEMPLATE_ARMOUR",
          "templates first, in text order");
    Check(g_compiled.size() == 4 && g_compiled[2] == "CWeaponDef:OBJECT_SWORD" && g_compiled[3] == "CArmourDef:OBJECT_PLATE",
          "then definitions, in text order");

    g_compiled.clear();
    CompileString(defs, "cweapondef");
    Check(g_compiled.size() == 2 && g_compiled[0] == "CWeaponDef:TEMPLATE_BLADE" && g_compiled[1] == "CWeaponDef:OBJECT_SWORD",
          "case-insensitive class filter");

    g_compiled.clear();
    CompileString("no blocks here", "");
    Check(g_compiled.empty(), "empty text");

    if (!g_fail) std::printf("009af1e0_TEST PASS\n");
    else std::printf("009af1e0_TEST FAIL\n");
    return g_fail;
}