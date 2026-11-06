// Behaviour test for NScript::CV_SickChildScript::CManInLove::Main (0x00EC8300): a hero talk gives the intro and the letter the first
// time (lover alive); later, holding the lover's object or letter before the reward, he asks for it back (yes = reward + morality by
// the lover's state; no = the opinion question: yes = happy, no = unhappy, both rewarded); without it: lover killed = reward, else
// deliver the letter; after the reward a post-quest line. A hero hit (not a heal spell) = ON_HIT; once rewarded he walks to the
// exit point and fades out there.
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
enum { ML_IN_LOVE = 0, ML_NOT_IN_LOVE_WITH_YOU = 1, ML_KILLED = 2 };
struct Man {
    int Frames, TerminateAt, TalkMask, HitMask, HealMask, Answer, Answer2, LoverState, ExitDistance, Morality;
    bool GivenLetter, GotReward, Helped, HoldingObject, HasInfo, Faded;
    bool T() const { return Frames >= TerminateAt || Faded; }
    void NSF() { ++Frames; }
    bool Bit(int m) const { return Frames < 31 && ((m >> Frames) & 1); }
    void Reward() { Log("ingredient"); GotReward = true; }
    void Talk() {
        if (!GivenLetter && LoverState != ML_KILLED) {
            Log("INTRO"); Log("give_letter"); GivenLetter = true; HasInfo = false;
        } else if (HoldingObject && !GotReward) {
            if (Answer == 1) {
                HoldingObject = false; Log("RETURNED"); Reward(); Helped = true;
                Morality += LoverState == ML_NOT_IN_LOVE_WITH_YOU ? 1 : -1; HasInfo = false;
            } else {
                Log("DIDNT_RETURN");
                if (Answer2 == 1) { Log("HAPPY"); Reward(); Helped = true; Morality += LoverState == ML_NOT_IN_LOVE_WITH_YOU ? 1 : -1; }
                else { Log("UNHAPPY"); Reward(); Morality += LoverState == ML_NOT_IN_LOVE_WITH_YOU ? -1 : 1; }
                HasInfo = false;
            }
        } else if (!GotReward) {
            if (LoverState == ML_KILLED) { Log("MYRA_DIED"); Reward(); } else Log("DELIVER_LETTER");
        } else Log(Helped ? "POST_HAPPY" : LoverState == ML_KILLED ? "POST_MYRA_DIED" : "POST_UNHAPPY");
    }
    void Main() {
        if (HoldingObject || !GivenLetter || LoverState == ML_KILLED) HasInfo = true;
        for (; !T(); NSF()) {
            if (Bit(TalkMask)) Talk();
            if (Bit(HitMask) && !Bit(HealMask)) Log("ON_HIT");
            if (GotReward) {
                if (ExitDistance > 2) { Log("walk_exit"); --ExitDistance; }
                else { Log("fade"); Faded = true; }
            }
        }
    }
};
int main()
{
    Man a = { 0, 40, (1 << 1) | (1 << 3), 0, 0, 0, 0, ML_IN_LOVE, 0, 0, false, false, false, false, false, false };
    a.Main();
    bool ok = !std::strcmp(g_log, "INTRO;give_letter;DELIVER_LETTER;") && a.GivenLetter && !a.HasInfo && a.Frames == 40;
    g_log[0] = 0;
    Man b = { 0, 40, (1 << 1), (1 << 2), 0, 1, 0, ML_NOT_IN_LOVE_WITH_YOU, 3, 0, true, false, false, true, false, false };
    b.Main();
    ok = ok && !std::strcmp(g_log, "RETURNED;ingredient;walk_exit;ON_HIT;fade;") && b.Morality == 1 && b.Helped && b.Faded;
    g_log[0] = 0;
    Man c = { 0, 40, (1 << 0), 0, 0, 0, 0, ML_IN_LOVE, 2, 0, true, false, false, true, false, false };
    c.Main();
    ok = ok && !std::strcmp(g_log, "DIDNT_RETURN;UNHAPPY;ingredient;fade;") && c.Morality == 1 && !c.Helped;
    g_log[0] = 0;
    Man d = { 0, 4, (1 << 0), (1 << 1) | (1 << 2), (1 << 2), 0, 0, ML_KILLED, 9, 0, false, false, false, false, false, false };
    d.Main();
    ok = ok && !std::strcmp(g_log, "MYRA_DIED;ingredient;walk_exit;ON_HIT;walk_exit;walk_exit;walk_exit;") && d.HasInfo;
    std::printf(ok ? "00ec8300_TEST PASS\n" : "00ec8300_TEST FAIL\n");
    return ok ? 0 : 1;
}
