// CTCHeroStats::ApplyBonusItemEffects(const CBonusItemDef&) -- applies a bonus item: excess healing turns into fatness, health over time or at once, stamina, will/skill/strength experience, morality, the three appearance stats, max health / max stamina (capped), the moonfish / golden-carrot time-of-day change, the speed multiplier and its duration, a particle effect on the hero, and drink
// Leaf: ApplyBonusItemEffects
// Retail: Fable.exe @ 0x0057C4F1, 1190 bytes, `leave ; ret 4`: frame-pointer size TU (`optimize("y", off)` + `optimize("s", on)`
// as the landed Drink 0x0057A82A / ProcessDrunkenness 0x0057CB7E).
// Callees: GetDefinitions 0x006869D0, SetFatness 0x0057B169, the thing's slot 59 (+0xEC, health change), the health-over-time
// call 0x006A6290, __ftol2 0x00BFEA70, ModifyStaminaBy 0x0057B1DB, std::bitset::test 0x00410DE0, LowerBound 0x0040F020,
// CTCHeroExperience::AddTrainableStatExperience 0x006D8300, SetMorality 0x0057C2E6, GetMaxExtraMaxHealth 0x0057A90B,
// SetMaxHealth 0x006A5260, GetMaxExtraMaxStamina 0x0057A962, CTCSound PlaySound 0x0066BD00, GetWorld 0x00686A70,
// CGameTimeManager IsNightTime 0x006BB210 / IsDay 0x006BB2C0 / SetTime 0x006BB810, 0x005812AE, CThing::GetPosition 0x004C73D0,
// Create 0x006E0880 (__fastcall), IS_THING_ALIVE 0x0040DCE0, GetTCDriver 0x004ACB50, AttachToThing 0x006E0BE0, Drink 0x0057A82A.
// Grade: DIFFER(1169v1192) -- first draft; this/def registers swapped (esi/edi), fabs compares without the fld st(0) copy; lane gap: the speed-duration round-to-int is retail's inline fistp helper (0.5 / 1.0 slots).
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1); // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
};
extern long g_framesPerSecond;                       // 0x01375550
extern "C" double __cdecl fabs(double x);
extern "C" int __cdecl abs(int x);
#pragma intrinsic(fabs, abs)
struct C3DVector { float x, y, z; };
struct CTCBase;
struct CBitset {
    bool test(unsigned long position) const;         // 0x00410DE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
};
struct CThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058();
    virtual void ChangeHealth(float amount, bool flag);   // slot 59 (+0xEC)
    unsigned char _pad04[0x1c];
    CBitset       m_present;                         // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap m_interfaces;                      // +0x44
    unsigned char _pad4c[0x64];
    float         MaxHealth;                         // +0xb0
    float         Health;                            // +0xb4 (descriptive)
    bool HasTC(long type) const { return m_present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* found = map->last;
        if (it != found && !(interfaceType < it->type))
            found = it;
        return found->component;
    }
    const C3DVector& GetPosition() const;            // 0x004C73D0
    void AddHealthOverTime(float amount, long frames);   // 0x006A6290
    void SetMaxHealth(float value, bool flag);       // 0x006A5260
    bool GetTCDriver(CTCBase** out);                 // 0x004ACB50
};
CThing* __fastcall CreateThing(long def, const C3DVector& pos, bool flag);   // 0x006E0880
bool __fastcall IS_THING_ALIVE(CThing** thing);      // 0x0040DCE0
struct CTCHeroExperience {
    void AddTrainableStatExperience(long category, long amount);   // 0x006D8300
};
struct CTCSound {
    void PlaySound(const CCharString& name, bool a, long b, long c, long d);   // 0x0066BD00
};
struct CTCSpeedModifier {
    void Apply(struct CTCHeroStats* stats, float multiplier);   // 0x005812AE
};
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long bone, const CCharString& name, long flags, float delay);   // 0x006E0BE0
};
struct CGameTimeManager {
    bool IsNightTime();                              // 0x006BB210
    bool IsDay();                                    // 0x006BB2C0
    void SetTime(float time);                        // 0x006BB810
};
struct CWorld {
    unsigned char _pad[0x1c];
    CGameTimeManager* TimeManager;                   // +0x1c
};
struct CHeroStatsDef {
    unsigned char _pad[0x7c];
    float FatnessPerExcessHealth;                    // +0x7c (descriptive)
};
struct CDefinitionHolder {
    unsigned char  _pad_0x00[0xDC];
    CHeroStatsDef* PHeroStatsDef;                    // +0xDC
};
extern const float g_dayTime;                        // 0x0124F6E0
extern const float g_nightTime;                      // 0x0124F6F4

struct CBonusItemDef {
    unsigned char _pad00[0x28];
    float HealthModifier;                            // +0x28
    bool  AddsFatness;                               // +0x2c
    long  StaminaModifier;                           // +0x30
    float SecondsToApplyHealthIncreaseOver;          // +0x34
    long  WillExperience;                            // +0x38
    long  SkillExperience;                           // +0x3c
    long  StrengthExperience;                        // +0x40
    long  MoralityChange;                            // +0x44
    float AgreeablenessChange;                       // +0x48
    float AttractivenessChange;                      // +0x4c
    float ScarinessChange;                           // +0x50
    float MaxHealthIncrease;                         // +0x54
    long  MaxStaminaIncrease;                        // +0x58
    bool  ChangeTimeOfDayToDay;                      // +0x5c
    bool  ChangeTimeOfDayToNight;                    // +0x5d
    float SpeedMultiplier;                           // +0x60
    float SecondsForSpeedMultiplierToLast;           // +0x64
    long  ParticleEffect;                            // +0x68
    bool  AddsDrunkenness;                           // +0x6c
    float AlcoholicStrength;                         // +0x70
};

struct CTCHeroStats {
    void*         __vftable;                         // +0x00
    CThing*       Thing;                             // +0x04
    unsigned char _pad08[0x20];
    long          Morality;                          // +0x28
    unsigned char _pad2c[0x0c];
    float         Fatness;                           // +0x38
    unsigned char _pad3c[0x1c];
    long          MaxStamina;                        // +0x58
    long          Stamina;                           // +0x5c
    unsigned char _pad60[0x20];
    float         Agreeableness;                     // +0x80
    float         Attractiveness;                    // +0x84
    float         Scariness;                         // +0x88
    float         ExtraMaxHealth;                    // +0x8c
    long          ExtraMaxStamina;                   // +0x90
    long          SpeedMultiplierFrames;             // +0x94
    unsigned char _pad98[0x18];
    float         PendingHealthA;                    // +0xb0
    float         PendingHealthB;                    // +0xb4
    CDefinitionHolder* GetDefinitions();             // 0x006869D0
    CWorld* GetWorld();                              // 0x00686A70
    void SetFatness(float value);                    // 0x0057B169
    void ModifyStaminaBy(long amount);               // 0x0057B1DB
    void SetMorality(long value);                    // 0x0057C2E6
    float GetMaxExtraMaxHealth();                    // 0x0057A90B
    long GetMaxExtraMaxStamina();                    // 0x0057A962
    void Drink(float amount);                        // 0x0057A82A
    void ApplyBonusItemEffects(const CBonusItemDef& bonus);
};

void CTCHeroStats::ApplyBonusItemEffects(const CBonusItemDef& bonus)
{
    CThing* thing = Thing;
    if (bonus.AddsFatness) {
        float excess = bonus.HealthModifier - (thing->MaxHealth - thing->Health);
        if (excess > 0.0f)
            SetFatness(excess * GetDefinitions()->PHeroStatsDef->FatnessPerExcessHealth + Fatness);
    }
    if (bonus.SecondsToApplyHealthIncreaseOver < 0.0f)
        thing->ChangeHealth(bonus.HealthModifier, true);
    else if ((float)fabs(bonus.HealthModifier) > 0.0f)
        thing->AddHealthOverTime(bonus.HealthModifier, (long)(g_framesPerSecond * bonus.SecondsToApplyHealthIncreaseOver));
    ModifyStaminaBy(bonus.StaminaModifier);
    if (bonus.StrengthExperience > 0 || bonus.SkillExperience > 0 || bonus.WillExperience > 0) {
        CThing* owner = Thing;
        if (owner->HasTC(0x68)) {
            CTCHeroExperience* experience = (CTCHeroExperience*)owner->GetTC(0x68);
            experience->AddTrainableStatExperience(0, bonus.StrengthExperience);
            experience->AddTrainableStatExperience(1, bonus.SkillExperience);
            experience->AddTrainableStatExperience(2, bonus.WillExperience);
        }
    }
    SetMorality(bonus.MoralityChange + Morality);
    Agreeableness += bonus.AgreeablenessChange;
    Attractiveness += bonus.AttractivenessChange;
    Scariness += bonus.ScarinessChange;
    if ((float)fabs(bonus.MaxHealthIncrease) > 0.0f) {
        float increase = bonus.MaxHealthIncrease;
        float maxExtra = GetMaxExtraMaxHealth();
        if (increase + ExtraMaxHealth > maxExtra)
            increase = maxExtra - ExtraMaxHealth;
        if (increase > 0.0f) {
            ExtraMaxHealth = increase + ExtraMaxHealth;
            thing->SetMaxHealth(increase + thing->MaxHealth, false);
            thing->ChangeHealth(increase, false);
        }
    }
    long stamina = bonus.MaxStaminaIncrease;
    if (abs(stamina) > 0) {
        long maxExtra = GetMaxExtraMaxStamina();
        if (ExtraMaxStamina + stamina > maxExtra)
            stamina = maxExtra - ExtraMaxStamina;
        if (stamina > 0) {
            Stamina += stamina;
            MaxStamina += stamina;
            ExtraMaxStamina += stamina;
        }
    }
    if (bonus.ChangeTimeOfDayToNight) {
        CThing* owner = Thing;
        if (owner->HasTC(0x10))
            ((CTCSound*)owner->GetTC(0x10))->PlaySound(CCharString("MOONFISH", -1), true, 0, 0, -1);
        CGameTimeManager* time = GetWorld()->TimeManager;
        if (!time->IsNightTime())
            time->SetTime(g_nightTime);
    }
    else if (bonus.ChangeTimeOfDayToDay) {
        CThing* owner = Thing;
        if (owner->HasTC(0x10))
            ((CTCSound*)owner->GetTC(0x10))->PlaySound(CCharString("GOLDEN_CARROT", -1), true, 0, 0, -1);
        CGameTimeManager* time = GetWorld()->TimeManager;
        if (!time->IsDay())
            time->SetTime(g_dayTime);
    }
    if ((float)fabs(1.0f - bonus.SpeedMultiplier) > 0.0f && bonus.SpeedMultiplier > 0.0f && bonus.SecondsForSpeedMultiplierToLast > 0.0f) {
        CThing* owner = Thing;
        if (owner->HasTC(0x8c)) {
            ((CTCSpeedModifier*)owner->GetTC(0x8c))->Apply(this, bonus.SpeedMultiplier);
            float frames = g_framesPerSecond * bonus.SecondsForSpeedMultiplierToLast;
            float half = 0.5f;
            float one = 1.0f;
            long rounded = (long)(frames + half);
            float back = (float)rounded - one;
            if (*(long*)&frames == *(long*)&back)
                --rounded;
            SpeedMultiplierFrames = rounded;
        }
    }
    PendingHealthA = 0;
    PendingHealthB = 0;
    if (bonus.ParticleEffect > 0) {
        CThing* effect = CreateThing(bonus.ParticleEffect, Thing->GetPosition(), false);
        if (IS_THING_ALIVE(&effect)) {
            CTCBase* driver = 0;
            effect->GetTCDriver(&driver);
            ((CTCDParticleEmitter*)driver)->AttachToThing(Thing, 8, CCharString("", -1), 0, 0.0f);
        }
    }
    if (bonus.AddsDrunkenness)
        Drink(bonus.AlcoholicStrength);
}
