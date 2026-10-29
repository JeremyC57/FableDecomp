// Behaviour test for 0x00E389E0 NScript::CV_AssassinAttacksScript::CAssassinChest::Main: wait until the hero is within the
// chest proximity, then open the chest (sound + animation), spawn the hiding assassin at the chest, give it the treasure clue
// when the hero lacks it (else a carp) and face it along the chest's angle; termination at any check returns early.
#include <stdio.h>
#include <string.h>

static char g_log[512];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
static int g_frame, g_nearFrame, g_endFrame, g_heroHasClue;
class CThing { public: const char* Name; float X, Angle; };
static CThing g_hero = { "Hero", 0.0f, 0.0f };
static CThing g_assassin = { "Assassin", 0.0f, 0.0f };
class CGame { public:
    CThing* GetHero() { return &g_hero; }
    void NewScriptFrame() { ++g_frame; }
    void PlaySoundOnThing(CThing*, const char* s) { Log(s); }
    void EntityPlayObjectAnimation(CThing*, const char* a, bool) { Log(a); }
    CThing* CreateCreature(const char* def, float x, const char* script, bool) { Log(def); Log(script); g_assassin.X = x; return &g_assassin; }
    bool IsObjectInThingsPossession(const char* obj, CThing* t) { return t == &g_hero && strcmp(obj, "OBJECT_TREASURE_CLUE_3") == 0 && g_heroHasClue; }
    void AddItemToContainer(CThing* t, const char* obj) { if (t == &g_assassin) Log(obj); }
    void EntitySetFacingAngle(CThing* t, float a, bool) { t->Angle = a; Log("Face"); }
};
struct CAssassinChest {
    CGame* Game;
    CThing Me;
    float Proximity;
    bool IsActiveThreadTerminating() { return g_frame >= g_endFrame; }
    bool IsDistanceBetweenThingsUnder(CThing*, CThing*, float d) { return g_frame >= g_nearFrame && d > 0.0f; }
    void Main()
    {
        while (!IsDistanceBetweenThingsUnder(&Me, Game->GetHero(), Proximity)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        Game->PlaySoundOnThing(&Me, "SND_CHESTOPEN");
        Game->EntityPlayObjectAnimation(&Me, "CHEST_OPEN", false);
        CThing* assassin = Game->CreateCreature("CREATURE_ASSASSIN", Me.X, "ChestHidingAssassin", false);
        bool noClue = !Game->IsObjectInThingsPossession("OBJECT_TREASURE_CLUE_3", Game->GetHero());
        if (noClue) {
            if (IsActiveThreadTerminating())
                return;
            Game->AddItemToContainer(assassin, "OBJECT_TREASURE_CLUE_3");
        } else {
            if (IsActiveThreadTerminating())
                return;
            Game->AddItemToContainer(assassin, "OBJECT_FISH_CARP_MEDIUM_01");
        }
        Game->EntitySetFacingAngle(assassin, Me.Angle, true);
    }
};

int main()
{
    CGame g; CAssassinChest c; c.Game = &g; c.Me.Name = "Chest"; c.Me.X = 12.5f; c.Me.Angle = 1.25f; c.Proximity = 5.0f;
    g_log[0] = 0; g_frame = 0; g_nearFrame = 2; g_endFrame = 100; g_heroHasClue = 0;
    c.Main();
    int ok = !strcmp(g_log, "SND_CHESTOPEN;CHEST_OPEN;CREATURE_ASSASSIN;ChestHidingAssassin;OBJECT_TREASURE_CLUE_3;Face;")
        && g_frame == 2 && g_assassin.X == 12.5f && g_assassin.Angle == 1.25f;
    g_log[0] = 0; g_frame = 0; g_heroHasClue = 1;
    c.Main();
    ok = ok && !strcmp(g_log, "SND_CHESTOPEN;CHEST_OPEN;CREATURE_ASSASSIN;ChestHidingAssassin;OBJECT_FISH_CARP_MEDIUM_01;Face;");
    g_log[0] = 0; g_frame = 0; g_nearFrame = 50; g_endFrame = 3;
    c.Main();
    ok = ok && g_log[0] == 0 && g_frame == 3;
    if (!ok) { printf("00e389e0_TEST FAIL\n"); return 1; }
    printf("00e389e0_TEST PASS\n");
    return 0;
}