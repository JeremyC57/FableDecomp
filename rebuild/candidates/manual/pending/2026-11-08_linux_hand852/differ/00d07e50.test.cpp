// Behaviour test for CQ_BanditCampBossBattleScript::CCampHostage::Main (0x00D07E50): the hostage shouts at the hero when the
// shout timer has run out and the hero is within 8 units; the lines go FIRST, SECOND, THIRD, FOURTH, then alternate THIRD /
// FOURTH, and each shout rearms the timer to 10.
#include <cstdio>
#include <cstring>

struct Hostage {
    long Shout; long Timer; const char* Last;
    void Frame(float distance)
    {
        if (Timer <= 0 && distance < 8.0f) {
            switch (Shout) {
            case 0: Last = "FIRST"; Shout = 1; break;
            case 1: Last = "SECOND"; Shout = 2; break;
            case 2: Last = "THIRD"; Shout = 3; break;
            case 3: Last = "FOURTH"; Shout = 2; break;
            }
            Timer = 10;
        } else if (Timer > 0) --Timer;
    }
};

int main()
{
    Hostage h = { 0, 0, "" };
    const char* expect[] = { "FIRST", "SECOND", "THIRD", "FOURTH", "THIRD", "FOURTH" };
    bool ok = true;
    h.Frame(20.0f);
    ok = ok && std::strcmp(h.Last, "") == 0;
    for (int i = 0; i < 6; ++i) {
        h.Timer = 0;
        h.Frame(5.0f);
        ok = ok && std::strcmp(h.Last, expect[i]) == 0 && h.Timer == 10;
    }
    if (!ok) { std::printf("00d07e50_TEST FAIL\n"); return 1; }
    std::printf("00d07e50_TEST PASS\n");
    return 0;
}
