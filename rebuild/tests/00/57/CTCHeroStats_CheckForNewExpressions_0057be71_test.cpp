// Behaviour test for CTCHeroStats::CheckForNewExpressions @ 0x0057BE71
#include <cstdio>
#include <cstring>

static const char* g_learned[32]; static int g_count;
static void Learn(const char* name) { g_learned[g_count++] = name; }
static bool Has(const char* name) { for (int i = 0; i < g_count; ++i) if (std::strcmp(g_learned[i], name) == 0) return true; return false; }
static void Check(bool child, long renown, float pct, long morality, bool hasAbilities, long stealth)
{
    g_count = 0;
    if (child) return;
    if (renown >= 1) { Learn("EXPRESSION_SNEER"); if (pct > 0.5f) Learn("EXPRESSION_TAP"); }
    if (renown >= 2) { Learn("EXPRESSION_VICTORY_PUMP"); if (pct > 0.5f) Learn("EXPRESSION_KISS_MY_ASS"); }
    if (renown >= 3) { Learn("EXPRESSION_EVIL_LAUGH"); if (pct > 0.5f) Learn("EXPRESSION_FLAMENCO"); }
    if (renown >= 4) { Learn("EXPRESSION_HEROIC_STANCE"); if (pct > 0.5f) Learn("EXPRESSION_CROTCH_GRAB"); }
    if (renown >= 5) Learn("EXPRESSION_BATTLE_CRY");
    if (morality < 0) Learn("EXPRESSION_SHIT");
    if (morality < 1) Learn("EXPRESSION_PELVIC_THRUST");
    if (morality < 2) Learn("EXPRESSION_MIDDLE_FINGER");
    if (morality < 3) Learn("EXPRESSION_AIR_GUITAR");
    if (morality > 3) Learn("EXPRESSION_APOLOGY");
    if (morality > 4) Learn("EXPRESSION_THANKS");
    if (morality > 5) Learn("EXPRESSION_SATURDAY_NIGHT_FEVER");
    if (morality > 6) Learn("EXPRESSION_BALLET");
    if (hasAbilities) { if (stealth >= 3) Learn("EXPRESSION_STEAL"); if (stealth >= 6) Learn("EXPRESSION_PICKLOCK"); }
}
int main()
{
    Check(true, 5, 1.0f, 0, true, 9);
    if (g_count != 0) { std::printf("0057be71_TEST FAIL\n"); return 1; }
    Check(false, 2, 0.5f, 5, true, 3);
    if (g_count != 5 || !Has("EXPRESSION_VICTORY_PUMP") || Has("EXPRESSION_TAP") || !Has("EXPRESSION_THANKS") ||
        Has("EXPRESSION_SATURDAY_NIGHT_FEVER") || !Has("EXPRESSION_STEAL") || Has("EXPRESSION_PICKLOCK")) { std::printf("0057be71_TEST FAIL\n"); return 1; }
    Check(false, 0, 0.9f, 3, false, 9);
    if (g_count != 0) { std::printf("0057be71_TEST FAIL\n"); return 1; }
    Check(false, 1, 0.9f, -1, true, 6);
    if (g_count != 8 || !Has("EXPRESSION_TAP") || !Has("EXPRESSION_SHIT") || !Has("EXPRESSION_PICKLOCK")) { std::printf("0057be71_TEST FAIL\n"); return 1; }
    std::printf("0057be71_TEST PASS\n");
    return 0;
}