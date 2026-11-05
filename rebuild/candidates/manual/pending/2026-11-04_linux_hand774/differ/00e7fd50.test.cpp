// Behaviour test for 0x00e7fd50: NScript::CV_FishingCompetitionScript::GenerateSignText -- the fish-sign notice
// Host model: stub interface records the sign text; checks the hero-title substitution, the two-decimal rounding and both formats.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
static std::string Fmt(double v) { char b[32]; std::sprintf(b, "%.2f", v); return b; }
static float RoundNear(float v, long places) { float s = 1.0f; for (long i = places; i != 0; --i) s *= 10.0f; return (float)(long)std::floor(v * s + 0.5f) / s; }
struct CGame {
    std::string SignText; std::string SignName;
    std::string GetTextString(const std::string& tag) const { return "<" + tag + ">"; }
    std::string GetFormattedString(const std::string& tag, const std::vector<std::string>& p) const {
        std::string r = tag; for (size_t i = 0; i < p.size(); ++i) r += "|" + p[i]; return r; }
    void SetReadableObjectText(const std::string& thing, const std::string& text) { SignName = thing; SignText = text; }
};
struct CV_FishingCompetitionScript {
    CGame* Game; bool CompetitionWon; long PrizeNumber; std::string Winner; float BiggestCompetitorFish; std::vector<float> Weights;
    std::string GetHeroTitleTextTag() { return "TEXT_GUI_HERO_TITLE_AVATAR"; }
    float GetNextPrizeCategoryWeight() { long next = PrizeNumber + 1; if (PrizeNumber == -1) ++next; return Weights[next]; }
    void GenerateSignText() {
        std::string sign = "FishCompSign";
        std::string winnerName = Game->GetTextString(Winner);
        if (Winner == "HERO") winnerName = Game->GetTextString(GetHeroTitleTextTag());
        float nextWeight = GetNextPrizeCategoryWeight();
        std::vector<std::string> params;
        params.push_back(Fmt(RoundNear(BiggestCompetitorFish, 2)));
        params.push_back(winnerName);
        std::string text;
        if (CompetitionWon) text = Game->GetFormattedString("TEXT_QST_B15_FISH_SIGN_INFO_HEROWON", params);
        else { params.push_back(Fmt(nextWeight)); text = Game->GetFormattedString("TEXT_QST_B15_FISH_SIGN_INFO_NORMAL", params); }
        Game->SetReadableObjectText(sign, text);
    }
};
int main()
{
    CGame g; CV_FishingCompetitionScript s; s.Game = &g; s.CompetitionWon = false; s.PrizeNumber = -1; s.Winner = "TEXT_QST_B15_FISHER_NAME_1";
    s.BiggestCompetitorFish = 3.14159f; s.Weights.push_back(1.0f); s.Weights.push_back(2.5f); s.Weights.push_back(4.0f); s.Weights.push_back(6.0f);
    s.GenerateSignText();
    CHECK(g.SignName == "FishCompSign");
    CHECK(g.SignText == "TEXT_QST_B15_FISH_SIGN_INFO_NORMAL|3.14|<TEXT_QST_B15_FISHER_NAME_1>|2.50");
    s.PrizeNumber = 1; s.GenerateSignText();
    CHECK(g.SignText == "TEXT_QST_B15_FISH_SIGN_INFO_NORMAL|3.14|<TEXT_QST_B15_FISHER_NAME_1>|4.00");
    s.Winner = "HERO"; s.CompetitionWon = true; s.GenerateSignText();
    CHECK(g.SignText == "TEXT_QST_B15_FISH_SIGN_INFO_HEROWON|3.14|<TEXT_GUI_HERO_TITLE_AVATAR>");
    if (g_fail) { std::printf("00e7fd50_TEST FAIL\n"); return 1; }
    std::printf("00e7fd50_TEST PASS\n");
    return 0;
}
