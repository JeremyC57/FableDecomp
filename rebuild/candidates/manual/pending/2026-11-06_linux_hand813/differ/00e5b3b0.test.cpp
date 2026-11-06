// Behaviour test for CBeggarBully::Main (0x00E5B3B0): the per-frame step of the Bowerstone bully -- the first chat teaches the
// belch on a yes (tutorial once, quest card once), later chats depend on TaughtBelch / BelchedAtBeggar, the taunt timer drives
// the disapproval + TAUNT line, and belches (FIRST / SECOND / THIRD line, leave on the fourth) or a fart make him leave.
#include <cstdio>
#include <cstring>

struct Parent {
    bool BeggarHit, BeggarLeft, BullyLeft, BullyHit, TaughtBelch, BullyHasTaunted, BelchedAtBully, QuestCardGiven, ExpressionTutorialShown;
    long BelchedAtBeggar, TauntTimer, BeggarReplyTimer;
};

struct Bully {
    Parent* P;
    bool FriendTalkSaid, FirstChatHad;
    long BelchCount, Tutorials, Cards, Taunts;
    char Last[48];
    void Say(const char* t) { std::strcpy(Last, t); }
    bool Running() const { return !P->BeggarHit && !P->BullyHit && !P->BeggarLeft && !P->BullyLeft; }
    void Teach(long answer, const char* refusal)
    {
        if (answer == 1) {
            Say("TEACH_BELCH");
            if (!P->ExpressionTutorialShown) { ++Tutorials; P->ExpressionTutorialShown = true; }
            P->TaughtBelch = true;
        } else Say(refusal);
    }
    void Step(bool talked, bool hit, long answer, int animation, const char* expression)
    {
        if (hit) P->BullyHit = true;
        else if (talked) {
            if (!FirstChatHad && !P->TaughtBelch) {
                FirstChatHad = true;
                Say("FIRST_CHAT");
                Teach(answer, "TEACH_BELCH_REFUSED");
                if (!P->QuestCardGiven) { ++Cards; P->QuestCardGiven = true; }
            } else if (P->TaughtBelch) {
                if (P->BelchedAtBeggar != 0) {
                    if (!FriendTalkSaid) { FriendTalkSaid = true; Say("FRIEND_TALK"); }
                    else Say("FRIEND_SNEER_OFFER");
                } else Say("FRIEND_BELCH_COMMAND");
            } else {
                Say("FRIEND_TALK_20");
                Teach(answer, "REPEAT_BELCH_QUESTION_REFUSAL");
            }
            P->TauntTimer += 5;
        } else if (P->TauntTimer <= 0) {
            P->TauntTimer = 10;
            if (animation >= 0 && animation < 5) {
                ++Taunts; Say("TAUNT");
                P->BullyHasTaunted = true; P->TauntTimer = 12; P->BeggarReplyTimer = 5;
            }
        }
        if (expression) {
            if (std::strcmp(expression, "EXPRESSION_BELCH") == 0) {
                ++BelchCount;
                switch (BelchCount) {
                case 1: Say("FIRST_BELCH"); break;
                case 2: Say("SECOND_BELCH"); break;
                case 3: Say("THIRD_BELCH"); break;
                case 4: P->BelchedAtBully = true; P->BullyLeft = true; break;
                }
            } else if (std::strcmp(expression, "EXPRESSION_FART") == 0) P->BullyLeft = true;
        }
    }
};

int main()
{
    Parent p; std::memset(&p, 0, sizeof p);
    Bully b; std::memset(&b, 0, sizeof b); b.P = &p;
    bool ok = true;
    p.TauntTimer = 3;
    b.Step(true, false, 0, 0, 0);
    ok = ok && std::strcmp(b.Last, "TEACH_BELCH_REFUSED") == 0 && b.Cards == 1 && !p.TaughtBelch && p.TauntTimer == 8;
    b.Step(true, false, 1, 0, 0);
    ok = ok && std::strcmp(b.Last, "TEACH_BELCH") == 0 && p.TaughtBelch && b.Tutorials == 1 && b.Cards == 1;
    b.Step(true, false, 0, 0, 0);
    ok = ok && std::strcmp(b.Last, "FRIEND_BELCH_COMMAND") == 0;
    p.BelchedAtBeggar = 1;
    b.Step(true, false, 0, 0, 0);
    ok = ok && std::strcmp(b.Last, "FRIEND_TALK") == 0;
    b.Step(true, false, 0, 0, 0);
    ok = ok && std::strcmp(b.Last, "FRIEND_SNEER_OFFER") == 0;
    p.TauntTimer = 0;
    b.Step(false, false, 0, 3, 0);
    ok = ok && b.Taunts == 1 && p.BullyHasTaunted && p.TauntTimer == 12 && p.BeggarReplyTimer == 5;
    b.Step(false, false, 0, 0, "EXPRESSION_SNEER");
    ok = ok && b.BelchCount == 0 && b.Running();
    b.Step(false, false, 0, 0, "EXPRESSION_BELCH");
    b.Step(false, false, 0, 0, "EXPRESSION_BELCH");
    b.Step(false, false, 0, 0, "EXPRESSION_BELCH");
    ok = ok && std::strcmp(b.Last, "THIRD_BELCH") == 0 && b.Running();
    b.Step(false, false, 0, 0, "EXPRESSION_BELCH");
    ok = ok && p.BelchedAtBully && p.BullyLeft && !b.Running();
    Parent q; std::memset(&q, 0, sizeof q); b.P = &q; q.TauntTimer = 9;
    b.Step(false, false, 0, 0, "EXPRESSION_FART");
    ok = ok && q.BullyLeft && !q.BelchedAtBully;
    q.BullyLeft = false;
    b.Step(false, true, 0, 0, 0);
    ok = ok && q.BullyHit && !b.Running();
    if (!ok) { std::printf("00e5b3b0_TEST FAIL\n"); return 1; }
    std::printf("00e5b3b0_TEST PASS\n");
    return 0;
}
