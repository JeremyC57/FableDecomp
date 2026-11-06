// Behaviour test for CV_GhostGrannyNecklaceScript::CGhostGrannySon::Main (0x00E8EF80): once the necklace is returned the son thanks
// the hero (ghost at rest) or sends him to his mother; a hero carrying the necklace is asked for it (No -> "find gift", Yes or shown
// item -> the outro hand-over); otherwise the first talk plays the intro (INTROA after the raid outcome 2, else INTROB), later talks
// say "oh mother", and the quest card is offered until accepted; at health <= 1 he runs away and is removed.
#include <cstdio>
#include <cstring>

struct Son {
    bool NecklaceReturned, GhostToRest, QuestCardGiven, Removed; long TalkCounter, RaidLast;
    const char* Talk(bool carries, bool shownItem, long answerReturn, long answerCard)
    {
        const char* r = "";
        if (NecklaceReturned) r = GhostToRest ? "THANKS" : "TELL_MOTHER";
        else if (carries) {
            if (!shownItem && answerReturn == 0) r = "FIND_GIFT";
            else { r = "OUTRO"; NecklaceReturned = true; }
        } else {
            if (TalkCounter == 0) r = RaidLast == 2 ? "INTROA" : "INTROB";
            else r = "OH_MOTHER";
            if (!QuestCardGiven && answerCard == 1) QuestCardGiven = true;
            ++TalkCounter;
        }
        return r;
    }
    void Health(float h) { if (h <= 1.0f) Removed = true; }
};

int main()
{
    bool ok = true;
    Son s = { false, false, false, false, 0, 2 };
    ok = ok && !std::strcmp(s.Talk(false, false, -1, 0), "INTROA") && !s.QuestCardGiven && s.TalkCounter == 1;
    ok = ok && !std::strcmp(s.Talk(false, false, -1, 1), "OH_MOTHER") && s.QuestCardGiven;
    ok = ok && !std::strcmp(s.Talk(true, false, 0, 0), "FIND_GIFT") && !s.NecklaceReturned;
    ok = ok && !std::strcmp(s.Talk(true, true, 0, 0), "OUTRO") && s.NecklaceReturned;
    ok = ok && !std::strcmp(s.Talk(true, false, 1, 0), "TELL_MOTHER");
    s.GhostToRest = true; ok = ok && !std::strcmp(s.Talk(false, false, 0, 0), "THANKS");
    Son t = { false, false, false, false, 0, 1 };
    ok = ok && !std::strcmp(t.Talk(false, false, -1, 0), "INTROB");
    t.Health(1.5f); ok = ok && !t.Removed; t.Health(1.0f); ok = ok && t.Removed;
    if (!ok) { std::printf("00e8ef80_TEST FAIL\n"); return 1; }
    std::printf("00e8ef80_TEST PASS\n");
    return 0;
}
