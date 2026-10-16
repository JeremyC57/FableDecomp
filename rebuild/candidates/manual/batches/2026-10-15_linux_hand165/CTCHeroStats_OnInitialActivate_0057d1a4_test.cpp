#include <cstdio>
// Behaviour model of CTCHeroStats::OnInitialActivate (0x0057D1A4): re-applies morality, age, fatness, sun tan and money
// (money with the flag set), looks up the drunkenness def by name, then resets the training status, in that order.
static char g_log[16]; static int g_n;
struct Stats {
    long Morality; float Age, Fatness, SunTan; long Money; long drunkDef;
    void SetMorality(long) { g_log[g_n++] = 'm'; }
    void SetAge(float) { g_log[g_n++] = 'a'; }
    void SetFatness(float) { g_log[g_n++] = 'f'; }
    void SetSunTan(float) { g_log[g_n++] = 's'; }
    void SetMoney(long, bool flag) { g_log[g_n++] = flag ? '$' : '?'; }
    void ResetTrainingStatus() { g_log[g_n++] = 'r'; }
    void OnInitialActivate()
    {
        SetMorality(Morality); SetAge(Age); SetFatness(Fatness); SetSunTan(SunTan); SetMoney(Money, true);
        drunkDef = 42; g_log[g_n++] = 'd';
        ResetTrainingStatus();
    }
};
int main()
{
    Stats s = { 1, 2.0f, 0.5f, 0.25f, 100, 0 };
    s.OnInitialActivate();
    g_log[g_n] = 0;
    const char* want = "mafs$dr";
    for (int i = 0; want[i] || g_log[i]; ++i)
        if (want[i] != g_log[i]) { std::printf("0057d1a4_TEST FAIL\n"); return 1; }
    if (s.drunkDef != 42) { std::printf("0057d1a4_TEST FAIL\n"); return 1; }
    std::printf("0057d1a4_TEST PASS\n");
    return 0;
}
