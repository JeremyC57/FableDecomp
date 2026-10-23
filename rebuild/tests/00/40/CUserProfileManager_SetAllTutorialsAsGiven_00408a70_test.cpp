#include <cstdio>
#include <cstring>
static int g_fail = 0;
static void check(bool c, int line) { if (!c) { if (!g_fail) std::printf("check failed at line %d\n", line); g_fail = 1; } }
#define CHECK(c) check((c), __LINE__)
struct CWideString { char s[64]; };
struct CCharString {
    char s[64];
    CCharString() { s[0] = 0; }
    CCharString(const char* text, long = -1) { std::strncpy(s, text, 63); s[63] = 0; }
    CCharString(const CCharString& o) { std::strcpy(s, o.s); }
    bool operator!=(const char* text) const { return std::strcmp(s, text) != 0; }
    CCharString& operator=(const CCharString& o) { std::strcpy(s, o.s); return *this; }
    CCharString& operator+=(const char* text) { std::strncat(s, text, 63 - std::strlen(s)); return *this; }
    CCharString GetSubString(long start, long length) const
    {
        CCharString r; std::strncpy(r.s, s + start, length); r.s[length] = 0; return r;
    }
    CWideString ToWideString() const { CWideString w; std::strcpy(w.s, s); return w; }
    long GetLength() const { return (long)std::strlen(s); }
    const char* GetString() const { return s; }
    bool EndsWith(const CCharString& suffix) const
    {
        long start = GetLength() - suffix.GetLength();
        if (start >= 0)
            return std::strcmp(GetString() + start, suffix.GetString()) == 0;
        return false;
    }
};
CCharString operator+(const CCharString& left, const char* right) { CCharString r(left); r += right; return r; }
struct CTextVector {
    CCharString items[95];
    CCharString& operator[](unsigned int i) { return items[i]; }
};
struct CPlayerGuiDef { CTextVector TutorialText; };
static CPlayerGuiDef g_def;
CPlayerGuiDef* PeekGuiDef() { return &g_def; }
static int g_entries = 0;
static char g_titles[4][64];
static char g_texts[4][64];
struct CHeroLogBook {
    void AddLogEntry(const CWideString& title, const CWideString& heading, const CWideString& text, long category, bool given)
    {
        if (g_entries < 4) { std::strcpy(g_titles[g_entries], title.s); std::strcpy(g_texts[g_entries], text.s); }
        if (std::strcmp(title.s, heading.s) != 0 || category != 2 || !given) g_fail = 1;
        ++g_entries;
    }
};
struct CPlayer { CHeroLogBook* PHeroLogBook; };
struct CPlayerManager { CPlayer player; CPlayer* GetMainPlayer() { return &player; } };
struct CMainGameComponent { CPlayerManager* PlayerManager; };
static CMainGameComponent* g_pMainGameComponent;
enum ETutorialCategory { TUTORIAL_CATEGORY_NONE };
struct CTutorialCategoryVector {
    int n; int last;
    void push_back(const ETutorialCategory& x) { ++n; last = (int)x; }
};
struct CUserProfileManager {
    CTutorialCategoryVector GivenTutorialCategories;
    void SetAllTutorialsAsGiven();
};

void CUserProfileManager::SetAllTutorialsAsGiven()
{
    CPlayerGuiDef* def = PeekGuiDef();
    CHeroLogBook* logBook = g_pMainGameComponent->PlayerManager->GetMainPlayer()->PHeroLogBook;
    for (long i = 0; i < 95; ++i) {
        if (i > 0) {
            CCharString text = def->TutorialText[i];
            if (text != "") {
                CCharString title(text);
                if (title.EndsWith("_PC"))
                    title = title.GetSubString(0, title.GetLength() - 3) + "_TITLE";
                else
                    title += "_TITLE";
                logBook->AddLogEntry(title.ToWideString(), title.ToWideString(), text.ToWideString(), 2, true);
            }
            GivenTutorialCategories.push_back((ETutorialCategory)i);
        }
    }
}

int main()
{
    CHeroLogBook book; CPlayerManager pm; pm.player.PHeroLogBook = &book;
    CMainGameComponent mgc; mgc.PlayerManager = &pm; g_pMainGameComponent = &mgc;
    for (int i = 0; i < 95; ++i) g_def.TutorialText[i] = CCharString("");
    g_def.TutorialText[0] = CCharString("TUT_ZERO");
    g_def.TutorialText[3] = CCharString("TUT_MOVE_PC");
    g_def.TutorialText[94] = CCharString("TUT_LAST");
    CUserProfileManager m; m.GivenTutorialCategories.n = 0; m.GivenTutorialCategories.last = 0;
    m.SetAllTutorialsAsGiven();
    CHECK(g_entries == 2);
    CHECK(std::strcmp(g_titles[0], "TUT_MOVE_TITLE") == 0 && std::strcmp(g_texts[0], "TUT_MOVE_PC") == 0);
    CHECK(std::strcmp(g_titles[1], "TUT_LAST_TITLE") == 0);
    CHECK(m.GivenTutorialCategories.n == 94 && m.GivenTutorialCategories.last == 94);
    if (g_fail) { std::printf("00408a70_TEST FAIL\n"); return 1; }
    std::printf("00408a70_TEST PASS\n");
    return 0;
}