#include <cstdio>
#include <cstring>
// Behaviour test for CTCTavernGameCoinGolf::GetBestScore (0x007D2A00): the body of src/ over stand-ins -- a fixed-buffer
// wide string, the def's best-score text id, the table's def name and the hero's two coin-golf best scores.
class CWideString { public:
    char text[64];
    CWideString() { text[0] = 0; }
    CWideString(const char* t) { strcpy(text, t); }
    const CWideString& operator+=(const CWideString& other) { strcat(text, other.text); return *this; }
};
class CCharString { public:
    char text[32];
    CWideString ToWideString() const { return CWideString(text); }
    static CCharString FormatDouble(long digits, double value)
    {
        CCharString s;
        sprintf(s.text, "%.*f", (int)digits, value);
        return s;
    }
};
class CDefString { public:
    const char* name;
    bool operator==(const char* t) const { return strcmp(name, t) == 0; }
};
class CThing { public:
    const char* defName;
    CDefString GetDefName() const { CDefString d; d.name = defName; return d; }
};
static int g_releases = 0;
class CTavernGameDef { public:
    long m_count;
    unsigned long BestScore;
    void ReleaseTemporary() { --m_count; ++g_releases; }
};
static CTavernGameDef g_def = { 1, 77 };
struct CDefPointer {
    CTavernGameDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CTavernGameDef* operator->() const { return Object; }
};
class CTCHeroStats { public:
    float BestScoreCoinGolfOakVale;
    float BestScoreCoinGolfSnowSpire;
};
static CTCHeroStats g_stats;
static unsigned long g_textId = 0;
class CTCTavernGame { public:
    CThing* ParentThing;
    void GetTavernGameDef(CDefPointer* out) { out->Object = &g_def; ++g_def.m_count; }
    void GetHeroStats(CTCHeroStats** stats) { *stats = &g_stats; }
    CWideString GetText(unsigned long id) { g_textId = id; return CWideString("Best: "); }
};
class CTCTavernGameCoinGolf : public CTCTavernGame { public:
    CWideString GetBestScore();
};

CWideString CTCTavernGameCoinGolf::GetBestScore()
{
    CDefPointer def;
    GetTavernGameDef(&def);
    CWideString text = GetText(def->BestScore);
    CTCHeroStats* stats = 0;
    GetHeroStats(&stats);
    if (ParentThing->GetDefName() == "OBJECT_TAVERN_TABLE_COIN_GOLF" && stats->BestScoreCoinGolfOakVale != 0.0f)
        text += CCharString::FormatDouble(0, stats->BestScoreCoinGolfOakVale).ToWideString();
    else {
        CThing* parent = ParentThing;
        if (parent->GetDefName() == "OBJECT_TAVERN_TABLE_COIN_GOLF_SNOWSPIRE" && stats->BestScoreCoinGolfSnowSpire != 0.0f)
            text += CCharString::FormatDouble(0, stats->BestScoreCoinGolfSnowSpire).ToWideString();
    }
    return text;
}

static int fail(int line) { printf("007d2a00_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    CThing table;
    CTCTavernGameCoinGolf game;
    game.ParentThing = &table;
    g_stats.BestScoreCoinGolfOakVale = 12.0f;
    g_stats.BestScoreCoinGolfSnowSpire = 7.0f;

    table.defName = "OBJECT_TAVERN_TABLE_COIN_GOLF";
    CHECK(strcmp(game.GetBestScore().text, "Best: 12") == 0);
    CHECK(g_textId == 77 && g_releases == 1 && g_def.m_count == 1);

    table.defName = "OBJECT_TAVERN_TABLE_COIN_GOLF_SNOWSPIRE";
    CHECK(strcmp(game.GetBestScore().text, "Best: 7") == 0);

    g_stats.BestScoreCoinGolfSnowSpire = 0.0f;            // no score yet: label only
    CHECK(strcmp(game.GetBestScore().text, "Best: ") == 0);

    table.defName = "OBJECT_TAVERN_TABLE_COIN_GOLF";      // Oak Vale table without a score does not fall through to Snowspire
    g_stats.BestScoreCoinGolfOakVale = 0.0f; g_stats.BestScoreCoinGolfSnowSpire = 9.0f;
    CHECK(strcmp(game.GetBestScore().text, "Best: ") == 0);

    table.defName = "OBJECT_TAVERN_TABLE_SHOVE";
    CHECK(strcmp(game.GetBestScore().text, "Best: ") == 0);
    CHECK(g_releases == 5);
    printf("007d2a00_TEST PASS\n");
    return 0;
}