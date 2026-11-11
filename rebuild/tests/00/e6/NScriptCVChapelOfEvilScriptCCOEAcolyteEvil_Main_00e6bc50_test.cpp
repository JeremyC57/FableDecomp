// Behaviour test for NScript::CV_ChapelOfEvilScript::CCOE_AcolyteEvil::Main (0x00E6BC50): once the chapel is attacked the acolyte turns
// hostile and idles; a talk with followers offers one for sacrifice (one follower: ask about him; several: a random victim, the others
// teleported away), tortures the victim and scales the donation by victim type, night and Sunday; without followers he says so;
// otherwise he prays on a timer. A log model of the donation arithmetic, the branch order and the follower teleport loop (every
// follower but the victim is sent to the teleport marker; the loop stops when the script thread is terminating).
#include <cstdio>
#include <cmath>
static float Donation(float health, int roll, int type, int gender, bool guard, bool night, bool sunday)
{
    float d = health * 0.25f * (float)(roll + 1) + 50.0f;   // VictimHealth*(rand()%3+1) scaled, plus the base
    if (type == 1) d *= 1.125f; else if (type == 2) d *= 0.5f; else if (gender == 2) d *= 1.175f; else if (guard) d *= 1.375f;
    if (night) d *= 1.75f;
    if (sunday) d *= 1.55f;
    return d;
}
static const char* Pick(int nfollowers, int answer) { if (nfollowers == 0) return "none"; if (answer != 1) return "declined"; return nfollowers == 1 ? "single" : "multiple"; }
// the multiple-follower branch: `for (i = 0; i < followers.size(); ++i) if (!(victim == followers[i])) { if (T) return; teleport }`
static int TeleportOthers(const int* followers, int count, int victim, int terminate_at, int* sent)
{
    int n = 0;
    for (int i = 0; i < count; ++i) {
        const int& follower = followers[i];
        if (!(victim == follower)) {
            if (n >= terminate_at) return -1;
            sent[n++] = followers[i];
        }
    }
    return n;
}
int main()
{
    bool ok = std::fabs(Donation(100.0f, 0, 1, 1, false, false, false) - 84.375f) < 0.01f
        && std::fabs(Donation(100.0f, 2, 0, 2, false, true, false) - (125.0f * 1.175f * 1.75f)) < 0.01f
        && std::fabs(Donation(40.0f, 1, 2, 1, false, false, true) - (70.0f * 0.5f * 1.55f)) < 0.01f;
    ok = ok && Pick(0, 1)[0] == 'n' && Pick(1, 1)[0] == 's' && Pick(3, 1)[0] == 'm' && Pick(2, 0)[0] == 'd';
    static const int followers[4] = { 11, 22, 33, 44 };
    int sent[4] = { 0, 0, 0, 0 };
    ok = ok && TeleportOthers(followers, 4, 33, 99, sent) == 3 && sent[0] == 11 && sent[1] == 22 && sent[2] == 44;
    ok = ok && TeleportOthers(followers, 2, 11, 99, sent) == 1 && sent[0] == 22;
    ok = ok && TeleportOthers(followers, 4, 44, 2, sent) == -1 && sent[0] == 11 && sent[1] == 22;   // thread exit inside the loop
    std::printf(ok ? "00e6bc50_TEST PASS\n" : "00e6bc50_TEST FAIL\n");
    return ok ? 0 : 1;
}