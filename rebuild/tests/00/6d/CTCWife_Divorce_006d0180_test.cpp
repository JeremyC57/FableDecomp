// 006d0180 behaviour test: CTCWife::Divorce (control flow + data of the final.cpp body)
#include <stdio.h>
#include <string.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
class CThing {
public:
    void* Iface[0x60];
    unsigned char Flags91;
    unsigned char Flags1C9;
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    { if (Iface[interfaceType]) { *ppOut = (T*)Iface[interfaceType]; return true; } return false; }
};
struct CCharString { char Text[48]; CCharString(const char* t, long = -1) { strcpy(Text, t); } ~CCharString() {} };
static long g_worldFrame = 100;
struct CWorld { static long GetWorldFrame() { return g_worldFrame; } };
struct CPlayer { CThing* Hero; CThing* GetHeroThing() const { return Hero; } };
struct CPlayerManager { CPlayer* Main; CPlayer* GetMainPlayer() { return Main; } };
static CWorld g_world;
struct CTCCreatureOpinionOfHero {
    long LastOpinionReactionFrame;
    bool HeroOpinionEnemy;
    char Reaction[48], Deed[48], Personality[48]; bool Force; int Cleared;
    float Offsets[4]; long Frames[4]; float Decay[4]; int NumOffsets;
    CWorld* GetWorld() { return &g_world; }
    void SetOpinionReactionMask(const CCharString& n) { strcpy(Reaction, n.Text); }
    void SetOpinionDeedMask(const CCharString& n) { strcpy(Deed, n.Text); }
    void SetPersonalityOverride(const CCharString& n, bool f) { strcpy(Personality, n.Text); Force = f; }
    void ClearAllKnownDeeds() { ++Cleared; }
    void ApplyOpinionTransientOffset(long type, float amount, long, long frames, float decay, long)
    { Offsets[type] = amount; Frames[type] = frames; Decay[type] = decay; ++NumOffsets; }
};
struct CTCHeroStats {
    long Morality; long Max; CThing* Spouse; long MarriageState;
    void SetMarriageState(CThing* spouse, long state) { Spouse = spouse; MarriageState = state; }
    long GetMoralityMax() { return Max; }
    void SetMorality(long m) { Morality = m; }
};
struct CHomeOccupant { CThing* Thing; };
struct CTCHome { CHomeOccupant* Occ; CHomeOccupant* GetOccupant() { return Occ; } };
static int g_refreshed = 0;
struct CPlayerGui { void Refresh(long) { ++g_refreshed; } };
CPlayerGui* g_pPlayerGui;
long g_FrameRate = 30;
struct CHeroMarriageDef { float HeroStatMoralityInOpinionFormChangePerDivorce; };
static int g_unmarried = 0;
struct CTCWife {
    CThing* ParentThing;
    CHeroMarriageDef* PHeroMarriageDef;
    long FrameGotMarriedToThePlayer;
    bool DivorcedHero;
    float LoveAttitudeValue;
    CPlayerManager* PM;
    CPlayerManager* GetPlayerManager() { return PM; }
    void Unmarry(bool notify) { g_unmarried = notify ? 2 : 1; }
    void Divorce();
};
void CTCWife::Divorce()
{
    if (FrameGotMarriedToThePlayer > CWorld::GetWorldFrame())
        return;
    Unmarry(false);
    DivorcedHero = true;
    CTCCreatureOpinionOfHero* opinion;
    if (ParentThing->GetInterface(&opinion, 0x4f)) {
        {
            CCharString mask("OPINION_REACTION_MASK_WIFE_DIVORCED");
            opinion->SetOpinionReactionMask(mask);
        }
        {
            CCharString mask("OPINION_DEED_MASK_WIFE_DIVORCED");
            opinion->SetOpinionDeedMask(mask);
        }
        {
            CCharString personality("OPINION_PERSONALITY_WIFE_DIVORCED");
            opinion->SetPersonalityOverride(personality, true);
        }
        opinion->LastOpinionReactionFrame = opinion->GetWorld()->GetWorldFrame();
        opinion->ClearAllKnownDeeds();
        opinion->HeroOpinionEnemy = false;
    }
    ParentThing->Flags1C9 &= ~8;
    LoveAttitudeValue = 0.0f;
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
    if (hero && !(hero->Flags91 & 1)) {
        CTCHeroStats* stats;
        hero->GetInterface(&stats, 4);
        stats->SetMarriageState(ParentThing, 5);
        long morality = stats->Morality;
        const CHeroMarriageDef* def = PHeroMarriageDef;
        stats->SetMorality((long)(stats->GetMoralityMax() * def->HeroStatMoralityInOpinionFormChangePerDivorce + morality));
    }
    CTCHome* home;
    if (ParentThing->GetInterface(&home, 0x23)) {
        CHomeOccupant* occupant = home->GetOccupant();
        if (occupant) {
            CTCCreatureOpinionOfHero* other;
            if (occupant->Thing->GetInterface(&other, 0x4f)) {
                other->ApplyOpinionTransientOffset(0, -1.0f, 0, g_FrameRate * 300, 0.0f, 0);
                other->ApplyOpinionTransientOffset(3, -2.0f, 0, g_FrameRate * 300, -0.5f, 0);
            }
        }
    }
    g_pPlayerGui->Refresh(0);
}
int main()
{
    CPlayerGui gui; g_pPlayerGui = &gui;
    static CThing wifeThing, hero, other; memset(&wifeThing, 0, sizeof wifeThing); memset(&hero, 0, sizeof hero); memset(&other, 0, sizeof other);
    static CTCCreatureOpinionOfHero op, op2; memset(&op, 0, sizeof op); memset(&op2, 0, sizeof op2);
    CTCHeroStats stats = { 50, 1000, 0, 0 };
    CHomeOccupant occ = { &other }; CTCHome home = { &occ };
    wifeThing.Iface[0x4f] = &op; wifeThing.Iface[0x23] = &home; wifeThing.Flags1C9 = 0xff;
    hero.Iface[4] = &stats; other.Iface[0x4f] = &op2;
    CPlayer player = { &hero }; CPlayerManager pm = { &player };
    CHeroMarriageDef def = { -0.25f };
    CTCWife w; w.ParentThing = &wifeThing; w.PHeroMarriageDef = &def; w.DivorcedHero = false; w.LoveAttitudeValue = 3.0f; w.PM = &pm;

    // married in the future: nothing happens
    w.FrameGotMarriedToThePlayer = 101;
    w.Divorce();
    CHECK(g_unmarried == 0 && !w.DivorcedHero && g_refreshed == 0);

    w.FrameGotMarriedToThePlayer = 100; op.HeroOpinionEnemy = true;
    w.Divorce();
    CHECK(g_unmarried == 1 && w.DivorcedHero && w.LoveAttitudeValue == 0.0f && wifeThing.Flags1C9 == 0xf7);
    CHECK(strcmp(op.Reaction, "OPINION_REACTION_MASK_WIFE_DIVORCED") == 0 && strcmp(op.Deed, "OPINION_DEED_MASK_WIFE_DIVORCED") == 0);
    CHECK(strcmp(op.Personality, "OPINION_PERSONALITY_WIFE_DIVORCED") == 0 && op.Force && op.Cleared == 1);
    CHECK(op.LastOpinionReactionFrame == 100 && !op.HeroOpinionEnemy);
    CHECK(stats.Spouse == &wifeThing && stats.MarriageState == 5 && stats.Morality == -200);
    CHECK(op2.NumOffsets == 2 && op2.Offsets[0] == -1.0f && op2.Offsets[3] == -2.0f && op2.Frames[3] == 9000 && op2.Decay[3] == -0.5f);
    CHECK(g_refreshed == 1);

    // no opinion, dead hero, no occupant
    wifeThing.Iface[0x4f] = 0; hero.Flags91 = 1; occ.Thing = 0; home.Occ = 0; stats.Morality = 7; op2.NumOffsets = 0;
    w.Divorce();
    CHECK(stats.Morality == 7 && op2.NumOffsets == 0 && g_refreshed == 2);
    // no hero, no home
    player.Hero = 0; wifeThing.Iface[0x23] = 0;
    w.Divorce();
    CHECK(g_refreshed == 3);

    if (!g_fail) printf("006d0180_TEST PASS\n");
    return g_fail;
}