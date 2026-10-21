// Behaviour test for NCreatureMode::CModeScorpionKingVulnerable::Recover (0x008DF3A0): when the tail plunge is
// accepted the owner's queued anims are cleared and mode 0x37 removed; when refused the belly hit location is
// disabled and the body one enabled.
#include <cstdio>
#include <string>
#include <map>

struct CHitLocation { bool Enabled; };
struct CCreature {
    bool Accept; std::string LastAction; int Cleared; int RemovedMode;
    std::map<std::string, CHitLocation> Hits;
    bool SetCurrentAction(const std::string& a) { LastAction = a; return Accept; }
};
struct CMode {
    CCreature* ParentThing; CCreature* PCreature; CCreature* PTarget;
    void Recover()
    {
        if (PCreature->SetCurrentAction(PTarget ? "TAIL_PLUNGE" : "TAIL_PLUNGE_NOTARGET")) {
            ++ParentThing->Cleared;
            PCreature->RemovedMode = 0x37;
        } else {
            CHitLocation* belly = &PCreature->Hits["HIT_LOCATION_SCORPION_KING_BELLY"];
            CHitLocation* body = &PCreature->Hits["HIT_LOCATION_SCORPION_KING"];
            belly->Enabled = false;
            body->Enabled = true;
        }
    }
};

int main()
{
    CCreature king; king.Accept = true; king.Cleared = 0; king.RemovedMode = 0;
    king.Hits["HIT_LOCATION_SCORPION_KING_BELLY"].Enabled = true; king.Hits["HIT_LOCATION_SCORPION_KING"].Enabled = false;
    CCreature hero; hero.Accept = false;
    CMode m = { &king, &king, &hero };
    m.Recover();
    bool ok = king.LastAction == "TAIL_PLUNGE" && king.Cleared == 1 && king.RemovedMode == 0x37
        && king.Hits["HIT_LOCATION_SCORPION_KING_BELLY"].Enabled;
    king.Accept = false; king.RemovedMode = 0;
    m.Recover();
    ok = ok && king.Cleared == 1 && king.RemovedMode == 0 && !king.Hits["HIT_LOCATION_SCORPION_KING_BELLY"].Enabled
        && king.Hits["HIT_LOCATION_SCORPION_KING"].Enabled;
    if (!ok) { std::printf("008df3a0_TEST FAIL\n"); return 1; }
    std::printf("008df3a0_TEST PASS\n");
    return 0;
}
