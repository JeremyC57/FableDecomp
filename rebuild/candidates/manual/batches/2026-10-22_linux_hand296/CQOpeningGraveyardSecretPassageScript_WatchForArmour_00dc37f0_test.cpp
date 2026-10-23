#include <cstdio>
#include <cstring>
// Behaviour test for CQ_OpeningGraveyardSecretPassageScript::WatchForArmour (0x00DC37F0): the counter climbs as each
// ceremonial piece is acquired; the objective text changes on each new piece and switches to 03B when all four are held.
struct CScriptThing { int Id; };
struct CGSI {
    int Frames; int Counter; long Value; bool Display; int Objectives; char LastObjective[64]; bool Removed; CScriptThing Hero;
    void NewScriptFrame() { ++Frames; }
    CScriptThing& GetHero() { return Hero; }
    bool IsObjectInThingsPossession(const char* object, const CScriptThing&)
    {
        if (!std::strcmp(object, "OBJECT_CEREMONIAL_SWORD")) return Frames >= 2;
        if (!std::strcmp(object, "OBJECT_CEREMONIAL_SHIELD")) return Frames >= 5;
        if (!std::strcmp(object, "OBJECT_CEREMONIAL_ARMOUR")) return Frames >= 5;
        return Frames >= 9;
    }
    void SetQuestCardObjective(const char*, const char* text, const char*, const char*) { ++Objectives; std::strcpy(LastObjective, text); }
    void DisplayQuestInfo(bool d) { Display = d; }
    long AddQuestInfoCounter(const char*, long, float) { return 77; }
    void UpdateQuestInfoCounter(long c, long v, long) { Counter = (int)c; Value = v; }
    void RemoveQuestInfoElement(long c) { Removed = c == 77; }
    const char* GetActiveQuestName() { return "Q"; }
};
struct CQ_OpeningGraveyardSecretPassageScript {
    CGSI* GSI;
    bool IsActiveThreadTerminating() { return GSI->Frames > 50; }
    void WatchForArmour()
    {
        long counter = GSI->AddQuestInfoCounter("HUD_NOSTRO_ARMOUR_ICON", 4, 1.0f);
        GSI->DisplayQuestInfo(true);
        bool haveSword = false, haveShield = false, haveArmour = false, haveHelmet = false;
        long found;
        do {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            bool changed = false;
            if (!haveSword && GSI->IsObjectInThingsPossession("OBJECT_CEREMONIAL_SWORD", GSI->GetHero())) { if (IsActiveThreadTerminating()) return; haveSword = true; changed = true; }
            if (!haveShield && GSI->IsObjectInThingsPossession("OBJECT_CEREMONIAL_SHIELD", GSI->GetHero())) { if (IsActiveThreadTerminating()) return; haveShield = true; changed = true; }
            if (!haveArmour && GSI->IsObjectInThingsPossession("OBJECT_CEREMONIAL_ARMOUR", GSI->GetHero())) { if (IsActiveThreadTerminating()) return; haveArmour = true; changed = true; }
            if (!haveHelmet && GSI->IsObjectInThingsPossession("OBJECT_CEREMONIAL_HELMET", GSI->GetHero())) { if (IsActiveThreadTerminating()) return; haveHelmet = true; changed = true; }
            if (changed) {
                if (IsActiveThreadTerminating()) return;
                GSI->SetQuestCardObjective(GSI->GetActiveQuestName(), "TEXT_QUEST_GRAVEYARD_SECRET_PASSAGE_OBJECTIVE_03A", "", "Graveyard");
            }
            found = haveSword + haveShield + haveArmour + haveHelmet;
            GSI->UpdateQuestInfoCounter(counter, found, -1);
        } while (found != 4);
        if (IsActiveThreadTerminating()) return;
        GSI->SetQuestCardObjective(GSI->GetActiveQuestName(), "TEXT_QUEST_GRAVEYARD_SECRET_PASSAGE_OBJECTIVE_03B", "", "Graveyard");
        GSI->RemoveQuestInfoElement(counter);
        GSI->DisplayQuestInfo(false);
    }
};
int main()
{
    CGSI gsi; std::memset(&gsi, 0, sizeof gsi);
    CQ_OpeningGraveyardSecretPassageScript s; s.GSI = &gsi;
    s.WatchForArmour();
    bool ok = gsi.Frames == 9 && gsi.Value == 4 && gsi.Counter == 77 && gsi.Objectives == 4 && gsi.Removed && !gsi.Display
              && std::strcmp(gsi.LastObjective, "TEXT_QUEST_GRAVEYARD_SECRET_PASSAGE_OBJECTIVE_03B") == 0;
    if (!ok) { std::printf("00dc37f0_TEST FAIL %d %ld %d\n", gsi.Frames, gsi.Value, gsi.Objectives); return 1; }
    std::printf("00dc37f0_TEST PASS\n");
    return 0;
}
