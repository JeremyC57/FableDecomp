// Behaviour test for 0x00e5e5a0 NScript::CV_BodyGuardScript::MakeHenchmanPerformIdleComment: false unless every henchman is in
// state group 2; the BORED comment after 15 s standing still once the bored timer is 0 (timer -> 30); when the idle timer is 0
// (timer -> 60) a QUESTION for two or more henchmen three times in four, else the first good (morality > 3) or evil (< 3)
// henchman, scanned round from a random start, says HERO_IDLE; true otherwise.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

struct CHench { int Group; std::string Data; };
static std::vector<CHench> HenchmanList;
static float HeroStoodStillCounter; static long HenchmanLastBoredChatTimer, HenchmanLastIdleChatTimer;
static long g_morality; static int g_rand[8]; static int g_ri;
static std::string g_lastComment; static int g_lastMode; static std::string g_lastSpeaker; static int g_comments;
enum { HM_COMMENT_SINGLE, HM_COMMENT_MULTIPLE, HM_COMMENT_MULTIPLE_RANDOM };
static int my_rand() { return g_rand[g_ri++ & 7]; }
static void MakeHenchmanComment(const char* key, int, int mode, const CHench* who = 0)
{ ++g_comments; g_lastComment = key; g_lastMode = mode; g_lastSpeaker = who ? who->Data : ""; }

static bool MakeHenchmanPerformIdleComment()
{
    for (unsigned long i = 0; i < HenchmanList.size(); i++)
        if (HenchmanList[i].Group != 2) return false;
    if (HeroStoodStillCounter > 15.0f && HenchmanLastBoredChatTimer == 0) {
        HenchmanLastBoredChatTimer = 30;
        MakeHenchmanComment("BORED", 1, HM_COMMENT_MULTIPLE_RANDOM);
        return true;
    }
    if (HenchmanLastIdleChatTimer == 0) {
        HenchmanLastIdleChatTimer = 60;
        if (HenchmanList.size() > 1 && my_rand() % 4) {
            MakeHenchmanComment("QUESTION", 1, HM_COMMENT_MULTIPLE);
            return true;
        }
        if (HenchmanList.size()) {
            unsigned long start = my_rand() % HenchmanList.size();
            for (unsigned long j = 0; j < HenchmanList.size(); j++) {
                CHench henchman = HenchmanList[(start + j) % HenchmanList.size()];
                if (g_morality > 3 && (henchman.Data == "_GOOD1" || henchman.Data == "_GOOD2")) {
                    MakeHenchmanComment("HERO_IDLE", 1, HM_COMMENT_SINGLE, &henchman); return true; }
                if (g_morality < 3 && (henchman.Data == "_EVIL1" || henchman.Data == "_EVIL2")) {
                    MakeHenchmanComment("HERO_IDLE", 1, HM_COMMENT_SINGLE, &henchman); return true; }
            }
        }
    }
    return true;
}

int main()
{
    bool ok = true;
    CHench a = { 2, "_GOOD1" }, b = { 2, "_EVIL2" }, c = { 1, "_GOOD2" };
    HenchmanList.push_back(a); HenchmanList.push_back(c);
    ok = ok && !MakeHenchmanPerformIdleComment() && g_comments == 0;              // c not in group 2
    HenchmanList[1] = b;
    HeroStoodStillCounter = 16.0f;
    ok = ok && MakeHenchmanPerformIdleComment() && g_lastComment == "BORED" && g_lastMode == HM_COMMENT_MULTIPLE_RANDOM && HenchmanLastBoredChatTimer == 30;
    g_rand[0] = 1;                                                                 // 1 % 4 != 0: QUESTION
    ok = ok && MakeHenchmanPerformIdleComment() && g_lastComment == "QUESTION" && HenchmanLastIdleChatTimer == 60;
    HenchmanLastIdleChatTimer = 0; g_rand[1] = 4; g_rand[2] = 1; g_morality = 1;  // no question; start at b (index 1), evil hero
    ok = ok && MakeHenchmanPerformIdleComment() && g_lastComment == "HERO_IDLE" && g_lastSpeaker == "_EVIL2";
    HenchmanLastIdleChatTimer = 0; g_rand[3] = 0; g_rand[4] = 1; g_morality = 5;  // good hero: scans round to a
    ok = ok && MakeHenchmanPerformIdleComment() && g_lastSpeaker == "_GOOD1";
    int n = g_comments;
    HenchmanLastIdleChatTimer = 0; g_rand[5] = 0; g_rand[6] = 0; g_morality = 3;  // neutral: nobody speaks, still true
    ok = ok && MakeHenchmanPerformIdleComment() && g_comments == n && HenchmanLastIdleChatTimer == 60;
    ok = ok && MakeHenchmanPerformIdleComment() && g_comments == n;               // both timers running
    std::printf(ok ? "00e5e5a0_TEST PASS\n" : "00e5e5a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
