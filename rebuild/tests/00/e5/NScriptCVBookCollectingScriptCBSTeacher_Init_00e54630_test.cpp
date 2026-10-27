#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (calls are recorded, not executed).
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static const char* g_str = "";
class CScriptDef { public: int BooksNumberWanted, BooksNumberAccepted, BooksNumberComment, BooksNumberTotal, BadBooksForHat, GoodBooksForHat; float BooksMoralityReward; };
static CScriptDef g_def = { 5, 4, 3, 20, 2, 6, 7.75f };
static CScriptDef* pGlobalScriptDef = &g_def;
struct E { int BadBooksForHat, GoodBooksForHat, BooksWanted, BooksAccepted, BooksComment, MoralityReward;
    void Init()
    {
        BadBooksForHat = pGlobalScriptDef->BadBooksForHat;
        GoodBooksForHat = pGlobalScriptDef->GoodBooksForHat;
        MoralityReward = (int)pGlobalScriptDef->BooksMoralityReward;
        BooksWanted = pGlobalScriptDef->BooksNumberWanted;
        BooksAccepted = pGlobalScriptDef->BooksNumberAccepted;
        BooksComment = pGlobalScriptDef->BooksNumberComment;
    }
};
int main()
{
    g_log[0] = 0;
    bool ok = true;
    E e; e.Init();
    ok = e.BadBooksForHat == 2 && e.GoodBooksForHat == 6 && e.MoralityReward == 7 && e.BooksWanted == 5 && e.BooksAccepted == 4 && e.BooksComment == 3;
    if (!ok) { std::printf("00e54630_TEST FAIL\n"); return 1; }
    std::printf("00e54630_TEST PASS\n");
    return 0;
}