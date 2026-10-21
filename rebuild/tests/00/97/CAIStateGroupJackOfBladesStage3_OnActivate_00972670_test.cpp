// Behaviour test for CAIStateGroup_JackOfBladesStage3::OnActivate (0x00972670): model copy, scripted stubs.
#include <cstdio>
#include <cstring>

struct CCharString { const char* text; CCharString(const char* t) : text(t) { } };
struct CAttackStyleDef { int refs; int destroyed; const char* name; void Release() { if (--refs == 0) ++destroyed; } };
struct CDefPointer {
    CAttackStyleDef* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p) { m_p->Release(); m_p = 0; } }
};
static CAttackStyleDef g_def = { 0, 0, "HERO_JACK_OF_BLADES_ATTACK_STYLE_LAST_STAND" };
struct CDefinitionManager {
    bool GetDef(const CCharString& name, CDefPointer* def) { if (std::strcmp(name.text, g_def.name)) return false; ++g_def.refs; def->m_p = &g_def; return true; }
};
struct CTCWillPower { bool Active; };
static int g_styleSet; static CAttackStyleDef* g_style;
struct CTCCombat { void SetAttackStyleDef(const CDefPointer* def) { ++g_styleSet; g_style = def->m_p; if (g_style) ++g_style->refs; } };
struct CHitLocation { int enabled; void SetEnabled(bool e) { enabled = e ? 1 : 0; } };
static CHitLocation g_big = { 1 }, g_force = { 1 }, g_body = { 0 };
struct CTCHitLocations {
    CHitLocation* GetPHitLocationFromName(const CCharString& name)
    {
        if (!std::strcmp(name.text, "HIT_LOCATION_JACK_OF_BLADES_BIG")) return &g_big;
        if (!std::strcmp(name.text, "HIT_LOCATION_JACK_OF_BLADES_FORCEFIELD")) return &g_force;
        if (!std::strcmp(name.text, "HIT_LOCATION_JACK_OF_BLADES")) return &g_body;
        return 0;
    }
};
struct CThing {
    CTCWillPower* will; CTCCombat* combat; CTCHitLocations* hit;
    bool GetInterface(CTCWillPower** o, long) { if (!will) return false; *o = will; return true; }
    CTCCombat* GetCombat() { return combat; }
    CTCHitLocations* GetHitLocations() { return hit; }
};
struct CAIBrain { CThing* PCreature; };
static CDefinitionManager g_defs;
struct CAIStateGroup_JackOfBladesStage3 {
    CAIBrain* PBrain;
    CThing* GetCreature() const { return PBrain->PCreature; }
    CDefinitionManager* GetPDefinitionManager() { return &g_defs; }
    void OnActivate();
};

void CAIStateGroup_JackOfBladesStage3::OnActivate()
{
    CTCWillPower* will;
    if (GetCreature()->GetInterface(&will, 0xce))
        will->Active = false;
    CTCCombat* combat = GetCreature()->GetCombat();
    CDefPointer def;
    {
        CCharString name("HERO_JACK_OF_BLADES_ATTACK_STYLE_LAST_STAND");
        GetPDefinitionManager()->GetDef(name, &def);
    }
    combat->SetAttackStyleDef(&def);
    CTCHitLocations* hitLocations = GetCreature()->GetHitLocations();
    CHitLocation* big = hitLocations->GetPHitLocationFromName("HIT_LOCATION_JACK_OF_BLADES_BIG");
    big->SetEnabled(false);
    CHitLocation* forcefield = hitLocations->GetPHitLocationFromName("HIT_LOCATION_JACK_OF_BLADES_FORCEFIELD");
    forcefield->SetEnabled(false);
    CHitLocation* body = hitLocations->GetPHitLocationFromName("HIT_LOCATION_JACK_OF_BLADES");
    body->SetEnabled(true);
}

int main()
{
    int ok = 1;
    CTCWillPower will = { true };
    CTCCombat combat; CTCHitLocations hit;
    CThing jack = { &will, &combat, &hit };
    CAIBrain brain = { &jack };
    CAIStateGroup_JackOfBladesStage3 s; s.PBrain = &brain;
    s.OnActivate();
    if (will.Active || g_styleSet != 1 || g_style != &g_def) ok = 0;
    if (g_def.refs != 1 || g_def.destroyed) ok = 0;   // the combat component keeps its reference, the local one is released
    if (g_big.enabled || g_force.enabled || !g_body.enabled) ok = 0;
    // without a will component the flag is left alone and the rest still runs
    jack.will = 0; will.Active = true; g_big.enabled = 1;
    s.OnActivate();
    if (!will.Active || g_styleSet != 2 || g_big.enabled) ok = 0;
    if (!ok) { std::printf("00972670_TEST FAIL\n"); return 1; }
    std::printf("00972670_TEST PASS\n");
    return 0;
}