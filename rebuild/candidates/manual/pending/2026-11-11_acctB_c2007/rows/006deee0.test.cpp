// Behaviour test for 0x006DEEE0 CTCTrophy::GlobalUpdate: a host copy of the row's logic over plain models (the trophy's flags, the
// two queued messages, the carrier / owner and the effects it causes), checked arm by arm.
#include <cstdio>
#include <cstring>

enum { DEED_EVIL = 55, DEED_GOOD = 56 };
enum { DLG_NONE = 0, DLG_INSTRUCTIONS = 1, DLG_PLAY_GAME = 2 };

struct Message { bool Present; long ExtraData; bool Removed; };
struct Creature { bool IsCreature; bool HasAction; long InterruptionGroupID; bool HasDeedLog;
                  int Finished, UnsheatheActions, ShowActions; int Deeds[2]; int DeedCount; bool DeedFactionFlag[2]; };
struct Trophy {
    bool WaitingForConfirmation, WaitingForScore, WaitingForTerminate, WaitingForNextInstructionsPage, IsGlobalTC, EvilTrophy;
    long CurrentInstructionsPage, BestWitnessesAheadToDate, AvailableWitnesses;
    Creature* Carrier; Creature* Owner;
    // effects
    int SoundsUpdated, RemovedFromGlobal, MinigameStarted, Terminated, Unpaused, StartSounds;
    int Dialog; long DialogPage, DialogFlags; bool DialogDoneButton; long DialogCount1, DialogCount2;
};

// The row's logic (same order of tests and effects as 0x006DEEE0).
static void GlobalUpdate(Trophy& t, Message& answer, Message& click, bool update_sounds)
{
    if (update_sounds) t.SoundsUpdated++;
    if (!t.WaitingForConfirmation) return;
    if (answer.Present) {
        t.RemovedFromGlobal++; t.IsGlobalTC = false; t.WaitingForConfirmation = false;
        if (answer.ExtraData == 1) {
            if (t.Carrier && t.Carrier->IsCreature) {
                t.MinigameStarted++;
            } else {
                Creature* owner = t.Owner;
                if (owner && owner->IsCreature) {
                    if (owner->HasAction && owner->InterruptionGroupID <= 0) owner->Finished++;
                    owner->UnsheatheActions++;
                    t.MinigameStarted++;
                }
            }
        } else if (answer.ExtraData == 0) {
            Creature* owner = t.Owner;
            if (owner && owner->IsCreature) {
                if (owner->HasAction && owner->InterruptionGroupID <= 0) owner->Finished++;
                owner->ShowActions++;
                if (owner->HasDeedLog) {
                    owner->Deeds[owner->DeedCount] = t.EvilTrophy ? DEED_EVIL : DEED_GOOD;
                    owner->DeedFactionFlag[owner->DeedCount++] = true;
                    owner->Deeds[owner->DeedCount] = t.EvilTrophy ? DEED_GOOD : DEED_EVIL;
                    owner->DeedFactionFlag[owner->DeedCount++] = false;
                }
            }
        } else if (answer.ExtraData == 2) {
            t.Dialog = DLG_INSTRUCTIONS; t.DialogPage = t.CurrentInstructionsPage; t.DialogFlags = 10; t.DialogDoneButton = false;
            t.WaitingForNextInstructionsPage = true;
        }
        answer.Removed = true;
        return;
    }
    if (!click.Present) return;
    t.RemovedFromGlobal++; t.IsGlobalTC = false;
    click.Removed = true;
    t.WaitingForConfirmation = false;
    if (t.WaitingForTerminate) {
        t.WaitingForTerminate = false; t.Terminated++;
    } else if (t.WaitingForScore) {
        t.WaitingForScore = false;
    } else if (t.WaitingForNextInstructionsPage) {
        if (++t.CurrentInstructionsPage < 2) {
            t.Dialog = DLG_INSTRUCTIONS; t.DialogPage = t.CurrentInstructionsPage; t.DialogFlags = 10;
            t.DialogDoneButton = t.CurrentInstructionsPage + 1 == 2;
        } else {
            t.CurrentInstructionsPage = 0; t.WaitingForNextInstructionsPage = false; t.StartSounds++;
            t.Dialog = DLG_PLAY_GAME; t.DialogFlags = 4; t.DialogCount1 = t.AvailableWitnesses; t.DialogCount2 = t.BestWitnessesAheadToDate;
        }
    } else {
        t.Unpaused++;
    }
}

static void Reset(Trophy& t, Message& a, Message& c)
{
    std::memset(&t, 0, sizeof(t)); std::memset(&a, 0, sizeof(a)); std::memset(&c, 0, sizeof(c));
    t.WaitingForConfirmation = true; t.IsGlobalTC = true; t.AvailableWitnesses = 7; t.BestWitnessesAheadToDate = 3;
}

int main()
{
    Trophy t; Message a, c; Creature hero, other; bool ok = true;

    // nothing pending: only the sounds
    Reset(t, a, c); t.WaitingForConfirmation = false;
    GlobalUpdate(t, a, c, true);
    ok = ok && t.SoundsUpdated == 1 && t.RemovedFromGlobal == 0;

    // yes while carried by a creature: start at once
    Reset(t, a, c); std::memset(&hero, 0, sizeof(hero)); hero.IsCreature = true; t.Carrier = &hero;
    a.Present = true; a.ExtraData = 1;
    GlobalUpdate(t, a, c, false);
    ok = ok && t.MinigameStarted == 1 && a.Removed && !t.IsGlobalTC && !t.WaitingForConfirmation && hero.UnsheatheActions == 0;

    // yes from the inventory: the owner unsheathes it first (finishing an interruptible action), then start
    Reset(t, a, c); std::memset(&hero, 0, sizeof(hero)); hero.IsCreature = true; hero.HasAction = true; t.Owner = &hero;
    a.Present = true; a.ExtraData = 1;
    GlobalUpdate(t, a, c, false);
    ok = ok && t.MinigameStarted == 1 && hero.Finished == 1 && hero.UnsheatheActions == 1 && a.Removed;
    // ... but not when the owner is no creature
    Reset(t, a, c); std::memset(&other, 0, sizeof(other)); t.Owner = &other;
    a.Present = true; a.ExtraData = 1;
    GlobalUpdate(t, a, c, false);
    ok = ok && t.MinigameStarted == 0 && a.Removed && other.UnsheatheActions == 0;

    // no: show the trophy and post the two opposite deeds (evil trophy: evil for the witnesses, good for the bandits)
    Reset(t, a, c); std::memset(&hero, 0, sizeof(hero)); hero.IsCreature = true; hero.HasDeedLog = true;
    hero.HasAction = true; hero.InterruptionGroupID = 3; t.Owner = &hero; t.EvilTrophy = true;
    a.Present = true; a.ExtraData = 0;
    GlobalUpdate(t, a, c, false);
    ok = ok && hero.ShowActions == 1 && hero.Finished == 0 && hero.DeedCount == 2 && hero.Deeds[0] == DEED_EVIL
            && hero.Deeds[1] == DEED_GOOD && hero.DeedFactionFlag[0] && !hero.DeedFactionFlag[1] && a.Removed;

    // third answer: the instructions, page 0, then click past to page 1 (with the done button), then to the play-game text
    Reset(t, a, c);
    a.Present = true; a.ExtraData = 2;
    GlobalUpdate(t, a, c, false);
    ok = ok && t.Dialog == DLG_INSTRUCTIONS && t.DialogPage == 0 && t.DialogFlags == 10 && t.WaitingForNextInstructionsPage;
    a.Present = false; c.Present = true; t.WaitingForConfirmation = true;
    GlobalUpdate(t, a, c, false);
    ok = ok && t.Dialog == DLG_INSTRUCTIONS && t.DialogPage == 1 && t.DialogDoneButton && c.Removed;
    t.WaitingForConfirmation = true;
    GlobalUpdate(t, a, c, false);
    ok = ok && t.Dialog == DLG_PLAY_GAME && t.DialogFlags == 4 && t.DialogCount1 == 7 && t.DialogCount2 == 3
            && t.CurrentInstructionsPage == 0 && !t.WaitingForNextInstructionsPage && t.StartSounds == 1;

    // click past: terminate, then the score wait, then unpause
    Reset(t, a, c); c.Present = true; t.WaitingForTerminate = true; t.WaitingForScore = true;
    GlobalUpdate(t, a, c, false);
    ok = ok && t.Terminated == 1 && !t.WaitingForTerminate && t.WaitingForScore;
    t.WaitingForConfirmation = true;
    GlobalUpdate(t, a, c, false);
    ok = ok && !t.WaitingForScore && t.Unpaused == 0;
    t.WaitingForConfirmation = true;
    GlobalUpdate(t, a, c, false);
    ok = ok && t.Unpaused == 1 && t.RemovedFromGlobal == 3;

    if (!ok) { std::printf("006deee0_TEST FAIL\n"); return 1; }
    std::printf("006deee0_TEST PASS\n");
    return 0;
}
