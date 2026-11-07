// Behaviour test for NScript::CV_SickChildScript::CMansLover::Main (0x00EC9D30): the man's lover. Talked to, or
// shown the man's love letter, while the hero carries that letter: unless shown it she says INTRO and asks whether he brings a letter
// (no: MAN_NOT_SEEN_10); then she takes it, says FROM_YOU_10 and asks whether it is from the hero: no gives MAN_LIKES_ME_10/20, her reply
// letter, the truth morality and ML_NOT_IN_LOVE_WITH_YOU; yes gives FROM_YOU_20/30, the woman's letter for the hero, LIKES_YOU_10, the
// lying morality and ML_IN_LOVE_WITH_YOU; either way she starts leaving. Without the letter she answers by lover state, or (leaving, rand
// even) LEAVING_REGION_10. While leaving and idle she walks to MansLoverLeavesHere and fades out within 2 units. A state model.
#include <cstdio>
#include <cstring>
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
enum { ML_NOT_RECEIVED_LETTER, ML_IN_LOVE_WITH_YOU, ML_NOT_IN_LOVE_WITH_YOU };
struct MansLover {
    int Frames, TerminateAt, TalkAt[4], NTalk, PresentAt, Answer[4], Answer2, Rand[4], State; bool HeroHasLetter, LeavingRegion; int Dist;
    int NAnswer, NRand; bool Dead;
    bool T() const { return Frames >= TerminateAt; }
    bool Talked() const { for (int i = 0; i < NTalk; ++i) if (TalkAt[i] == Frames) return true; return false; }
    void Main()
    {
        bool ShownLetter = false;
        for (;; ++Frames) {
            if (T()) return;
            if (Frames == PresentAt) ShownLetter = true;
            if (Talked() || ShownLetter) {
                if (HeroHasLetter) {
                    bool GiveLetter = false;
                    if (!ShownLetter) {
                        Log("INTRO"); Log("GIVE_LETTER_QUESTION");
                        if (Answer[NAnswer++] == 1) GiveLetter = true;
                        else Log("MAN_NOT_SEEN_10");
                    }
                    else GiveLetter = true;
                    if (GiveLetter) {
                        ShownLetter = false;
                        HeroHasLetter = false; Log("TAKE"); Log("FROM_YOU_10"); Log("LETTER_FROM_QUESTION");
                        if (Answer2 == 0) {
                            Log("MAN_LIKES_ME_10"); Log("MAN_LIKES_ME_20"); Log("GIVE_REPLY"); Log("MORALITY_TRUTH");
                            State = ML_NOT_IN_LOVE_WITH_YOU;
                        }
                        else {
                            Log("FROM_YOU_20"); Log("FROM_YOU_30"); Log("GIVE_WOMANS_LETTER"); Log("LIKES_YOU_10"); Log("MORALITY_LIE");
                            State = ML_IN_LOVE_WITH_YOU;
                        }
                        LeavingRegion = true;
                    }
                }
                else if (LeavingRegion && Rand[NRand++] % 2 == 0) Log("LEAVING_REGION_10");
                else switch (State) {
                    case ML_NOT_RECEIVED_LETTER: Log("INTRO_PRELETTER"); break;
                    case ML_NOT_IN_LOVE_WITH_YOU: Log("IN_LOVE_WITH_MAN"); break;
                    default: Log("IN_LOVE_WITH_YOU_10"); break;
                }
            }
            if (LeavingRegion) {
                if (Dist > 2) { Log("WALK"); --Dist; }
                else { Log("FADE"); Dead = true; return; }
            }
        }
    }
};
int main()
{
    MansLover a = { 0, 20, { 2, 4 }, 2, -1, { 0, 1 }, 0, { 0 }, ML_NOT_RECEIVED_LETTER, true, false, 3 };
    a.Main();
    bool ok = !std::strcmp(g_log, "INTRO;GIVE_LETTER_QUESTION;MAN_NOT_SEEN_10;INTRO;GIVE_LETTER_QUESTION;TAKE;FROM_YOU_10;LETTER_FROM_QUESTION;"
                                  "MAN_LIKES_ME_10;MAN_LIKES_ME_20;GIVE_REPLY;MORALITY_TRUTH;WALK;FADE;")
        && a.State == ML_NOT_IN_LOVE_WITH_YOU && a.Dead && a.Frames == 5;
    g_log[0] = 0;
    MansLover b = { 0, 20, { 0 }, 0, 3, { 0 }, 1, { 0 }, ML_NOT_RECEIVED_LETTER, true, false, 1 };
    b.Main();
    ok = ok && !std::strcmp(g_log, "TAKE;FROM_YOU_10;LETTER_FROM_QUESTION;FROM_YOU_20;FROM_YOU_30;GIVE_WOMANS_LETTER;LIKES_YOU_10;MORALITY_LIE;FADE;")
        && b.State == ML_IN_LOVE_WITH_YOU && b.Dead;
    g_log[0] = 0;
    MansLover c = { 0, 5, { 1, 2 }, 2, -1, { 0 }, 0, { 0 }, ML_NOT_IN_LOVE_WITH_YOU, false, false, 9 };
    c.Main();
    ok = ok && !std::strcmp(g_log, "IN_LOVE_WITH_MAN;IN_LOVE_WITH_MAN;") && !c.Dead;
    g_log[0] = 0;
    MansLover d = { 0, 3, { 1, 2 }, 2, -1, { 0 }, 0, { 0, 1 }, ML_NOT_IN_LOVE_WITH_YOU, false, true, 100 };
    d.Main();
    ok = ok && !std::strcmp(g_log, "WALK;LEAVING_REGION_10;WALK;IN_LOVE_WITH_MAN;WALK;") && d.Dist == 97;
    std::printf(ok ? "00ec9d30_TEST PASS\n" : "00ec9d30_TEST FAIL\n");
    return ok ? 0 : 1;
}
