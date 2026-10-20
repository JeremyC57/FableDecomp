#include <cstdio>
#include <cstring>
struct Str { char buf[64]; Str(const char* t) { std::strcpy(buf, t); } void operator+=(const char* t) { std::strcat(buf, t); } void operator=(const char* t) { std::strcpy(buf, t); } };
static int g_rand = 0, g_time = 0; static bool g_done = false; static char g_said[64];
static int rnd() { return g_rand; }
struct CBriarRose {
    bool IsDone() { return g_done; }
    void DoSpeech(const Str& s) { std::strcpy(g_said, s.buf); }
    void DoOnHitSpeech()
    {
        Str line("TEXT_QST_B08_BRIAR_ONHIT");
        if (rnd() % 4 == 0) {
            if (IsDone()) return;
            if (g_time > 48) { if (IsDone()) return; line += "_START"; }
            else if (g_time > 24) { if (IsDone()) return; line += "_MIDDLE"; }
            else { if (IsDone()) return; line += "_END"; }
        } else {
            if (IsDone()) return;
            line = "TEXT_QST_B08_BRIAR_ONHIT_DEFAULT";
        }
        DoSpeech(line);
    }
};
int main()
{
    CBriarRose b;
    g_rand = 8; g_time = 30; b.DoOnHitSpeech();
    if (std::strcmp(g_said, "TEXT_QST_B08_BRIAR_ONHIT_MIDDLE")) { std::printf("00da26b0_TEST FAIL\n"); return 1; }
    g_rand = 5; b.DoOnHitSpeech();
    if (std::strcmp(g_said, "TEXT_QST_B08_BRIAR_ONHIT_DEFAULT")) { std::printf("00da26b0_TEST FAIL\n"); return 1; }
    g_rand = 4; g_time = 60; b.DoOnHitSpeech();
    if (std::strcmp(g_said, "TEXT_QST_B08_BRIAR_ONHIT_START")) { std::printf("00da26b0_TEST FAIL\n"); return 1; }
    g_done = true; g_said[0] = 0; b.DoOnHitSpeech();
    if (g_said[0]) { std::printf("00da26b0_TEST FAIL\n"); return 1; }
    std::printf("00da26b0_TEST PASS\n");
    return 0;
}
