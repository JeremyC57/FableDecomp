// Behaviour test for NScript::CV_BeggarAndChildScript::CBeggarBully::Main (0x00E5B3B0): the bully at Lookout Point. Until the beggar
// or the bully is hit or leaves: talked to the first time (belch not taught) he offers the belch lesson (yes: TEACH_BELCH, the quick
// access item, the expression tutorial once (Xbox / PC text), TaughtBelch; no: TEACH_BELCH_REFUSED) and gives the quest card once;
// later chats depend on TaughtBelch / BelchedAtBeggar / friend_talk_said (or repeat the offer); every chat pushes the taunt timer on
// by 5; when the taunt timer runs out he faces the beggar, waits 2 frames for a chat, then plays one of five disapproval animations
// (rand() % 5), says the TAUNT line to the beggar and sets BullyHasTaunted, the taunt timer to 12 and the reply timer to 5; an
// expression performed to him is answered after a 2-frame wait: belches 1..3 get a line, the 4th sets BelchedAtBully and BullyLeft,
// a fart sets BullyLeft; a hero hit (a heal-life spell excepted) sets BullyHit. On leaving the loop the three preferred expression
// items are cleared. A frame-stepped model of that logic with the engine calls logged.
#include <cstdio>
#include <cstring>
static char g_log[8192];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Bully {
    // script state (CV_BeggarAndChildScript members the entity reads / writes)
    bool BeggarHit, BeggarLeft, BullyLeft, BullyHit, TaughtBelch, BullyHasTaunted, BelchedAtBully, QuestCardGiven, ExpressionTutorialShown;
    long BelchedAtBeggar, TauntTimer, BeggarReplyTimer;
    // world inputs
    int Frame, TerminateAt, TalkAt[4], NTalk, HitAt, HealAt, ExprAt[6], NExpr, Answer, RandValue; const char* Expr[6]; bool Xbox;
    bool T() const { return Frame >= TerminateAt; }
    void NSF() { ++Frame; if (TauntTimer > 0) --TauntTimer; if (BeggarReplyTimer > 0) --BeggarReplyTimer; }
    bool Talked() const { for (int i = 0; i < NTalk; ++i) if (TalkAt[i] == Frame) return true; return false; }
    bool Hit() const { return Frame == HitAt || (Frame == HealAt && false); }   // a heal-life spell is a special ability that does not count
    const char* Performed() const { for (int i = 0; i < NExpr; ++i) if (ExprAt[i] == Frame) return Expr[i]; return 0; }
    void Tutorial() { if (!ExpressionTutorialShown) { Log(Xbox ? "TEXT_QST_015_TUTORIAL_EXPRESSIONS" : "TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC"); ExpressionTutorialShown = true; } }
    bool Wait2(bool& talked)   // the 2-frame wait both reactions share; false when the thread is terminating
    {
        int wait = 2;
        while (!talked && wait > 0) { NSF(); --wait; if (T()) return false; if (Talked()) talked = true; if (Hit()) BullyHit = true; }
        return true;
    }
    void Main()
    {
        NSF(); if (T()) return;
        Log("StartScripting"); Log("NotPushable");
        bool friend_talk_said = false, first_chat_had = false, talked_to = false; long belch_count = 0;
        while (!BeggarHit && !BullyHit && !BeggarLeft && !BullyLeft) {
            NSF(); if (T()) return;
            if (Talked()) talked_to = true;
            if (Hit()) BullyHit = true;
            else if (talked_to) {
                talked_to = false;
                if (!first_chat_had && !TaughtBelch) {
                    first_chat_had = true;
                    Log("TEXT_QST_015_BULLY_FIRST_CHAT");
                    if (Answer == 1) { Log("TEXT_QST_015_BULLY_TEACH_BELCH"); Log("QuickAccess:BELCH"); Tutorial(); TaughtBelch = true; }
                    else Log("TEXT_QST_015_BULLY_TEACH_BELCH_REFUSED");
                    if (!QuestCardGiven) { Log("OBJECT_QUEST_CARD_BEGGAR_AND_CHILD"); QuestCardGiven = true; }
                } else if (TaughtBelch) {
                    if (BelchedAtBeggar != 0) {
                        if (!friend_talk_said) { friend_talk_said = true; Log("TEXT_QST_015_BULLY_FRIEND_TALK"); }
                        else Log("TEXT_QST_015_BULLY_FRIEND_SNEER_OFFER");
                    } else Log("TEXT_QST_015_BULLY_FRIEND_BELCH_COMMAND");
                } else {
                    Log("TEXT_QST_015_BULLY_FRIEND_TALK_20");
                    if (Answer == 1) { Log("TEXT_QST_015_BULLY_TEACH_BELCH"); Log("QuickAccess:BELCH"); Tutorial(); TaughtBelch = true; }
                    else Log("TEXT_QST_015_BULLY_REPEAT_BELCH_QUESTION_REFUSAL");
                }
                TauntTimer = TauntTimer + 5;
            } else if (TauntTimer <= 0) {
                Log("FaceBeggar"); TauntTimer = 10;
                if (!Wait2(talked_to)) return;
                if (!talked_to) {
                    static const char* const anim[5] = { "DISMISSING_HAND_SWIPE", "NYAH_NYAH", "POINTING_AWAY_GET_OUT", "SHAKE_FIST", "POINT_AT" };
                    Log(anim[RandValue % 5]); Log("TEXT_QST_015_BULLY_TAUNT");
                    BullyHasTaunted = true; TauntTimer = 12; BeggarReplyTimer = 5;
                }
            }
            const char* expression = Performed();
            if (expression) {
                Log("FaceHero");
                if (!Wait2(talked_to)) return;
                if (!talked_to) {
                    if (!std::strcmp(expression, "EXPRESSION_BELCH")) {
                        ++belch_count;
                        switch (belch_count) {
                        case 1: Log("TEXT_QST_015_BULLY_FIRST_BELCH"); break;
                        case 2: Log("TEXT_QST_015_BULLY_SECOND_BELCH"); break;
                        case 3: Log("TEXT_QST_015_BULLY_THIRD_BELCH"); break;
                        case 4: BelchedAtBully = true; BullyLeft = true; break;
                        }
                    } else if (!std::strcmp(expression, "EXPRESSION_FART")) BullyLeft = true;
                }
            }
        }
        if (T()) return;
        Log("Clear:BELCH"); Log("Clear:FART"); Log("Clear:SNEER");
    }
};
static Bully Make()
{
    Bully b; std::memset(&b, 0, sizeof b);
    b.TerminateAt = 1000; b.HitAt = -1; b.HealAt = -1; b.TauntTimer = 100;
    return b;
}
int main()
{
    bool ok = true;
    {   // first chat, lesson accepted on PC, then four belches drive him off
        Bully b = Make(); b.TalkAt[0] = 3; b.NTalk = 1; b.Answer = 1;
        b.ExprAt[0] = 6; b.ExprAt[1] = 10; b.ExprAt[2] = 14; b.ExprAt[3] = 18; b.NExpr = 4;
        for (int i = 0; i < 4; ++i) b.Expr[i] = "EXPRESSION_BELCH";
        b.Main();
        ok = ok && !std::strcmp(g_log, "StartScripting;NotPushable;TEXT_QST_015_BULLY_FIRST_CHAT;TEXT_QST_015_BULLY_TEACH_BELCH;QuickAccess:BELCH;"
            "TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC;OBJECT_QUEST_CARD_BEGGAR_AND_CHILD;FaceHero;TEXT_QST_015_BULLY_FIRST_BELCH;FaceHero;"
            "TEXT_QST_015_BULLY_SECOND_BELCH;FaceHero;TEXT_QST_015_BULLY_THIRD_BELCH;FaceHero;Clear:BELCH;Clear:FART;Clear:SNEER;")
            && b.TaughtBelch && b.QuestCardGiven && b.ExpressionTutorialShown && b.BelchedAtBully && b.BullyLeft && !b.BullyHit && b.TauntTimer > 100 - 25;
    }
    g_log[0] = 0;
    {   // the taunt: timer out, nobody talks, animation 3, then the hero hits him
        Bully b = Make(); b.TauntTimer = 2; b.RandValue = 8; b.HitAt = 9;
        b.Main();
        ok = ok && !std::strcmp(g_log, "StartScripting;NotPushable;FaceBeggar;SHAKE_FIST;TEXT_QST_015_BULLY_TAUNT;Clear:BELCH;Clear:FART;Clear:SNEER;")
            && b.BullyHasTaunted && b.BullyHit && !b.BullyLeft && b.BeggarReplyTimer == 0 && b.TauntTimer == 7;
    }
    g_log[0] = 0;
    {   // lesson refused, then the repeat offer refused, then a fart; the thread dies before the loop ends in a third run
        Bully b = Make(); b.TalkAt[0] = 3; b.TalkAt[1] = 5; b.NTalk = 2; b.Answer = 0; b.ExprAt[0] = 7; b.Expr[0] = "EXPRESSION_FART"; b.NExpr = 1; b.Xbox = true;
        b.Main();
        ok = ok && !std::strcmp(g_log, "StartScripting;NotPushable;TEXT_QST_015_BULLY_FIRST_CHAT;TEXT_QST_015_BULLY_TEACH_BELCH_REFUSED;"
            "OBJECT_QUEST_CARD_BEGGAR_AND_CHILD;TEXT_QST_015_BULLY_FRIEND_TALK_20;TEXT_QST_015_BULLY_REPEAT_BELCH_QUESTION_REFUSAL;FaceHero;"
            "Clear:BELCH;Clear:FART;Clear:SNEER;") && !b.TaughtBelch && b.BullyLeft && !b.BelchedAtBully;
        g_log[0] = 0;
        Bully c = Make(); c.TerminateAt = 4; c.Main();
        ok = ok && !std::strcmp(g_log, "StartScripting;NotPushable;") && c.Frame == 4;
    }
    std::printf(ok ? "00e5b3b0_TEST PASS\n" : "00e5b3b0_TEST FAIL\n");
    return ok ? 0 : 1;
}