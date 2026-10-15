// Behaviour test for 0x00CBE9EE: the reconstructed logic over a host model.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
// Host model: string concatenation, wide conversion and the script interface's log book / frame calls.
struct W { char s[40]; W() { s[0] = 0; } W(const char* t) { std::strcpy(s, t); } bool eq(const char* t) const { return std::strcmp(s, t) == 0; } };
struct C { char s[40]; C(const char* t) { std::strcpy(s, t); } W ToWideString() const { return W(s); } };
static C operator+(const C& l, const char* r) { C c(l.s); std::strcat(c.s, r); return c; }
struct GSI {
    W title, heading, text; int category, entries, frames;
    void AddLogBookEntry(const W& t, const W& h, const W& x, int cat) { title = t; heading = h; text = x; category = cat; ++entries; }
    void NewScriptFrame() { ++frames; }
};
static GSI g_gsi;
static GSI* g_pGameScriptInterface = &g_gsi;
static void AddLogbookTutorialEntry(const C& tag)
{
    C title = tag + "_TITLE";
    C text = tag;
    g_pGameScriptInterface->AddLogBookEntry(title.ToWideString(), title.ToWideString(), text.ToWideString(), 2);
    g_pGameScriptInterface->NewScriptFrame();
}
static void Run()
{
    g_gsi.entries = 0; g_gsi.frames = 0;
    AddLogbookTutorialEntry(C("TUT_A"));
    CHECK(g_gsi.title.eq("TUT_A_TITLE") && g_gsi.heading.eq("TUT_A_TITLE") && g_gsi.text.eq("TUT_A"));
    CHECK(g_gsi.category == 2 && g_gsi.entries == 1 && g_gsi.frames == 1);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00cbe9ee_TEST FAIL\n"); return 1; }
    std::printf("00cbe9ee_TEST PASS\n");
    return 0;
}