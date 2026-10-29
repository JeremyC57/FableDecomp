// Behaviour test for CTCFireballSpell::CastFireball @ 0x007C89F0: level clamp, stamina cost, build-up effect kill, cast
// sound criteria, FIREBALL_<n> definition pick, shot creation arguments and the post-creation shot settings.
#include <stdio.h>
#include <string.h>
class C3DVector { public: float X, Y, Z; C3DVector() { } C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Rotate(const C3DVector&, float) { float t = X; X = -Y; Y = t; } };
static float g_frameAngle = 0;
class CRightHandedSet { public: C3DVector Up, Forward; CRightHandedSet() { } void Rotate(const C3DVector&, float angle) { g_frameAngle = angle; } };
static char g_criteria[64]; static int g_stopped = 0; static unsigned long g_stoppedSound = 0;
class CTCSoundPlayer { public: void StopAttachedSound(unsigned long s) { g_stopped++; g_stoppedSound = s; }
    void PlayAttachedCriteria(const char* c, bool a, bool b, bool d, long e) { if (a && !b && !d && e == -1) strcpy(g_criteria, c); } };
static long g_stamina = 0;
class CTCHeroStats { public: void ModifyStaminaBy(long amount) { g_stamina += amount; } };
static char g_defName[32];
class CGameDefinitionManager { public: long GetDefGlobalIndexFromName(const char* n) const { strcpy(g_defName, n); return 100 + n[9] - '0'; } };
static CGameDefinitionManager g_manager;
class CTCPhysicsBase { public: C3DVector Facing; CRightHandedSet Last;
    C3DVector GetFacingVector() const { return Facing; } const CRightHandedSet& GetLastRHSet() const { return Last; } };
static char g_dummy[32];
struct CTCGraphicAppearance { bool GetDummyObject(const char* name, long, unsigned long, C3DVector& p, CRightHandedSet&, float) const
    { strcpy(g_dummy, name); p = C3DVector(1, 2, 3); return true; } };
struct CTCShotDefinitionBase { bool CanBeDeflected; void SetCanBeDeflected(bool v) { CanBeDeflected = v; } };
class CThing { public: bool Dead; bool Drawable; bool IsAffectedByBulletTimeOverride; int Kills;
    CTCHeroStats* Stats; CTCSoundPlayer* Sound; CTCShotDefinitionBase* Shot; CTCPhysicsBase* PhysicsTC; CTCGraphicAppearance* PTCGraphicAppearance;
    CThing(CTCHeroStats* st, CTCSoundPlayer* so, CTCShotDefinitionBase* sh, CTCPhysicsBase* ph, CTCGraphicAppearance* ap)
        : Dead(false), Drawable(true), IsAffectedByBulletTimeOverride(true), Kills(0), Stats(st), Sound(so), Shot(sh), PhysicsTC(ph), PTCGraphicAppearance(ap) { }
    virtual ~CThing() { }
    bool IsDead() const { return Dead; } void Kill(bool) { Kills++; } virtual void SetAsDrawable(bool d) { Drawable = d; }
    const CGameDefinitionManager& PeekDefinitionManager() const { return g_manager; }
    bool GetTC(CTCHeroStats** o) { *o = Stats; return Stats != 0; } bool GetTC(CTCSoundPlayer** o) { *o = Sound; return Sound != 0; }
    bool GetTC(CTCShotDefinitionBase** o) { *o = Shot; return Shot != 0; } };
static CThing* g_newShot = 0; static long g_shotIndex = -1; static const CThing* g_target = 0; static float g_acc = 0, g_a = 0, g_b = 0;
static CThing* CreateShot(long index, CThing*, CThing* origin, const CThing* target, float acc, float a, float b, const C3DVector* pos, const CRightHandedSet*)
{ g_shotIndex = index; g_target = target; g_acc = acc; g_a = a; g_b = b; if (origin || !pos || pos->Y != 2) return 0; return g_newShot; }
struct CIntelligentPointer { CThing* P; CThing* Get() const { return P; } CThing* operator->() const { return P; } };
static bool IS_THING_ALIVE(const CIntelligentPointer& p) { if (p.Get() != 0) { if (!p->IsDead()) return true; } return false; }
struct CDef { float Inclination; float StaminaCost[4]; };
class CTCFireballSpell { public: CThing* ParentThing; long WillAccumulatedLevel; const CDef* PDef; unsigned long InitLoopingSound;
    CIntelligentPointer FireballBuildUpEffect; const char* FireballAttachmentName; long MaxLevelAllowableByStamina;
    void CastFireball(CThing* target)
    {
        long level = WillAccumulatedLevel;
        if (level < 0) level = 0; else if (level > MaxLevelAllowableByStamina) level = MaxLevelAllowableByStamina;
        CTCHeroStats* heroStats;
        if (ParentThing->GetTC(&heroStats)) heroStats->ModifyStaminaBy((long)-PDef->StaminaCost[level]);
        if (IS_THING_ALIVE(FireballBuildUpEffect)) FireballBuildUpEffect->Kill(true);
        CTCSoundPlayer* soundPlayer;
        if (ParentThing->GetTC(&soundPlayer)) {
            soundPlayer->StopAttachedSound(InitLoopingSound);
            switch (level) {
            case 0: soundPlayer->PlayAttachedCriteria("HERO_ABILITY_FIREBALL_CAST_NO_POWERUP", true, false, false, -1); break;
            case 1: soundPlayer->PlayAttachedCriteria("HERO_ABILITY_FIREBALL_CAST_LEVEL_1", true, false, false, -1); break;
            case 2: soundPlayer->PlayAttachedCriteria("HERO_ABILITY_FIREBALL_CAST_LEVEL_2", true, false, false, -1); break;
            case 3: soundPlayer->PlayAttachedCriteria("HERO_ABILITY_FIREBALL_CAST_LEVEL_3", true, false, false, -1); break;
            }
        }
        CThing* parent = ParentThing;
        C3DVector position; CRightHandedSet frame;
        parent->PTCGraphicAppearance->GetDummyObject(FireballAttachmentName, 0, 0, position, frame, 1.0f);
        long shotIndex = 0;
        switch (level) {
        case 0: shotIndex = parent->PeekDefinitionManager().GetDefGlobalIndexFromName("FIREBALL_1"); break;
        case 1: shotIndex = parent->PeekDefinitionManager().GetDefGlobalIndexFromName("FIREBALL_2"); break;
        case 2: shotIndex = parent->PeekDefinitionManager().GetDefGlobalIndexFromName("FIREBALL_3"); break;
        case 3: shotIndex = parent->PeekDefinitionManager().GetDefGlobalIndexFromName("FIREBALL_4"); break;
        }
        C3DVector side = parent->PhysicsTC->GetFacingVector();
        side.Rotate(C3DVector(0.0f, 0.0f, 1.0f), 0.25f);
        frame = parent->PhysicsTC->GetLastRHSet();
        frame.Rotate(C3DVector(side), PDef->Inclination / 360.0f);
        CThing* shot = CreateShot(shotIndex, parent, 0, target, 100.0f, 1.0f, 0.0f, &position, &frame);
        if (shot && !shot->IsDead()) {
            shot->SetAsDrawable(false);
            shot->IsAffectedByBulletTimeOverride = false;
            CTCShotDefinitionBase* shotDefinition;
            if (shot->GetTC(&shotDefinition)) shotDefinition->SetCanBeDeflected(false);
        }
    } };
static int fail(int line) { printf("007c89f0_TEST FAIL\n"); printf("line %d\n", line); return 1; }
int main()
{
    CTCHeroStats stats; CTCSoundPlayer sound; CTCPhysicsBase phys; phys.Facing = C3DVector(1, 0, 0); CTCGraphicAppearance app;
    CThing hero(&stats, &sound, 0, &phys, &app);
    CThing effect(0, 0, 0, 0, 0);
    CTCShotDefinitionBase shotTC = { true };
    CThing shot(0, 0, &shotTC, 0, 0);
    CThing target(0, 0, 0, 0, 0);
    CDef def = { 90.0f, { 10.0f, 20.5f, 30.0f, 40.0f } };
    CTCFireballSpell f; f.ParentThing = &hero; f.PDef = &def; f.InitLoopingSound = 77; f.FireballBuildUpEffect.P = &effect;
    f.FireballAttachmentName = "FIREBALL_HAND"; f.MaxLevelAllowableByStamina = 2;
    g_newShot = &shot;
    f.WillAccumulatedLevel = 5; f.CastFireball(&target);
    if (g_stamina != -30 || effect.Kills != 1 || g_stopped != 1 || g_stoppedSound != 77) return fail(__LINE__);
    if (strcmp(g_criteria, "HERO_ABILITY_FIREBALL_CAST_LEVEL_2") != 0 || strcmp(g_defName, "FIREBALL_3") != 0 || g_shotIndex != 103) return fail(__LINE__);
    if (strcmp(g_dummy, "FIREBALL_HAND") != 0 || g_target != &target || g_acc != 100.0f || g_a != 1.0f || g_b != 0.0f || g_frameAngle != 0.25f) return fail(__LINE__);
    if (shot.Drawable || shot.IsAffectedByBulletTimeOverride || shotTC.CanBeDeflected) return fail(__LINE__);
    effect.Dead = true; g_stamina = 0; shot.Drawable = true; shot.Shot = 0;
    f.WillAccumulatedLevel = -3; f.CastFireball(&target);
    if (g_stamina != -10 || effect.Kills != 1 || strcmp(g_criteria, "HERO_ABILITY_FIREBALL_CAST_NO_POWERUP") != 0 || g_shotIndex != 101 || shot.Drawable) return fail(__LINE__);
    f.WillAccumulatedLevel = 1; f.CastFireball(&target);
    if (g_stamina != -30 || g_shotIndex != 102) return fail(__LINE__);   // (long)-20.5f truncates to -20
    shot.Dead = true; shot.Drawable = true; hero.Stats = 0; hero.Sound = 0; g_stopped = 0;
    f.WillAccumulatedLevel = 2; f.CastFireball(&target);
    if (!shot.Drawable || g_stopped != 0 || g_stamina != -30 || g_shotIndex != 103) return fail(__LINE__);
    printf("007c89f0_TEST PASS\n");
    return 0;
}