// Deterministic dependency fixtures with the production constructor body below.
#include <cstdio>
#include <cstring>

typedef const char* CCharString;
enum EHeroAbility { HERO_ABILITY_DIVINE_WRATH_SPELL = 18 };
struct C3DVector { float X, Y, Z; };
struct CThing;

const CCharString PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME = "WEAPON_FOCUS_01";
const CCharString SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME = "WEAPON_FOCUS_02";

template<class T> struct CIntelligentPointer {
    T* Pointer;
    CIntelligentPointer(T* pointer = 0) : Pointer(pointer) {}
    T* operator->() const { return Pointer; }
};
template<class T> bool IS_THING_ALIVE(const CIntelligentPointer<T>& thing)
{
    return thing.Pointer && !thing->IsDead();
}
template<class T> struct CDefPointer {
    T* Pointer;
    CDefPointer() : Pointer(0) {}
    T* operator->() const { return Pointer; }
};
struct CDefString {
    CCharString Name;
    operator CCharString() const { return Name; }
};

struct CTCDParticleEmitter {
    const CThing* Target;
    long Attachment;
    CCharString Helper;
    bool Update;
    CTCDParticleEmitter() : Target(0), Attachment(0), Helper(""), Update(false) {}
    static CThing* Create(long low, long high, float blend, const C3DVector& pos, bool flag);
    void AttachToThing(CThing& target, long attachment, const CCharString& helper, long index, float offset);
    void SetAlwaysUpdate(bool update) { Update = update; }
};
struct CTCSpecialAbilities {
    bool Locked;
    EHeroAbility Ability;
    CTCSpecialAbilities() : Locked(false), Ability(EHeroAbility(0)) {}
    void LockAbilitiyMutex(EHeroAbility ability) { Locked = true; Ability = ability; }
    static CThing* CreateParticleEffect(long effect, const C3DVector& pos);
};
struct CTCInventoryAbilities {
    long Level;
    mutable EHeroAbility Requested;
    CTCInventoryAbilities() : Level(0), Requested(EHeroAbility(0)) {}
    long GetAbilityLevel(EHeroAbility ability) const { Requested = ability; return Level; }
};
struct CTCVolumeContainmentTracker {
    bool Update;
    CTCVolumeContainmentTracker() : Update(true) {}
    void SetUpdateLightingChannels(bool update) { Update = update; }
};
struct CEnginePrimitiveThemeChannelDesc {
    int Index0, Index1, Blend;
    CEnginePrimitiveThemeChannelDesc(int i0 = 0, int i1 = 0, int blend = 0)
        : Index0(i0), Index1(i1), Blend(blend) {}
};
namespace NGraphicAppearance {
struct CTCGraphicAppearance {
    CEnginePrimitiveThemeChannelDesc Channel;
    int Calls;
    CTCGraphicAppearance() : Channel(2, 3, 4), Calls(0) {}
    const CEnginePrimitiveThemeChannelDesc& PeekLightingChannel() const { return Channel; }
    void SetLightingChannel(const CEnginePrimitiveThemeChannelDesc& channel, bool update_primitives, bool update_children)
    {
        Channel = channel;
        Calls += update_primitives && update_children ? 1 : 100;
    }
};
}
struct CThing {
    C3DVector Pos;
    bool Dead;
    CTCSpecialAbilities* Special;
    CTCInventoryAbilities* Inventory;
    CTCVolumeContainmentTracker* Volume;
    NGraphicAppearance::CTCGraphicAppearance* Appearance;
    CTCDParticleEmitter* Emitter;
    CThing() : Dead(false), Special(0), Inventory(0), Volume(0), Appearance(0), Emitter(0)
    {
        Pos.X = 0; Pos.Y = 0; Pos.Z = 0;
    }
    bool IsDead() const { return Dead; }
    const C3DVector& GetPos() const { return Pos; }
    bool GetTC(CTCSpecialAbilities** out) { *out = Special; return Special != 0; }
    bool GetTC(CTCVolumeContainmentTracker** out) { *out = Volume; return Volume != 0; }
    bool GetTC(CTCDParticleEmitter** out) { *out = Emitter; return Emitter != 0; }
    bool PeekTC(const CTCInventoryAbilities** out) const { *out = Inventory; return Inventory != 0; }
    NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance() { return Appearance; }
};

static bool Good;
static CThing* Ring;
static CThing* Hands[2];
static const C3DVector* CasterPos;
static long RingsCreated, HandsCreated;
static float RingScale;
static long SoundCalls;
static CCharString Sounds[2];

CThing* CTCDParticleEmitter::Create(long low, long high, float blend, const C3DVector& pos, bool flag)
{
    Good = Good && low == 9 && high == 17 && &pos == CasterPos && !flag;
    ++RingsCreated;
    RingScale = blend;
    return Ring;
}
void CTCDParticleEmitter::AttachToThing(CThing& target, long attachment,
    const CCharString& helper, long index, float offset)
{
    Good = Good && index == 0 && offset == 0.0f && &target.Pos == CasterPos;
    Target = &target;
    Attachment = attachment;
    Helper = helper;
}
CThing* CTCSpecialAbilities::CreateParticleEffect(long effect, const C3DVector& pos)
{
    Good = Good && effect == 23 && &pos == CasterPos && HandsCreated < 2;
    return Hands[HandsCreated++ % 2];
}

struct CSpecialEffectsDef {
    bool HasSmallRing, HasLargeRing, HasHand;
    mutable long Lookups;
    bool GetEffectFromString(const CCharString& name, long& effect) const
    {
        ++Lookups;
        if (std::strcmp(name, "SMALL_RING_EFFECT") == 0) { effect = 9; return HasSmallRing; }
        if (std::strcmp(name, "LARGE_RING_EFFECT") == 0) { effect = 17; return HasLargeRing; }
        if (std::strcmp(name, "HAND_EFFECT") == 0) { effect = 23; return HasHand; }
        Good = false;
        return false;
    }
};
struct CSpecialAbilitiesDivineWrathDef {
    float NumSecsBetweenDamageHit[2], DurationOfSpellSecs[2], EnvChangeSecs[2], StaminaCostPerSec[2], RadiusOfEffect[2];
    float SmallRingRadius, LargeRingRadius;
    CDefString StartPowerupSound[2], LoopingPowerupSound[2];
    CSpecialEffectsDef Effects;
    bool GetSubDef(CDefPointer<const CSpecialEffectsDef>* out) const
    {
        out->Pointer = &Effects;
        return true;
    }
};
static CSpecialAbilitiesDivineWrathDef Definition;
static long Theme;
struct CGameDefinitionManager {
    long GetGlobalIndexFromClassIndex(const CCharString& name, long index) const
    {
        Good = Good && std::strcmp(name, "SPECIAL_ABILITIES_DIVINE_WRATH_DEF") == 0 && index == 1;
        return 27;
    }
    bool GetDef(long index, CDefPointer<const CSpecialAbilitiesDivineWrathDef>* out) const
    {
        Good = Good && index == 27;
        out->Pointer = &Definition;
        return true;
    }
    long GetDefGlobalIndexFromName(const CCharString& name) const
    {
        Good = Good && std::strcmp(name, "ENVIRONMENT_DIVINE_WRATH") == 0;
        return Theme;
    }
};
struct CEnvironment {
    int Calls, ChannelMask;
    float Time;
    void TransitionToTheme(long channel, long priority, long index, float time)
    {
        Good = Good && channel == Calls && priority == 2 && index == Theme;
        ++Calls;
        ChannelMask |= 1 << channel;
        Time = time;
    }
};
struct CWorld {
    CEnvironment Environment;
    CEnvironment& GetEnvironment() { return Environment; }
};
static CWorld World;
struct CTCBase {
    CThing& Parent;
    CTCBase(CThing& thing) : Parent(thing) {}
    CThing& GetParentThing() { return Parent; }
    const CGameDefinitionManager& GetDefinitionManager() const { static CGameDefinitionManager manager; return manager; }
    CWorld& GetWorld() const { return World; }
};
struct CMainGameComponent {
    static long GetConstantFPS() { return 4; }
};
struct CWillButtonHelperTimer {
    EHeroAbility Ability;
    CThing& Parent;
    CWillButtonHelperTimer(EHeroAbility ability, CThing& parent) : Ability(ability), Parent(parent) {}
};
struct CThingCreatureBase;
struct CTCDivineWrath : CTCBase {
    CDefPointer<const CSpecialAbilitiesDivineWrathDef> PDef;
    CIntelligentPointer<CThing> PItemRemoved;
    bool DivineWrathActionInitialised;
    CWillButtonHelperTimer Timer;
    long SpellLevel, NumFramesBetweenDamageHit, DurationOfSpellFrames, NumFramesForEnvChange, CurFramesForEnvChange;
    bool Finished;
    long NumFramesToWaitForFadeOut;
    bool CutSceneCancel;
    CIntelligentPointer<CThingCreatureBase> PTarget;
    bool IsZTargetting, PerformedReleaseAction;
    float PerFrameStaminaModify, AccModifyStamina;
    long StaminaModify;
    CIntelligentPointer<CThing> BuildUpEffect;
    CEnginePrimitiveThemeChannelDesc HeroDefaultChannel, SpellExclusionChannel;
    CIntelligentPointer<CThing> HandEffect1, HandEffect2;
    unsigned long LoopingSound;
    CIntelligentPointer<CThing> PObstruction;
    CTCDivineWrath(CThing& thing);
    unsigned long PlaySoundW(const CCharString& name, bool loop)
    {
        Good = Good && !loop && SoundCalls < 2;
        Sounds[SoundCalls++ % 2] = name;
        return 40 + SoundCalls;
    }
};

CTCDivineWrath::CTCDivineWrath(CThing& thing)
    : CTCBase(thing), PItemRemoved(0), DivineWrathActionInitialised(false),
      Timer(HERO_ABILITY_DIVINE_WRATH_SPELL, GetParentThing()), SpellLevel(0),
      NumFramesBetweenDamageHit(0), DurationOfSpellFrames(0), NumFramesForEnvChange(0),
      CurFramesForEnvChange(0), Finished(false), NumFramesToWaitForFadeOut(0),
      CutSceneCancel(false), PTarget(0), IsZTargetting(false), PerformedReleaseAction(false),
      PerFrameStaminaModify(0), AccModifyStamina(0), StaminaModify(0), BuildUpEffect(0),
      SpellExclusionChannel(5, 5, 0xff), LoopingSound(0)
{
    CTCSpecialAbilities* ptc_spec_abils;
    if (GetParentThing().GetTC(&ptc_spec_abils))
        ptc_spec_abils->LockAbilitiyMutex(HERO_ABILITY_DIVINE_WRATH_SPELL);

    long def_index = GetDefinitionManager().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_DIVINE_WRATH_DEF", 1);
    GetDefinitionManager().GetDef(def_index, &PDef);

    const CTCInventoryAbilities* ptc_inventory_abilities;
    if (GetParentThing().PeekTC(&ptc_inventory_abilities))
        SpellLevel = ptc_inventory_abilities->GetAbilityLevel(HERO_ABILITY_DIVINE_WRATH_SPELL);

    NumFramesBetweenDamageHit = static_cast<long>(CMainGameComponent::GetConstantFPS() * PDef->NumSecsBetweenDamageHit[SpellLevel]);
    DurationOfSpellFrames = static_cast<long>(CMainGameComponent::GetConstantFPS() * PDef->DurationOfSpellSecs[SpellLevel]);
    NumFramesForEnvChange = static_cast<long>(CMainGameComponent::GetConstantFPS() * PDef->EnvChangeSecs[SpellLevel]);

    long theme_index = GetDefinitionManager().GetDefGlobalIndexFromName("ENVIRONMENT_DIVINE_WRATH");
    if (theme_index > 0) {
        for (long i = 0; i < 6; ++i) {
            if (i == 5)
                continue;
            GetWorld().GetEnvironment().TransitionToTheme(i, 2, theme_index, PDef->EnvChangeSecs[SpellLevel]);
        }
    }

    NGraphicAppearance::CTCGraphicAppearance* ptc_graphic_appearance = GetParentThing().GetTCGraphicAppearance();
    if (ptc_graphic_appearance) {
        HeroDefaultChannel = ptc_graphic_appearance->PeekLightingChannel();
        ptc_graphic_appearance->SetLightingChannel(SpellExclusionChannel, true, true);
    }

    CTCVolumeContainmentTracker* ptc_volume_contain;
    if (GetParentThing().GetTC(&ptc_volume_contain))
        ptc_volume_contain->SetUpdateLightingChannels(false);

    float perframe = PDef->StaminaCostPerSec[SpellLevel] / CMainGameComponent::GetConstantFPS();
    StaminaModify = static_cast<long>(perframe);
    PerFrameStaminaModify = perframe - static_cast<float>(StaminaModify);

    CDefPointer<const CSpecialEffectsDef> pspecial_effects;
    long effect_index_lo = 0;
    long effect_index_hi = 0;
    if (PDef->GetSubDef(&pspecial_effects)) {
        if (pspecial_effects->GetEffectFromString("SMALL_RING_EFFECT", effect_index_lo)
            && pspecial_effects->GetEffectFromString("LARGE_RING_EFFECT", effect_index_hi)) {
            float interpolate = (PDef->RadiusOfEffect[SpellLevel] - PDef->SmallRingRadius)
                / (PDef->LargeRingRadius - PDef->SmallRingRadius);
            BuildUpEffect = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(
                effect_index_lo, effect_index_hi, interpolate, GetParentThing().GetPos(), false));
            if (IS_THING_ALIVE(BuildUpEffect)) {
                CTCDParticleEmitter* ptcEmitter = 0;
                BuildUpEffect->GetTC(&ptcEmitter);
                ptcEmitter->AttachToThing(GetParentThing(), 8, "", 0, 0.0f);
                ptcEmitter->SetAlwaysUpdate(true);
            }
        }
    }

    long hand_effect = 0;
    if (pspecial_effects->GetEffectFromString("HAND_EFFECT", hand_effect)) {
        HandEffect1 = CIntelligentPointer<CThing>(CTCSpecialAbilities::CreateParticleEffect(hand_effect, GetParentThing().GetPos()));
        HandEffect2 = CIntelligentPointer<CThing>(CTCSpecialAbilities::CreateParticleEffect(hand_effect, GetParentThing().GetPos()));
        if (IS_THING_ALIVE(HandEffect1)) {
            CTCDParticleEmitter* tcParticleEmitter = 0;
            if (HandEffect1->GetTC(&tcParticleEmitter) && tcParticleEmitter)
                tcParticleEmitter->AttachToThing(GetParentThing(), 10, PRIMARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME, 0, 0.0f);
        }
        if (IS_THING_ALIVE(HandEffect2)) {
            CTCDParticleEmitter* tcParticleEmitter = 0;
            HandEffect2->GetTC(&tcParticleEmitter);
            tcParticleEmitter->AttachToThing(GetParentThing(), 10, SECONDARY_HOLDABLE_WEAPON_DUMMY_OBJECT_NAME, 0, 0.0f);
        }
    }

    PlaySoundW(PDef->StartPowerupSound[SpellLevel], false);
    LoopingSound = PlaySoundW(PDef->LoopingPowerupSound[SpellLevel], false);
}

int main()
{
    Definition.SmallRingRadius = 1.0f;
    Definition.LargeRingRadius = 5.0f;
    Definition.StartPowerupSound[0].Name = "fixture_start_level_0";
    Definition.StartPowerupSound[1].Name = "fixture_start_level_1";
    Definition.LoopingPowerupSound[0].Name = "fixture_loop_level_0";
    Definition.LoopingPowerupSound[1].Name = "fixture_loop_level_1";
    for (long level = 0; level < 2; ++level) {
        Definition.NumSecsBetweenDamageHit[level] = 1.0f + level;
        Definition.DurationOfSpellSecs[level] = 3.0f + level;
        Definition.EnvChangeSecs[level] = 2.0f + level;
        Definition.StaminaCostPerSec[level] = 6.0f + level * 4.0f;
        Definition.RadiusOfEffect[level] = 3.0f + level;
    }

    for (int i = 0; i < 128; ++i) {
        CThing caster, ring, primary, secondary;
        CTCSpecialAbilities special;
        CTCInventoryAbilities inventory;
        CTCVolumeContainmentTracker volume;
        NGraphicAppearance::CTCGraphicAppearance appearance;
        CTCDParticleEmitter ringEmitter, primaryEmitter, secondaryEmitter;
        const bool has_special = (i & 1) != 0;
        const long level = (i & 2) ? 1 : 0;
        const bool has_volume = (i & 4) != 0;
        const bool has_appearance = (i % 3) != 0;
        caster.Special = has_special ? &special : 0;
        caster.Inventory = level ? &inventory : 0;
        caster.Volume = has_volume ? &volume : 0;
        caster.Appearance = has_appearance ? &appearance : 0;
        inventory.Level = 1;
        CasterPos = &caster.Pos;
        ring.Emitter = &ringEmitter;
        primary.Emitter = i % 6 ? &primaryEmitter : 0;
        secondary.Emitter = &secondaryEmitter;

        Ring = i % 11 ? &ring : 0;
        Hands[0] = i % 7 ? &primary : 0;
        Hands[1] = i % 11 ? &secondary : 0;
        ring.Dead = i % 13 == 0;
        primary.Dead = i % 4 == 0;
        secondary.Dead = i % 5 == 0;
        Definition.Effects.HasSmallRing = i % 5 != 0;
        Definition.Effects.HasLargeRing = i % 7 != 0;
        Definition.Effects.HasHand = i % 3 != 0;
        Definition.Effects.Lookups = 0;
        Theme = i % 3 == 0 ? 42 : i % 3 == 1 ? 0 : -1;
        World.Environment.Calls = World.Environment.ChannelMask = 0;
        RingsCreated = HandsCreated = SoundCalls = 0;
        Good = true;

        CTCDivineWrath spell(caster);
        if (!Good || special.Locked != has_special || special.Ability != EHeroAbility(has_special ? 18 : 0)
            || inventory.Requested != EHeroAbility(level ? 18 : 0) || volume.Update == has_volume
            || spell.Timer.Ability != HERO_ABILITY_DIVINE_WRATH_SPELL || &spell.Timer.Parent != &caster)
            return 1;
        if (spell.SpellLevel != level || spell.NumFramesBetweenDamageHit != 4 * (1 + level)
            || spell.DurationOfSpellFrames != 4 * (3 + level) || spell.NumFramesForEnvChange != 4 * (2 + level)
            || spell.StaminaModify != 1 + level || spell.PerFrameStaminaModify != 0.5f || spell.AccModifyStamina != 0.0f)
            return 2;
        if (World.Environment.Calls != (Theme > 0 ? 5 : 0) || World.Environment.ChannelMask != (Theme > 0 ? 31 : 0)
            || (Theme > 0 && World.Environment.Time != 2 + level))
            return 3;
        if (appearance.Calls != (has_appearance ? 1 : 0)
            || (has_appearance && (spell.HeroDefaultChannel.Index0 != 2 || spell.HeroDefaultChannel.Index1 != 3
            || spell.HeroDefaultChannel.Blend != 4 || appearance.Channel.Index0 != 5
            || appearance.Channel.Index1 != 5 || appearance.Channel.Blend != 255)))
            return 4;
        const bool create_ring = Definition.Effects.HasSmallRing && Definition.Effects.HasLargeRing;
        const bool live_ring = create_ring && Ring && !ring.Dead;
        if (Definition.Effects.Lookups != 2 + Definition.Effects.HasSmallRing
            || RingsCreated != (create_ring ? 1 : 0) || (create_ring && RingScale != (2 + level) * 0.25f)
            || ringEmitter.Update != live_ring || ringEmitter.Attachment != (live_ring ? 8 : 0)
            || (live_ring && std::strcmp(ringEmitter.Helper, "") != 0))
            return 5;
        const bool live_primary = Definition.Effects.HasHand && Hands[0] && !primary.Dead && primary.Emitter;
        const bool live_secondary = Definition.Effects.HasHand && Hands[1] && !secondary.Dead;
        if (HandsCreated != (Definition.Effects.HasHand ? 2 : 0)
            || primaryEmitter.Attachment != (live_primary ? 10 : 0) || secondaryEmitter.Attachment != (live_secondary ? 10 : 0)
            || (live_primary && std::strcmp(primaryEmitter.Helper, "WEAPON_FOCUS_01") != 0)
            || (live_secondary && std::strcmp(secondaryEmitter.Helper, "WEAPON_FOCUS_02") != 0))
            return 6;
        if (SoundCalls != 2 || Sounds[0] != Definition.StartPowerupSound[level].Name
            || Sounds[1] != Definition.LoopingPowerupSound[level].Name || spell.LoopingSound != 42
            || spell.Finished || spell.DivineWrathActionInitialised || spell.PTarget.Pointer || spell.PItemRemoved.Pointer
            || spell.PObstruction.Pointer || spell.IsZTargetting || spell.PerformedReleaseAction || spell.CutSceneCancel
            || spell.NumFramesToWaitForFadeOut || spell.CurFramesForEnvChange)
            return 7;
    }

    std::puts("0078ecd0_TEST PASS");
    return 0;
}