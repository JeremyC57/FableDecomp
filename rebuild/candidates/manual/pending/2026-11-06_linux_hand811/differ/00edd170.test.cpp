// Behaviour test for NScript::CV_TempleOfLightScript::CTOL_AcolyteGood::Main (0x00EDD170): once the hero attacks the temple the
// acolyte runs to the exit point and fades out; a talk gives ADVICE; a donation scales the amount by the hero's morality category
// (6 super, 5 very, 4 plain fiddle), picks the level speech (WEAPON_OUT when invalid, MAXIMUM awards the weapon, HIGHEST..LOWEST
// give a morality fraction, ZERO gives none), then gives the reward weapon once, else cool thing 1 (age) or 2 (title + renown),
// takes the gold unless invalid and clears the visit; otherwise when the prayer timer runs out it prays and re-arms the timer.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Acolyte {
    int Frames, TerminateAt, TalkMask, DonateMask, AttackAt, ExitDistance, MoralityCategory;
    long DonationThisVisit, DonationTotal, Gold, PrayerTimer, Prayers;
    bool DonationInvalid, AwardWeapon, ReceivedReward, GivenCool1, GivenCool2, FadedOut;
    float MoralityCumulative;
    bool T() const { return Frames >= TerminateAt; }
    void NSF() { ++Frames; if (PrayerTimer > 0) --PrayerTimer; }
    bool Bit(int m) const { return Frames < 31 && ((m >> Frames) & 1); }
    void Donate(long amount) {
        DonationThisVisit = amount; DonationTotal += amount;
        float morality_change = 0.0f, donation = (float)amount;
        if (MoralityCategory == 6) donation *= 0.5f;
        else if (MoralityCategory == 5) donation *= 0.75f;
        else if (MoralityCategory == 4) donation *= 0.9f;
        static const float level[6] = { 0.0f, 100.0f, 500.0f, 1000.0f, 5000.0f, 10000.0f };
        if (DonationInvalid) { Log("WEAPON_OUT"); DonationInvalid = false; }
        else if (donation >= level[5]) { Log("MAXIMUM"); AwardWeapon = true; morality_change = 0.5f; }
        else if (donation > level[4]) { Log("HIGHEST"); morality_change = 0.4f; }
        else if (donation > level[3]) { Log("HIGH"); morality_change = 0.3f; }
        else if (donation > level[2]) { Log("MIDDLE"); morality_change = 0.2f; }
        else if (donation > level[1]) { Log("LOW"); morality_change = 0.1f; }
        else if (donation >= 0.0001f) { Log("LOWEST"); morality_change = 0.05f; }
        else if (donation < 0.0001f) { Log("ZERO"); morality_change = 0.0f; }
        MoralityCumulative += morality_change;
        if (AwardWeapon && !ReceivedReward) { Log("reward_weapon"); AwardWeapon = false; ReceivedReward = true; }
        else if ((MoralityCumulative >= 0.6f || DonationTotal >= 20000) && !GivenCool1 && DonationThisVisit != 0) { Log("REMOVE_AGE"); GivenCool1 = true; }
        else if ((MoralityCumulative >= 0.9f || DonationTotal >= 40000) && !GivenCool2 && DonationThisVisit != 0) { Log("NEW_TITLE"); GivenCool2 = true; }
        Gold -= DonationThisVisit;
        DonationThisVisit = 0;
    }
    void Main() {
        if (T()) return;
        for (; !T(); NSF()) {
            if (Frames >= AttackAt) {
                while (ExitDistance >= 2) { --ExitDistance; NSF(); if (T()) return; }
                if (!FadedOut) { FadedOut = true; Log("fade_out"); }
            }
            else if (Bit(TalkMask)) Log("ADVICE");
            else if (Bit(DonateMask)) Donate(Frames == 2 ? 12000 : Frames == 4 ? 700 : 0);
            else if (PrayerTimer == 0) { ++Prayers; PrayerTimer = 5; }
        }
    }
};
int main()
{
    Acolyte a = { 0, 20, (1 << 1), (1 << 2) | (1 << 4) | (1 << 6), 15, 3, 0, 0, 0, 20000, 3, 0,
                  false, false, false, false, false, false, 0.0f };
    a.Main();
    bool ok = !std::strcmp(g_log, "ADVICE;MAXIMUM;reward_weapon;MIDDLE;REMOVE_AGE;ZERO;fade_out;")
              && a.ReceivedReward && a.GivenCool1 && !a.GivenCool2 && a.Gold == 20000 - 12700 && a.FadedOut && a.Prayers == 3 && a.Frames == 20;
    g_log[0] = 0;
    Acolyte b = { 0, 6, 0, (1 << 1), 99, 0, 6, 0, 0, 100, 99, 0, true, false, false, false, false, false, 0.0f };
    b.Main();
    ok = ok && !std::strcmp(g_log, "WEAPON_OUT;") && !b.DonationInvalid && b.Prayers == 0;
    std::printf(ok ? "00edd170_TEST PASS\n" : "00edd170_TEST FAIL\n");
    return ok ? 0 : 1;
}
