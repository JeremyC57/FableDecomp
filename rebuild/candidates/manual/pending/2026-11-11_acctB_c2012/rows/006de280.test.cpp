// Behaviour test for 0x006DE280 CTCTrophy::TerminateMinigame: a host copy of the row's logic over plain models (the carrier / hero,
// the two witness lists, the hero stats and the dialog / sound / deed effects), checked for the sheathed, fallback and score paths.
#include <cstdio>
#include <cstring>

enum { DEED_NONE = 0, DEED_ANNOYING_LARGE = 43, DEED_NICE_LARGE = 45 };
enum { TEXT_NONE = 0, TEXT_SCORED = 1, TEXT_BEATEN = 2, TEXT_FAILED = 3 };

struct Witness {
    bool Present, Dead, HasOpinion, HasEmote;
    int StateGroupRoot, PersonalityCleared, EmoteSet;
};
struct Thing {
    bool Creature, Dead, SheatheAccepted, HasStats, HasDeedLog;
    bool CurrentAction; long InterruptionGroup;
    int Finished, Sheathes, Deed;
    long BestTrophyWitnesses, Renown;
};
struct Trophy {
    bool InProgress, WaitingForScore, Evil, EmoteForAvailable, ReturnedToInventory, HasInventoryItem;
    long BestWitnessesAheadToDate, RenownPerWitness;
    Thing* Carrier; Thing* Hero;
    Witness* Gained[8]; int NumGained;
    Witness* Available[8]; int NumAvailable;
    int Texts[4]; long TextArgs[4][3]; int NumTexts;
    int HighScoreSounds, NoScoreSounds, MiniGameInfoOff; bool RegionConfirmationWaiting;
};

static void ShowText(Trophy& t, int text, long a, long b, long c)
{
    t.Texts[t.NumTexts] = text; t.TextArgs[t.NumTexts][0] = a; t.TextArgs[t.NumTexts][1] = b; t.TextArgs[t.NumTexts][2] = c; t.NumTexts++;
}

// The row's logic (same order of tests and effects as 0x006DE280).
static void TerminateMinigame(Trophy& t)
{
    t.InProgress = false;
    Thing* holder = 0;
    Thing* carrier = t.Carrier;
    bool sheathed = false;
    if (carrier && carrier->Creature) {
        holder = carrier;
        if (carrier->CurrentAction && carrier->InterruptionGroup <= 0)
            carrier->Finished++;
        carrier->Sheathes++;
        if (carrier->SheatheAccepted)
            sheathed = true;
    }
    if (!sheathed) {
        holder = t.Hero;
        if (t.HasInventoryItem)
            t.ReturnedToInventory = true;
    }
    for (int i = 0; i < t.NumGained; ++i) {
        Witness* w = t.Gained[i];
        if (w->Present && !w->Dead) {
            w->StateGroupRoot++;
            if (w->HasOpinion) w->PersonalityCleared++;
        }
    }
    for (int i = 0; i < t.NumAvailable; ++i) {
        Witness* w = t.Available[i];
        if (w->Present && !w->Dead) {
            w->StateGroupRoot++;
            if (t.EmoteForAvailable) w->EmoteSet++;
        }
    }
    if (holder && !holder->Dead) {
        long witnesses = t.NumGained - t.NumAvailable;
        if (witnesses < 0) witnesses = 0;
        if (witnesses > holder->BestTrophyWitnesses) holder->BestTrophyWitnesses = witnesses;
        ShowText(t, TEXT_SCORED, t.NumGained, t.NumAvailable, witnesses);
        t.WaitingForScore = true;
        if (witnesses > t.BestWitnessesAheadToDate) {
            long renown = (witnesses - t.BestWitnessesAheadToDate) * t.RenownPerWitness;
            holder->Renown += renown;
            if (!holder->Dead && holder->HasDeedLog) holder->Deed = t.Evil ? DEED_ANNOYING_LARGE : DEED_NICE_LARGE;
            t.HighScoreSounds++;
            ShowText(t, TEXT_BEATEN, t.BestWitnessesAheadToDate, renown, 0);
            t.BestWitnessesAheadToDate = witnesses;
        } else {
            t.NoScoreSounds++;
            ShowText(t, TEXT_FAILED, t.BestWitnessesAheadToDate, 0, 0);
        }
    }
    t.MiniGameInfoOff++;
    t.RegionConfirmationWaiting = false;
}

static Witness MakeWitness(bool present, bool dead)
{
    Witness w; std::memset(&w, 0, sizeof(w));
    w.Present = present; w.Dead = dead; w.HasOpinion = true; w.HasEmote = true;
    return w;
}

int main()
{
    bool ok = true;
    // 1. a creature carries the trophy and sheathes it: it is the holder, scores 3 - 1 = 2 against a best of 1 (renown (2 - 1) x 5 = 5)
    Witness g1 = MakeWitness(true, false), g2 = MakeWitness(true, true), g3 = MakeWitness(false, false), a1 = MakeWitness(true, false);
    Thing carrier; std::memset(&carrier, 0, sizeof(carrier));
    carrier.Creature = true; carrier.SheatheAccepted = true; carrier.CurrentAction = true; carrier.InterruptionGroup = 0;
    carrier.HasDeedLog = true; carrier.BestTrophyWitnesses = 1;
    Thing hero; std::memset(&hero, 0, sizeof(hero));
    Trophy t; std::memset(&t, 0, sizeof(t));
    t.InProgress = true; t.EmoteForAvailable = true; t.Evil = true; t.BestWitnessesAheadToDate = 1; t.RenownPerWitness = 5;
    t.RegionConfirmationWaiting = true; t.HasInventoryItem = true;
    t.Carrier = &carrier; t.Hero = &hero;
    t.Gained[0] = &g1; t.Gained[1] = &g2; t.Gained[2] = &g3; t.NumGained = 3;
    t.Available[0] = &a1; t.NumAvailable = 1;
    TerminateMinigame(t);
    ok = ok && !t.InProgress && carrier.Finished == 1 && carrier.Sheathes == 1 && !t.ReturnedToInventory;
    ok = ok && g1.StateGroupRoot == 1 && g1.PersonalityCleared == 1 && g2.StateGroupRoot == 0 && g3.StateGroupRoot == 0;
    ok = ok && a1.StateGroupRoot == 1 && a1.EmoteSet == 1;
    ok = ok && carrier.BestTrophyWitnesses == 2 && carrier.Renown == 5 && carrier.Deed == DEED_ANNOYING_LARGE;
    ok = ok && t.NumTexts == 2 && t.Texts[0] == TEXT_SCORED && t.TextArgs[0][0] == 3 && t.TextArgs[0][1] == 1 && t.TextArgs[0][2] == 2;
    ok = ok && t.Texts[1] == TEXT_BEATEN && t.TextArgs[1][0] == 1 && t.TextArgs[1][1] == 5 && t.BestWitnessesAheadToDate == 2;
    ok = ok && t.WaitingForScore && t.HighScoreSounds == 1 && t.NoScoreSounds == 0 && t.MiniGameInfoOff == 1 && !t.RegionConfirmationWaiting;

    // 2. no carrier: the trophy returns to the inventory, the hero holds it; fewer gained than available scores 0 (no new best)
    Witness b1 = MakeWitness(true, false), b2 = MakeWitness(true, false);
    Thing hero2; std::memset(&hero2, 0, sizeof(hero2)); hero2.BestTrophyWitnesses = 4;
    Trophy u; std::memset(&u, 0, sizeof(u));
    u.InProgress = true; u.HasInventoryItem = true; u.BestWitnessesAheadToDate = 3; u.Hero = &hero2;
    u.Available[0] = &b1; u.Available[1] = &b2; u.NumAvailable = 2;
    TerminateMinigame(u);
    ok = ok && u.ReturnedToInventory && b1.StateGroupRoot == 1 && b1.EmoteSet == 0;
    ok = ok && hero2.BestTrophyWitnesses == 4 && hero2.Renown == 0 && u.NumTexts == 2 && u.Texts[0] == TEXT_SCORED;
    ok = ok && u.TextArgs[0][2] == 0 && u.Texts[1] == TEXT_FAILED && u.TextArgs[1][0] == 3 && u.NoScoreSounds == 1;
    ok = ok && u.BestWitnessesAheadToDate == 3;

    // 3. a creature refuses the sheathe action (and is mid-action in an uninterruptible group): the hero holds it; a dead hero
    //    gets no score dialog, but the mini-game info and the region confirmation are still cleared
    Thing stubborn; std::memset(&stubborn, 0, sizeof(stubborn));
    stubborn.Creature = true; stubborn.CurrentAction = true; stubborn.InterruptionGroup = 2;
    Thing deadHero; std::memset(&deadHero, 0, sizeof(deadHero)); deadHero.Dead = true;
    Trophy v; std::memset(&v, 0, sizeof(v));
    v.InProgress = true; v.Carrier = &stubborn; v.Hero = &deadHero; v.RegionConfirmationWaiting = true;
    TerminateMinigame(v);
    ok = ok && stubborn.Finished == 0 && stubborn.Sheathes == 1 && v.NumTexts == 0 && !v.WaitingForScore;
    ok = ok && v.MiniGameInfoOff == 1 && !v.RegionConfirmationWaiting;

    if (!ok) { std::printf("006de280_TEST FAIL\n"); return 1; }
    std::printf("006de280_TEST PASS\n");
    return 0;
}
