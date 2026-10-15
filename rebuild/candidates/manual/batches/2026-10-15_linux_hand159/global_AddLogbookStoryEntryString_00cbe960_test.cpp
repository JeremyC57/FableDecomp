// Behaviour test for 0x00CBE960: the reconstructed logic over a host model.
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
static void AddLogbookStoryEntryString(const C& tag)
{
    C name = tag + "_NAME";
    C desc = tag + "_DESC";
    g_pGameScriptInterface->AddLogBookEntry(name.ToWideString(), name.ToWideString(), desc.ToWideString(), 1);
}
static void Run()
{
    g_gsi.entries = 0; g_gsi.frames = 0;
    AddLogbookStoryEntryString(C("TEXT_STORY_1"));
    CHECK(g_gsi.title.eq("TEXT_STORY_1_NAME") && g_gsi.heading.eq("TEXT_STORY_1_NAME") && g_gsi.text.eq("TEXT_STORY_1_DESC"));
    CHECK(g_gsi.category == 1 && g_gsi.entries == 1 && g_gsi.frames == 0);
}

int main()
{
    Run();
    if (g_fail) { std::printf("00cbe960_TEST FAIL\n"); return 1; }
    std::printf("00cbe960_TEST PASS\n");
    return 0;
}
