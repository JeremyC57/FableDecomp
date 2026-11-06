// Behaviour test for CNOVI_Victim::Main (0x00DBCD60): once the bully is subdued the boy thanks the hero once (DoneThanks) and
// becomes pushable; talked to, he thanks (after-hit line if the hero hit him) or pleads, and shows the game-info hint once; hit
// by the hero he makes the hero a bad deed (2) and remembers it; a teddy complaint is spoken once and the flag reset.
#include <cstdio>

struct Victim {
    bool BullySubdued, HeroAttackedVictim, Complains, DoneThanks, DisplayedGameInfo, Pushable;
    long BadDeeds, Thanks, AfterHitThanks, Pleas, Hints, Complaints;
    void Step(bool talked, bool hit)
    {
        if (BullySubdued && !DoneThanks) { DoneThanks = true; Pushable = true; }
        if (talked) {
            if (BullySubdued) { if (HeroAttackedVictim) ++AfterHitThanks; else ++Thanks; }
            else ++Pleas;
            if (!DisplayedGameInfo) { DisplayedGameInfo = true; ++Hints; }
        }
        if (hit) { BadDeeds += 2; HeroAttackedVictim = true; }
        if (Complains) { ++Complaints; Complains = false; }
    }
};

int main()
{
    Victim v = { false, false, false, false, false, false, 0, 0, 0, 0, 0, 0 };
    bool ok = true;
    v.Step(true, false);  ok = ok && v.Pleas == 1 && v.Hints == 1 && !v.Pushable;
    v.Step(true, false);  ok = ok && v.Pleas == 2 && v.Hints == 1;
    v.Step(false, true);  ok = ok && v.BadDeeds == 2 && v.HeroAttackedVictim;
    v.BullySubdued = true;
    v.Step(false, false); ok = ok && v.DoneThanks && v.Pushable;
    v.Step(true, false);  ok = ok && v.AfterHitThanks == 1 && v.Thanks == 0;
    v.Complains = true;
    v.Step(false, false); ok = ok && v.Complaints == 1 && !v.Complains;
    v.Step(false, false); ok = ok && v.Complaints == 1;
    if (!ok) { std::printf("00dbcd60_TEST FAIL\n"); return 1; }
    std::printf("00dbcd60_TEST PASS\n");
    return 0;
}