#include <cstdio>
#include <cstring>
// behaviour test: CThing::CreateTCsFromString adds one activated component per `CreateTC "<name>";` statement
struct CCharString {
    char m[32];
    CCharString(const char* text, long) { std::strncpy(m, text, 31); m[31] = 0; }
    CCharString() { m[0] = 0; }
};
static const char* g_text = 0;
struct CStringParser {
    const char* pos;
    CStringParser(const CCharString& text, void (*)(CCharString*), const CCharString&, long) : pos(g_text) {}
    void skipSpace() { while (*pos == ' ') ++pos; }
    bool NextItemIs(const CCharString& item) { skipSpace(); return std::strncmp(pos, item.m, std::strlen(item.m)) == 0 && (pos += std::strlen(item.m), true); }
    bool SkipPastNextItemIfIs(const CCharString& item) { skipSpace(); if (std::strncmp(pos, item.m, std::strlen(item.m))) return false; pos += std::strlen(item.m); return true; }
    CCharString ReadAsString() { CCharString r; int n = 0; while (*pos && *pos != '"') r.m[n++] = *pos++; r.m[n] = 0; return r; }
    bool NextItemIsText(const char* text) { CCharString item(text, -1); return NextItemIs(item); }
    void SkipPastText(const char* text) { CCharString item(text, -1); SkipPastNextItemIfIs(item); }
};
static const char g_emptyString_0122d70e[] = "";
static const char g_createTC[] = "CreateTC";
static const char g_quote[] = "\"";
static const char g_semicolon[] = ";";
struct CTCBase; struct CTCParamsBase;
struct CThing {
    char added[4][32]; int n; bool allActive;
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params)
    { std::strcpy(added[n++], name.m); allActive = allActive && activate && params == 0; return 0; }
    void CreateTCsFromString(const CCharString& text);
};
void CThing::CreateTCsFromString(const CCharString& text)
{
    CStringParser parser(text, 0, CCharString(g_emptyString_0122d70e, -1), 0);
    while (parser.NextItemIsText(g_createTC)) {
        parser.SkipPastText(g_quote);
        CCharString name = parser.ReadAsString();
        parser.SkipPastText(g_quote);
        AddTC(name, true, 0);
        parser.SkipPastText(g_semicolon);
    }
}
int main()
{
    CThing t; t.n = 0; t.allActive = true;
    g_text = "CreateTC \"TCLight\"; CreateTC \"TCDoor\"; Other";
    CCharString text("x", -1);
    t.CreateTCsFromString(text);
    bool ok = t.n == 2 && std::strcmp(t.added[0], "TCLight") == 0 && std::strcmp(t.added[1], "TCDoor") == 0 && t.allActive;
    CThing u; u.n = 0; u.allActive = true; g_text = "";
    u.CreateTCsFromString(text);
    ok = ok && u.n == 0;
    if (!ok) { std::printf("004ca230_TEST FAIL\n"); return 1; }
    std::printf("004ca230_TEST PASS\n");
    return 0;
}
