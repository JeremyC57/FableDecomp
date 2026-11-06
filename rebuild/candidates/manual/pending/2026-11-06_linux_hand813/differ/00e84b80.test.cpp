// Behaviour test for CFisticuffsMember::Main (0x00E84B80): the FC_Status state machine of a punch-club member -- reach the spot,
// cheer (pre-fight end on a result), join the queue as fighter one or two, fight (retire under the minimum health, re-queue the
// winner), wait for the challenge, pre-fight end, and leave; the club closing sends everyone to LEAVING.
#include <cstdio>

enum { FC_STATUS_REACHING_FC = 0, FC_STATUS_LEAVING_FC = 1, FC_STATUS_FIGHTING = 2, FC_STATUS_CHEERING = 3, FC_STATUS_WAITING = 4,
       FC_STATUS_PRE_FIGHT_END = 5 };

struct Club {
    bool HeroInFisticuffsClub, HeroBeatTyler, FighterOneInRing, FighterTwoInRing, ChallengeAccepted, FightFinished, HeroPaidEntryFee,
         FisticuffsActive, PlayerLost;
    long Fighter1Timer, Fighter2Timer, Challenges;
};

struct Member {
    Club* P; long Status; bool Queued, Killed; float Health;
    void Step(float distToSpot, bool roll, float minHealth, bool isFighter)
    {
        switch (Status) {
        case FC_STATUS_REACHING_FC:
            if (distToSpot <= 1.0f) Status = FC_STATUS_CHEERING;
            break;
        case FC_STATUS_CHEERING:
            if (P->HeroBeatTyler || P->PlayerLost) Status = FC_STATUS_PRE_FIGHT_END;
            if (!P->FighterOneInRing && !P->HeroPaidEntryFee && P->Fighter1Timer <= 0 && roll) {
                if (!P->FighterTwoInRing) { Queued = true; P->FighterOneInRing = true; P->Fighter2Timer = 5; Status = FC_STATUS_WAITING; }
                else { P->FighterOneInRing = true; P->ChallengeAccepted = true; P->FightFinished = false; Status = FC_STATUS_FIGHTING; }
            }
            if (!P->FighterTwoInRing && P->FighterOneInRing && P->Fighter2Timer <= 0 && roll) {
                P->FighterTwoInRing = true; P->ChallengeAccepted = true; P->FightFinished = false; Status = FC_STATUS_FIGHTING;
            }
            break;
        case FC_STATUS_FIGHTING:
            if (P->HeroPaidEntryFee) Status = FC_STATUS_CHEERING;
            if (Health < minHealth) { Health += 15.0f; Status = FC_STATUS_CHEERING; }
            else if (P->FightFinished && isFighter) {
                Queued = true; P->FighterOneInRing = true; P->Fighter2Timer = 5; P->FighterTwoInRing = false; P->ChallengeAccepted = false;
                Status = FC_STATUS_WAITING;
            }
            break;
        case FC_STATUS_WAITING:
            if (P->HeroPaidEntryFee) { Queued = false; Status = FC_STATUS_CHEERING; }
            else if (Queued && P->ChallengeAccepted && P->FisticuffsActive) { P->FightFinished = false; ++P->Challenges; Status = FC_STATUS_FIGHTING; }
            break;
        case FC_STATUS_PRE_FIGHT_END:
            Status = FC_STATUS_LEAVING_FC;
            break;
        case FC_STATUS_LEAVING_FC:
            Killed = true;
            return;
        }
        if (!P->FisticuffsActive && !P->HeroInFisticuffsClub) Status = FC_STATUS_LEAVING_FC;
    }
};

int main()
{
    Club c = { true, false, false, false, false, false, false, true, false, 0, 0, 0 };
    Member m = { &c, FC_STATUS_REACHING_FC, false, false, 50.0f };
    bool ok = true;
    m.Step(4.0f, false, 20.0f, false);
    ok = ok && m.Status == FC_STATUS_REACHING_FC;
    m.Step(0.5f, false, 20.0f, false);
    ok = ok && m.Status == FC_STATUS_CHEERING;
    m.Step(0.5f, true, 20.0f, false);
    ok = ok && m.Status == FC_STATUS_WAITING && m.Queued && c.FighterOneInRing && c.Fighter2Timer == 5;
    m.Step(0.5f, false, 20.0f, false);
    ok = ok && m.Status == FC_STATUS_WAITING;
    c.ChallengeAccepted = true;
    m.Step(0.5f, false, 20.0f, false);
    ok = ok && m.Status == FC_STATUS_FIGHTING && c.Challenges == 1;
    m.Health = 10.0f;
    m.Step(0.5f, false, 20.0f, true);
    ok = ok && m.Status == FC_STATUS_CHEERING && m.Health == 25.0f;
    c.HeroBeatTyler = true; c.FighterOneInRing = true; c.FighterTwoInRing = true;
    m.Step(0.5f, false, 20.0f, false);
    ok = ok && m.Status == FC_STATUS_PRE_FIGHT_END;
    m.Step(0.5f, false, 20.0f, false);
    ok = ok && m.Status == FC_STATUS_LEAVING_FC;
    m.Step(0.5f, false, 20.0f, false);
    ok = ok && m.Killed;
    Member n = { &c, FC_STATUS_CHEERING, false, false, 50.0f };
    c.HeroBeatTyler = false; c.FisticuffsActive = false; c.HeroInFisticuffsClub = false;
    n.Step(0.5f, false, 20.0f, false);
    ok = ok && n.Status == FC_STATUS_LEAVING_FC;
    if (!ok) { std::printf("00e84b80_TEST FAIL\n"); return 1; }
    std::printf("00e84b80_TEST PASS\n");
    return 0;
}
