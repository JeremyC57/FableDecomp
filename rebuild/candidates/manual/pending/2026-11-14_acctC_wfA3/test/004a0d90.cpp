// Behaviour test for CWorld::LoadQuestsFromFile (0x004A0D90): optionally clear the registered quests, then parse the quest list
// statement by statement: AddQuest("name", TRUE|FALSE) registers the quest (and marks it initially active when the flag reads TRUE,
// case-insensitive), AddTestQuest("quest", "hsp", colour, "display", "ini", "outi", "card") appends a test quest; unknown commands are
// skipped; every statement ends at ';'. A state model of the parse (CStringParser item semantics).
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>

struct Parser {
    const char* p;
    bool NextItemExists() { const char* q = p; while (*q && std::isspace((unsigned char)*q)) ++q; return *q != 0; }
    void SkipWhitespace() { while (*p && std::isspace((unsigned char)*p)) ++p; }
    void ReadUntil(const char* stop, char* out) { const char* e = std::strstr(p, stop); if (!e) e = p + std::strlen(p); std::memcpy(out, p, e - p); out[e - p] = 0; p = e; }
    void SkipPastNextItemIfIs(const char* s) { SkipWhitespace(); if (!std::strncmp(p, s, std::strlen(s))) p += std::strlen(s); }
    long ReadAsInteger() { SkipWhitespace(); char* e; long v = std::strtol(p, &e, 10); p = e; return v; }
    void SkipPastString(const char* s) { const char* e = std::strstr(p, s); p = e ? e + std::strlen(s) : p + std::strlen(p); }
};
static int StrICmp(const char* a, const char* b) { while (*a && std::tolower((unsigned char)*a) == std::tolower((unsigned char)*b)) { ++a; ++b; } return std::tolower((unsigned char)*a) - std::tolower((unsigned char)*b); }

struct TestQuest { char Quest[32], Hsp[32], Display[32], Ini[32], Outi[32], Card[32]; long Colour; };
struct World {
    char Registered[8][32]; int NumRegistered; char Active[8][32]; int NumActive; int ManagerRegistrations;
    TestQuest Tests[4]; int NumTests;
    void ClearRegisteredQuests() { NumRegistered = 0; NumActive = 0; }
    void ReadQuoted(Parser& parser, char* out) { parser.SkipPastNextItemIfIs("\""); parser.ReadUntil("\"", out); parser.SkipPastNextItemIfIs("\""); }
    void LoadQuestsFromFile(const char* file, bool clear)
    {
        if (clear) ClearRegisteredQuests();
        if (std::strlen(file) == 0) return;
        Parser parser = { file };
        while (parser.NextItemExists()) {
            parser.SkipWhitespace();
            char command[32]; parser.ReadUntil("(", command);
            if (!std::strcmp(command, "AddQuest")) {
                parser.SkipPastNextItemIfIs("(");
                char name[32]; ReadQuoted(parser, name);
                parser.SkipPastNextItemIfIs(","); parser.SkipWhitespace();
                char flag[32]; parser.ReadUntil(")", flag); parser.SkipPastNextItemIfIs(")");
                bool active = StrICmp(flag, "TRUE") == 0;
                std::strcpy(Registered[NumRegistered++], name);
                if (active) std::strcpy(Active[NumActive++], name);
                ++ManagerRegistrations;
            } else if (!std::strcmp(command, "AddTestQuest")) {
                TestQuest t;
                parser.SkipPastNextItemIfIs("(");
                ReadQuoted(parser, t.Quest); parser.SkipPastNextItemIfIs(","); parser.SkipWhitespace();
                ReadQuoted(parser, t.Hsp); parser.SkipPastNextItemIfIs(",");
                t.Colour = parser.ReadAsInteger(); parser.SkipPastNextItemIfIs(","); parser.SkipWhitespace();
                ReadQuoted(parser, t.Display); parser.SkipPastNextItemIfIs(","); parser.SkipWhitespace();
                ReadQuoted(parser, t.Ini); parser.SkipPastNextItemIfIs(","); parser.SkipWhitespace();
                ReadQuoted(parser, t.Outi); parser.SkipPastNextItemIfIs(","); parser.SkipWhitespace();
                ReadQuoted(parser, t.Card); parser.SkipPastNextItemIfIs(")");
                Tests[NumTests++] = t;
            }
            parser.SkipPastString(";");
        }
    }
};

int main()
{
    static World w;
    std::strcpy(w.Registered[0], "Old"); w.NumRegistered = 1;
    w.LoadQuestsFromFile("AddQuest(\"Q_A\", TRUE);\n Bogus(1);\nAddTestQuest(\"Q_T\", \"HSP\", 3, \"Disp\", \"a.ini\", \"b.ini\", \"CARD\");\nAddQuest(\"Q_B\", false);", true);
    bool ok = w.NumRegistered == 2 && !std::strcmp(w.Registered[0], "Q_A") && !std::strcmp(w.Registered[1], "Q_B")
           && w.NumActive == 1 && !std::strcmp(w.Active[0], "Q_A") && w.ManagerRegistrations == 2
           && w.NumTests == 1 && w.Tests[0].Colour == 3 && !std::strcmp(w.Tests[0].Hsp, "HSP") && !std::strcmp(w.Tests[0].Card, "CARD");
    w.LoadQuestsFromFile("AddQuest(\"Q_C\", true);", false);
    ok = ok && w.NumRegistered == 3 && w.NumActive == 2 && !std::strcmp(w.Active[1], "Q_C");
    w.LoadQuestsFromFile("", true);
    ok = ok && w.NumRegistered == 0 && w.NumActive == 0;
    std::printf(ok ? "004a0d90_TEST PASS\n" : "004a0d90_TEST FAIL\n");
    return ok ? 0 : 1;
}
