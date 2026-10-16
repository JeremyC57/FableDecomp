// Behaviour test for 0x005ccffc CTCInventoryStats::CreateTitle / CreateTitlePC
#include <cstdio>
#include <cstring>
struct CComponent {
    char def[48]; const char* text; CComponent* child;
    void SetText(const char* t, bool, bool) { text = t; }
    void AddChild(CComponent* c) { child = c; }
};
static CComponent g_made;
static CComponent* CreateComponent(const char* def, bool) { std::strcpy(g_made.def, def); return &g_made; }
static void CreateTitle(const char* text, CComponent* parent, bool statTitle)
{
    const char* definition = "PC_STATS_PERSONALITY_STAT_TITLE";
    if (!statTitle) definition = "PC_STATS_PERSONALITY_VIEWPORT_TEXT2";
    CComponent* title = CreateComponent(definition, false);
    title->SetText(text, true, true);
    parent->AddChild(title);
}
int main()
{
    CComponent parent; parent.child = 0;
    CreateTitle("Hi", &parent, true);
    bool ok = parent.child == &g_made && std::strcmp(g_made.def, "PC_STATS_PERSONALITY_STAT_TITLE") == 0 && std::strcmp(g_made.text, "Hi") == 0;
    CreateTitle("Yo", &parent, false);
    ok = ok && std::strcmp(g_made.def, "PC_STATS_PERSONALITY_VIEWPORT_TEXT2") == 0;
    if (!ok) { std::printf("005ccffc_TEST FAIL\n"); return 1; }
    std::printf("005ccffc_TEST PASS\n");
    return 0;
}