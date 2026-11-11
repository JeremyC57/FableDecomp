// Behaviour test for NScript::CV_BeggarAndChildScript::CLookoutPointBeggar::Main (0x00E58D40): the beggar at Lookout Point. Until
// the beggar or the bully is hit or leaves: the first chat (battle cry not taught) is FIRST_SPEAK or, after a belch at him,
// FIRST_SPEAK_BELCH, then the lesson offer (yes: TEACH_BATTLE_CRY, the quick access item, the expression tutorial once, TaughtBattleCry;
// no: TEACH_REFUSED) and the quest card once; later chats say REPEAT_BATTLE_CRY when taught, else repeat the offer (after a belch with
// REPEAT_BELCH_RETURN first; a refusal answers ..._REPEAT_FAIL_BELCH or ..._REPEAT_FAIL); every chat pushes the taunt timer on by 5.
// When the bully has taunted and the reply timer is out he faces the bully, clears BullyHasTaunted, waits 2 frames and answers with
// an animation (rand() % 5) and one of four reply lines (each next rand() % 4 == 0 picks 01 / 02 / 03, else 04). An expression performed
// to him: belches 1..3 get a line and the 4th sets BeggarLeft and BelchedAtBully; farts get the first / second / third battle-cry reply.
// A hero hit (heal-life excepted) sets BeggarHit. A frame-stepped model with the engine calls logged.
#include <cstdio>
#include <cstring>
static char g_log[8192];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Beggar {
    bool BeggarHit, BeggarLeft, BullyLeft, BullyHit, TaughtBattleCry, BullyHasTaunted, BelchedAtBully, QuestCardGiven, ExpressionTutorialShown;
    long BelchedAtBeggar, TauntTimer, BeggarReplyTimer;
    int Frame, TerminateAt, TalkAt[4], NTalk, HitAt, ExprAt[6], NExpr, Answer, Rand[8], NRand; const char* Expr[6]; bool Xbox;
    bool T() const { return Frame >= TerminateAt; }
    void NSF() { ++Frame; if (TauntTimer > 0) --TauntTimer; if (BeggarReplyTimer > 0) --BeggarReplyTimer; }
    bool Talked() const { for (int i = 0; i < NTalk; ++i) if (TalkAt[i] == Frame) return true; return false; }
    bool Hit() const { return Frame == HitAt; }
    int rnd() { return Rand[NRand++]; }
    const char* Performed() const { for (int i = 0; i < NExpr; ++i) if (ExprAt[i] == Frame) return Expr[i]; return 0; }
    void Offer(const char* refused)
    {
        if (Answer == 1) {
            Log("TEXT_QST_015_BEGGAR_TEACH_BATTLE_CRY"); Log("QuickAccess:FART");
            if (!ExpressionTutorialShown) { Log(Xbox ? "TEXT_QST_015_TUTORIAL_EXPRESSIONS" : "TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC"); ExpressionTutorialShown = true; }
            TaughtBattleCry = true;
        } else Log(refused);
    }
    bool Wait2(bool& talked)
    {
        int wait = 2;
        while (!talked && wait > 0) { NSF(); --wait; if (T()) return false; if (Talked()) talked = true; if (Hit()) BeggarHit = true; }
        return true;
    }
    void Main()
    {
        NSF(); if (T()) return;
        Log("StartScripting");
        bool first_chat_had = false, talked_to = false; long battle_cry_count = 0;
        while (!BeggarHit && !BullyHit && !BeggarLeft && !BullyLeft) {
            NSF(); if (T()) return;
            if (Talked()) talked_to = true;
            if (Hit()) BeggarHit = true;
            else if (talked_to) {
                talked_to = false;
                if (!first_chat_had && !TaughtBattleCry) {
                    first_chat_had = true;
                    Log(BelchedAtBeggar != 0 ? "TEXT_QST_015_BEGGAR_FIRST_SPEAK_BELCH" : "TEXT_QST_015_BEGGAR_FIRST_SPEAK");
                    Offer("TEXT_QST_015_BEGGAR_TEACH_REFUSED");
                    if (!QuestCardGiven) { Log("OBJECT_QUEST_CARD_BEGGAR_AND_CHILD"); QuestCardGiven = true; }
                } else if (BelchedAtBeggar != 0) {
                    if (TaughtBattleCry) Log("TEXT_QST_015_BEGGAR_REPEAT_BATTLE_CRY");
                    else { Log("TEXT_QST_015_BEGGAR_REPEAT_BELCH_RETURN"); Offer("TEXT_QST_015_BEGGAR_TEACH_OFFER_REPEAT_FAIL_BELCH"); }
                } else {
                    if (TaughtBattleCry) Log("TEXT_QST_015_BEGGAR_REPEAT_BATTLE_CRY");
                    else Offer("TEXT_QST_015_BEGGAR_TEACH_OFFER_REPEAT_FAIL");
                }
                TauntTimer = TauntTimer + 5;
            } else if (BullyHasTaunted && BeggarReplyTimer <= 0) {
                Log("FaceBully"); BullyHasTaunted = false;
                if (!Wait2(talked_to)) return;
                if (!talked_to) {
                    static const char* const anim[5] = { "DISMISSING_HAND_SWIPE", "CALLING_OVER_FOR_FIGHT", "POINTING_AWAY_GET_OUT", "SHAKE_FIST", "POINT_AT" };
                    Log(anim[rnd() % 5]);
                    if (rnd() % 4 == 0) Log("TEXT_QST_015_BEGGAR_TAUNT_REPLY_01");
                    else if (rnd() % 4 == 0) Log("TEXT_QST_015_BEGGAR_TAUNT_REPLY_02");
                    else if (rnd() % 4 == 0) Log("TEXT_QST_015_BEGGAR_TAUNT_REPLY_03");
                    else Log("TEXT_QST_015_BEGGAR_TAUNT_REPLY_04");
                }
            }
            const char* expression = Performed();
            if (expression) {
                Log("FaceHero");
                if (!Wait2(talked_to)) return;
                if (!talked_to) {
                    if (!std::strcmp(expression, "EXPRESSION_BELCH")) {
                        ++BelchedAtBeggar;
                        switch (BelchedAtBeggar) {
                        case 1: Log("TEXT_QST_015_BEGGAR_FIRST_BELCH_REPLY"); break;
                        case 2: Log("TEXT_QST_015_BEGGAR_SECOND_BELCH_REPLY"); break;
                        case 3: Log("TEXT_QST_015_BEGGAR_THIRD_BELCH_REPLY"); break;
                        case 4: BeggarLeft = true; BelchedAtBully = true; break;
                        }
                    } else if (!std::strcmp(expression, "EXPRESSION_FART")) {
                        ++battle_cry_count;
                        switch (battle_cry_count) {
                        case 1: Log("TEXT_QST_015_BEGGAR_FIRST_BATTLE_CRY_REPLY"); break;
                        case 2: Log("TEXT_QST_015_BEGGAR_SECOND_BATTLE_CRY_REPLY"); break;
                        default: Log("TEXT_QST_015_BEGGAR_THIRD_BATTLE_CRY_REPLY"); break;
                        }
                    }
                }
            }
        }
        if (T()) return;
        Log("Release");
    }
};
static Beggar Make()
{
    Beggar b; std::memset(&b, 0, sizeof b);
    b.TerminateAt = 1000; b.HitAt = -1;
    return b;
}
int main()
{
    bool ok = true;
    {   // first chat accepted (Xbox tutorial), repeat chat, three farts, then the hero hits him
        Beggar b = Make(); b.TalkAt[0] = 3; b.TalkAt[1] = 5; b.NTalk = 2; b.Answer = 1; b.Xbox = true; b.HitAt = 20;
        b.ExprAt[0] = 7; b.ExprAt[1] = 10; b.ExprAt[2] = 13; b.NExpr = 3;
        for (int i = 0; i < 3; ++i) b.Expr[i] = "EXPRESSION_FART";
        b.Main();
        ok = ok && !std::strcmp(g_log, "StartScripting;TEXT_QST_015_BEGGAR_FIRST_SPEAK;TEXT_QST_015_BEGGAR_TEACH_BATTLE_CRY;QuickAccess:FART;"
            "TEXT_QST_015_TUTORIAL_EXPRESSIONS;OBJECT_QUEST_CARD_BEGGAR_AND_CHILD;TEXT_QST_015_BEGGAR_REPEAT_BATTLE_CRY;FaceHero;"
            "TEXT_QST_015_BEGGAR_FIRST_BATTLE_CRY_REPLY;FaceHero;TEXT_QST_015_BEGGAR_SECOND_BATTLE_CRY_REPLY;FaceHero;"
            "TEXT_QST_015_BEGGAR_THIRD_BATTLE_CRY_REPLY;Release;") && b.TaughtBattleCry && b.BeggarHit && b.QuestCardGiven && b.TauntTimer == 0;
    }
    g_log[0] = 0;
    {   // a belch first, then the first chat refused, the repeat offer refused, three more belches: he leaves
        Beggar b = Make(); b.Answer = 0; b.TalkAt[0] = 8; b.TalkAt[1] = 10; b.NTalk = 2;
        b.ExprAt[0] = 3; b.ExprAt[1] = 12; b.ExprAt[2] = 15; b.ExprAt[3] = 18; b.NExpr = 4;
        for (int i = 0; i < 4; ++i) b.Expr[i] = "EXPRESSION_BELCH";
        b.Main();
        ok = ok && !std::strcmp(g_log, "StartScripting;FaceHero;TEXT_QST_015_BEGGAR_FIRST_BELCH_REPLY;TEXT_QST_015_BEGGAR_FIRST_SPEAK_BELCH;"
            "TEXT_QST_015_BEGGAR_TEACH_REFUSED;OBJECT_QUEST_CARD_BEGGAR_AND_CHILD;TEXT_QST_015_BEGGAR_REPEAT_BELCH_RETURN;"
            "TEXT_QST_015_BEGGAR_TEACH_OFFER_REPEAT_FAIL_BELCH;FaceHero;TEXT_QST_015_BEGGAR_SECOND_BELCH_REPLY;FaceHero;"
            "TEXT_QST_015_BEGGAR_THIRD_BELCH_REPLY;FaceHero;Release;") && b.BeggarLeft && b.BelchedAtBully && b.BelchedAtBeggar == 4 && !b.TaughtBattleCry;
    }
    g_log[0] = 0;
    {   // the bully has taunted: reply 03 with animation 1, then the bully leaves; and a terminated thread
        Beggar b = Make(); b.BullyHasTaunted = true; b.Rand[0] = 6; b.Rand[1] = 1; b.Rand[2] = 2; b.Rand[3] = 4; b.TerminateAt = 9;
        b.Main();
        ok = ok && !std::strcmp(g_log, "StartScripting;FaceBully;CALLING_OVER_FOR_FIGHT;TEXT_QST_015_BEGGAR_TAUNT_REPLY_03;")
            && !b.BullyHasTaunted && b.NRand == 4 && b.Frame == 9;
    }
    std::printf(ok ? "00e58d40_TEST PASS\n" : "00e58d40_TEST FAIL\n");
    return ok ? 0 : 1;
}