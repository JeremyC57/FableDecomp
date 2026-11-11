// Behaviour test for 0x006DE8C0 CTCTrophy::OnShowTrophyPerformAction: a host copy of the row's logic over plain models (the two
// witness lists, the creatures' perception / awareness / opinion components and the effects), checked list by list.
#include <cstdio>
#include <cstring>

enum { DEED_EVIL = 55, DEED_GOOD = 56, DEED_BORED = 57 };
enum { PERS_NONE = 0, PERS_BORED = 1, PERS_EVIL = 2, PERS_GOOD = 3 };

struct Creature {
    bool Alive, HasPerceive, IgnoreFovSight, Aware, HasOpinion, HasWife, Married, Divorced, HasEmote, Bandit;
    int Deed, Personality, StateGroupRoot, EmoteCleared;
};
struct Trophy {
    bool InProgress, EmoteForAvailable, EvilTrophy;
    long FramesLeft, StartingWitnesses, WitnessesLeft, PendingSounds;
    float TimeBonus;
    Creature* Available[8]; int NumAvailable;
    Creature* Achieved[8]; int NumAchieved;
    int AllWitnessesSounds, Dialogs; bool WaitingForTerminate; long DialogCount;
};

static const long FPS = 30;

static bool Noticed(Creature* c) { return c && c->Alive && c->HasPerceive && c->IgnoreFovSight && c->Aware; }

// The row's logic (same order of tests and effects as 0x006DE8C0).
static void OnShowTrophyPerformAction(Trophy& t)
{
    if (!t.InProgress) return;
    for (int i = 0; i < t.NumAchieved; ++i) {
        Creature* c = t.Achieved[i];
        if (!Noticed(c)) continue;
        c->Deed = DEED_BORED;
        if (c->HasOpinion) c->Personality = PERS_BORED;
        c->StateGroupRoot++;
    }
    for (int i = 0; i < t.NumAvailable; ) {
        Creature* c = t.Available[i];
        if (!Noticed(c)) { ++i; continue; }
        for (int j = i; j + 1 < t.NumAvailable; ++j) t.Available[j] = t.Available[j + 1];   // erase
        t.NumAvailable--;
        t.Achieved[t.NumAchieved++] = c;                                                 // push_back
        if (t.EmoteForAvailable) c->EmoteCleared++;
        t.FramesLeft = (long)(t.FramesLeft + FPS * t.TimeBonus);
        t.PendingSounds++;
        c->Deed = t.EvilTrophy == c->Bandit ? DEED_GOOD : DEED_EVIL;
        if (c->HasOpinion && (!c->HasWife || (!c->Married && !c->Divorced)))
            c->Personality = t.EvilTrophy ? PERS_EVIL : PERS_GOOD;
        c->StateGroupRoot++;
        t.WitnessesLeft--;
    }
    if (t.WitnessesLeft <= 0) {
        t.AllWitnessesSounds++;
        t.Dialogs++; t.DialogCount = t.StartingWitnesses;
        t.WaitingForTerminate = true;
    }
}

static Creature Make(bool noticed)
{
    Creature c; std::memset(&c, 0, sizeof(c));
    c.Alive = c.HasPerceive = c.IgnoreFovSight = true; c.Aware = noticed; c.HasOpinion = true;
    return c;
}

int main()
{
    Trophy t; bool ok = true;
    std::memset(&t, 0, sizeof(t));
    t.InProgress = true; t.EmoteForAvailable = true; t.TimeBonus = 2.0f; t.FramesLeft = 100; t.StartingWitnesses = 3; t.WitnessesLeft = 3;
    Creature old1 = Make(true), old2 = Make(false), a = Make(true), b = Make(false), c = Make(true), d = Make(true);
    c.Bandit = true; d.HasWife = true; d.Married = true;
    t.Achieved[0] = &old1; t.Achieved[1] = &old2; t.NumAchieved = 2;
    t.Available[0] = &a; t.Available[1] = &b; t.Available[2] = &c; t.Available[3] = &d; t.NumAvailable = 4;

    OnShowTrophyPerformAction(t);
    // the already-gained witness who noticed gets bored; the other is untouched
    ok = ok && old1.Deed == DEED_BORED && old1.Personality == PERS_BORED && old1.StateGroupRoot == 1 && old2.Deed == 0;
    // a, c, d move over (good trophy: a good deed for non-bandits, an evil one for the bandit); b stays available
    ok = ok && t.NumAvailable == 1 && t.Available[0] == &b && t.NumAchieved == 5 && t.Achieved[2] == &a && t.Achieved[4] == &d;
    ok = ok && a.Deed == DEED_GOOD && c.Deed == DEED_EVIL && d.Deed == DEED_GOOD;
    ok = ok && a.Personality == PERS_GOOD && c.Personality == PERS_GOOD && d.Personality == PERS_NONE;   // d is married to the hero
    ok = ok && a.EmoteCleared == 1 && t.FramesLeft == 100 + 3 * 60 && t.PendingSounds == 3 && t.WitnessesLeft == 0;
    ok = ok && t.AllWitnessesSounds == 1 && t.Dialogs == 1 && t.DialogCount == 3 && t.WaitingForTerminate;

    // not in progress: nothing happens
    Trophy u; std::memset(&u, 0, sizeof(u)); u.WitnessesLeft = 0;
    OnShowTrophyPerformAction(u);
    ok = ok && u.Dialogs == 0;

    if (!ok) { std::printf("006de8c0_TEST FAIL\n"); return 1; }
    std::printf("006de8c0_TEST PASS\n");
    return 0;
}
