// Behaviour test for NScript::CV_OracleScript::CVO_OracleMarker::Main (0x00EBCF70): the Oracle's symbol marker. The first time it
// shows the TEXT_QST_B20_ORACLE_TUTORIAL game info and waits until it is clicked past. Then, while the hero is within four units, an
// EXPRESSION_Y / _M / _A / _C picks the category PEOPLE / LEGENDS / HISTORY / CREATURES and the next subject of that category's list
// (TimesPlayed<n>Cat modulo the list length); for PEOPLE, WHISPER is skipped once Whisper was killed by the hero and THERESA once the
// Jack boss battle result is 1 (a skip moves the count past 1, so that first visit gets no intro). The first story of a category starts with TEXT_QST_B20_<category>_INTRO; every story then speaks
// TELL_YOU, TEXT_QST_B20_<category>_<subject>_TITLE and TEXT_QST_B20_<category>_<subject>. A state model.
#include <cstdio>
#include <cstring>
static char g_log[8192];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static const char* YCat[8] = { "WHISPER", "THERESA", "GUILDMASTER", "THUNDER", "BRIAR", "SCYTHE", "MAZE", "ARCHEOLOGIST" };
static const char* MCat[5] = { "DEMON_DOORS", "NECROPOLIS", "SHIP_DROWNED", "SINGING_SWORD", "SKORM_AVO" };
static const char* ACat[7] = { "ARCHON", "HOOK_COAST", "JACK", "NOSTRO", "ORACLE", "PROPHETS", "SNOWSPIRE" };
static const char* CCat[5] = { "BALVERINES", "KRAKENS", "SANDGOOSE", "SCREAMERS", "SUMMONERS" };
struct Marker {
    long TimesPlayedYCat, TimesPlayedMCat, TimesPlayedCCat, TimesPlayedACat; bool HaveBeenToldHowToInteract;
    bool WhisperKilledByHero; long JackBossBattleResult;
    void Say(const char* a, const char* b, const char* c) { char line[96]; std::sprintf(line, "TEXT_QST_B20_%s%s%s", a, b, c); Log(line); }
    void Main(const char* const* expressions, int n)
    {
        if (!HaveBeenToldHowToInteract) { Log("TEXT_QST_B20_ORACLE_TUTORIAL"); HaveBeenToldHowToInteract = true; }
        for (int i = 0; i < n; ++i) {
            const char* e = expressions[i];
            if (std::strcmp(e, "EXPRESSION_Y") && std::strcmp(e, "EXPRESSION_M") && std::strcmp(e, "EXPRESSION_C") && std::strcmp(e, "EXPRESSION_A")) continue;
            const char* category = ""; const char* subject = ""; long times = 0;
            if (!std::strcmp(e, "EXPRESSION_Y")) {
                category = "PEOPLE"; subject = YCat[TimesPlayedYCat % 8];
                if (!std::strcmp(subject, "WHISPER") && WhisperKilledByHero) { subject = YCat[1]; TimesPlayedYCat = 1; }
                if (!std::strcmp(subject, "THERESA") && JackBossBattleResult == 1) { subject = YCat[2]; TimesPlayedYCat = 2; }
                times = ++TimesPlayedYCat;
            } else if (!std::strcmp(e, "EXPRESSION_M")) { category = "LEGENDS"; subject = MCat[TimesPlayedMCat % 5]; times = ++TimesPlayedMCat; }
            else if (!std::strcmp(e, "EXPRESSION_A")) { category = "HISTORY"; subject = ACat[TimesPlayedACat % 7]; times = ++TimesPlayedACat; }
            else if (!std::strcmp(e, "EXPRESSION_C")) { category = "CREATURES"; subject = CCat[TimesPlayedCCat % 5]; times = ++TimesPlayedCCat; }
            if (times == 1) Say(category, "_INTRO", "");
            Log("TEXT_QST_B20_TELL_YOU");
            char cs[48]; std::sprintf(cs, "%s_%s", category, subject);
            Say(cs, "_TITLE", ""); Say(cs, "", "");
        }
    }
};
int main()
{
    Marker a = { 0, 0, 0, 0, false, true, 1 };
    const char* ea[] = { "EXPRESSION_Y", "EXPRESSION_X", "EXPRESSION_Y" };
    a.Main(ea, 3);
    bool ok = !std::strcmp(g_log, "TEXT_QST_B20_ORACLE_TUTORIAL;TEXT_QST_B20_TELL_YOU;"
        "TEXT_QST_B20_PEOPLE_GUILDMASTER_TITLE;TEXT_QST_B20_PEOPLE_GUILDMASTER;TEXT_QST_B20_TELL_YOU;"
        "TEXT_QST_B20_PEOPLE_THUNDER_TITLE;TEXT_QST_B20_PEOPLE_THUNDER;") && a.TimesPlayedYCat == 4 && a.HaveBeenToldHowToInteract;
    g_log[0] = 0;
    Marker b = { 0, 4, 0, 0, true, false, 0 };
    const char* eb[] = { "EXPRESSION_M", "EXPRESSION_C", "EXPRESSION_A" };
    b.Main(eb, 3);
    ok = ok && !std::strcmp(g_log, "TEXT_QST_B20_TELL_YOU;TEXT_QST_B20_LEGENDS_SKORM_AVO_TITLE;TEXT_QST_B20_LEGENDS_SKORM_AVO;"
        "TEXT_QST_B20_CREATURES_INTRO;TEXT_QST_B20_TELL_YOU;TEXT_QST_B20_CREATURES_BALVERINES_TITLE;TEXT_QST_B20_CREATURES_BALVERINES;"
        "TEXT_QST_B20_HISTORY_INTRO;TEXT_QST_B20_TELL_YOU;TEXT_QST_B20_HISTORY_ARCHON_TITLE;TEXT_QST_B20_HISTORY_ARCHON;")
        && b.TimesPlayedMCat == 5 && b.TimesPlayedCCat == 1 && b.TimesPlayedACat == 1;
    std::printf(ok ? "00ebcf70_TEST PASS\n" : "00ebcf70_TEST FAIL\n");
    return ok ? 0 : 1;
}
