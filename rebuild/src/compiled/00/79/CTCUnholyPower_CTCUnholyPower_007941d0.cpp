// CTCUnholyPower::CTCUnholyPower(CThing&) -- construct the unholy-power spell component: default members, lock the caster's ability mutex on the spell, look up the spell def and the caster's spell level, turn the frame counts from the def's seconds, start the environment theme on every channel but 5, swap the caster's lighting channel for the spell-exclusion one, stop the containment tracker's lighting updates, split the per-frame stamina cost, create the build-up ring / hand / dark-faces effects from the def's effects table, play the start and looping sounds
// Leaf: ??0CTCUnholyPower@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007941D0, 2073 bytes, thiscall, `ret 4` (returns this), no EH frame.
// Identity (proven): the vtable stored after CTCBase::CTCBase is 0x0126BFDC, RTTI .?AVCTCUnholyPower@@ (the landed ~CTCUnholyPower
//   0x00794AC0 stores the same); the only caller is CTCUnholyPower::Alloc 0x004D4ACC (Ego_r ?Alloc@CTCUnholyPower@@SIPAVCTCBase@@AAVCThing@@@Z:
//   `new` of 0xD0 bytes, the class size, then this call); Ego_r.pdb ??0CTCUnholyPower@@QAE@AAVCThing@@@Z; the literals
//   "SPECIAL_ABILITIES_UNHOLY_POWER_DEF", "ENVIRONMENT_UNHOLY_POWER", "SMALL_RING_EFFECT" / "LARGE_RING_EFFECT" / "HAND_EFFECT" /
//   "DARK_FACES_EFFECT".
// Callees: CTCBase::CTCBase 0x00686800; CBaseIntelligentPointer() 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / ~ 0x00A01C10;
//   CWillButtonHelperTimer(EHeroAbility, CThing&) 0x008EDFF0; malloc 0x00BFEA0E (the map header); CTCBase::GetDefinitionManager 0x006869D0,
//   CDefinitionManager::GetGlobalIndexFromClassIndex 0x009AD390 / GetDefGlobalIndexFromName 0x009AD410 / GetDef<CSpecialAbilitiesUnholyPowerDef>
//   0x005F8AB9; CVectorMap::LowerBound 0x0040F020 (interfaces 0xA7, 0x75, 0x4B) and the const 0x004365B0 (0x6F);
//   CTCInventoryAbilities::GetAbilityLevel 0x005EDE93; __ftol 0x00BFEA70; CTCBase::GetWorld 0x00686A70, CEnvironment::TransitionToTheme(long,
//   long, long, float) 0x006B3800; NGraphicAppearance::CTCGraphicAppearance::SetLightingChannel 0x004BFB40; CDefClassBase::GetSubDef
//   <CSpecialEffectsDef> 0x00669580; CSpecialEffectsDef::GetEffectFromString 0x00669D30; CThing::GetPos 0x004C73D0;
//   CTCDParticleEmitter::Create(long, long, float, const C3DVector&, bool) 0x006E11F0 / AttachToThing 0x006E0BE0 / SetAlwaysUpdate
//   0x006E0590; CTCSpecialAbilities::CreateParticleEffect 0x0072AB70; g_RightHandAttachmentName 0x013BAC30 / g_LeftHandAttachmentName
//   0x013BAC34; CDefStringTable::GetString 0x009D49B0 on GDefStringTable 0x013CA828; CTCUnholyPower::PlaySoundW 0x007940F0;
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0. All names are Ego_r.pdb / FableWin.pdb spellings.
// Layout: Ego_r.pdb CTCUnholyPower with retail STLport sizes, as the landed dtor 0x00794AC0 (PDef +0x0C .. PObstruction +0xC8, size 0xD0).
// Headers: engine/CThing.h (GETTC_EXPANDED: the 0x6F / 0x75 / 0x4B lookups; the 0xA7 one is HasTC + the forced-inline GetTC(type) on
//   Components straight, `cmp [eax],0A7h` folded), engine/CTCBase.h, CIntelligentPointer.h, CWillButtonHelperTimer.h,
//   CTCInventoryAbilities.h, CGameDefinitionManager.h, CWorld.h (GetEnvironment), CMainGameComponent.h (ConstantFPS 0x01375550),
//   stlport/fable_map.h; chunk 782 overlay opt-ins: CEnvironment.h TRANSITIONTOTHEME, NGraphicAppearance_CTCGraphicAppearance.h
//   LIGHTINGCHANNEL (PeekLightingChannel inline, LightingChannel +0x40), CTCVolumeContainmentTracker.h LIGHTING (SetUpdateLightingChannels
//   inline, UpdateLightingChannels +0x20), CEnginePrimitiveThemeChannelDesc.h INDICES (the EEE ctor), CSpecialEffectsDef.h TYPENAME +
//   GETEFFECT_BODY, and the new CSpecialAbilitiesUnholyPowerDef.h. CTCSpecialAbilities and CTCDParticleEmitter have no engine header:
//   modelled below with the members / Ego_r spellings the row uses.
// Shape: GetSubDef<T> is the Ego_r member template; its body (the landed 0x00669580 one) is visible here as in retail's TU (the template
//   lives in the def headers), so VC7.1 keeps the effects def in edi; GetEffectFromString's same-TU noinline body (overlay) does the same for
//   the hand-effect index. The small / large ring indices are function-scope locals zeroed before GetSubDef; the hand and dark-faces
//   indices are block locals (they take the large index's slot, the hand temporaries the small one's).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_TRANSITIONTOTHEME
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_LIGHTINGCHANNEL
#define FABLE_RETAIL_CTCVOLUMECONTAINMENTTRACKER_LIGHTING
#define FABLE_RETAIL_CENGINEPRIMITIVETHEMECHANNELDESC_INDICES
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/C3DVector.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CWillButtonHelperTimer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCVolumeContainmentTracker.h"
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
#include "engine/CDefClassBase.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefString.h"
#include "engine/CSpecialAbilitiesUnholyPowerDef.h"

template <class T> bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(T::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

class CTCSpecialAbilities : public CTCBase {               // interface TCI_SPECIAL_ABILITIES (167)
public:
    void LockAbilitiyMutex(EHeroAbility ability) { AbilityMutexLock = true; CurrentAbility = ability; }
    static CThing* __fastcall CreateParticleEffect(long effect, const C3DVector& position);   // 0x0072AB70
    bool AbilityMutexLock;                                 // +0x0C
    bool UsingBed;                                         // +0x0D
    EHeroAbility CurrentAbility;                           // +0x10
};
class CTCDParticleEmitter : public CTCBase {               // interface TCI_PARTICLE_EMITTER (75)
public:
    static CThing* __fastcall Create(long effect, long large_effect, float blend, const C3DVector& position, bool flag);   // 0x006E11F0
    void AttachToThing(CThing& thing, long attach_type, const CCharString& helper_name, long helper_index, float offset);  // 0x006E0BE0
    void SetAlwaysUpdate(bool always);                     // 0x006E0590
};
extern CCharString g_RightHandAttachmentName;              // 0x013BAC30
extern CCharString g_LeftHandAttachmentName;               // 0x013BAC34

template <class T> inline bool IS_THING_ALIVE(const CIntelligentPointer<T>& p)
{
    if (p.Get() != 0) {
        if (!p->IsDead())
            return true;
    }
    return false;
}

class CTCUnholyPower : public CTCBase {                    // vtable 0x0126BFDC
public:
    struct PerCreatureInfo {
        CIntelligentPointer<CThingCreatureBase>     ActualCreature;     // +0x00
        long                                        FramesToGetHit;     // +0x08
        CIntelligentPointer<CThing>                 Effect;             // +0x0C
        CEnginePrimitiveThemeChannelDesc            DefaultChannel;     // +0x14
    };
    struct PerEffectInfo {
        C3DVector                                   Position;           // +0x00
        CIntelligentPointer<CThing>                 Effect;             // +0x0C
    };
    CTCUnholyPower(CThing& thing);
    virtual ~CTCUnholyPower();
private:
    unsigned long PlaySoundW(const CCharString& sound, bool looping);   // 0x007940F0
    CDefPointer<const CSpecialAbilitiesUnholyPowerDef>          PDef;                           // +0x0C
    CIntelligentPointer<CThing>                                 PItemRemoved;                   // +0x10
    bool                                                        UnholyPowerActionInitialised;   // +0x18
    CWillButtonHelperTimer                                      Timer;                          // +0x1C
    long                                                        SpellLevel;                     // +0x50
    long                                                        NumFramesBetweenDamageHit;      // +0x54
    long                                                        DurationOfSpellFrames;          // +0x58
    long                                                        NumFramesForEnvChange;          // +0x5C
    long                                                        CurFramesForEnvChange;          // +0x60
    bool                                                        Finished;                       // +0x64
    long                                                        NumFramesToWaitForFadeOut;      // +0x68
    bool                                                        CutSceneCancel;                 // +0x6C
    CIntelligentPointer<CThingCreatureBase>                     PTarget;                        // +0x6D
    bool                                                        IsZTargetting;                  // +0x75
    bool                                                        PerformedReleaseAction;         // +0x76
    std::map<CThingCreatureBase*, PerCreatureInfo>              LastFrameCreaturesHit;          // +0x78
    std::vector<PerEffectInfo>                                  HitEffects;                     // +0x84
    float                                                       PerFrameStaminaModify;          // +0x90
    float                                                       AccModifyStamina;               // +0x94
    long                                                        StaminaModify;                  // +0x98
    CIntelligentPointer<CThing>                                 BuildUpEffect;                  // +0x9C
    CIntelligentPointer<CThing>                                 DarkFacesEffect;                // +0xA4
    CEnginePrimitiveThemeChannelDesc                            HeroDefaultChannel;             // +0xAC
    CEnginePrimitiveThemeChannelDesc                            SpellExclusionChannel;          // +0xB0
    CIntelligentPointer<CThing>                                 HandEffect1;                    // +0xB4
    CIntelligentPointer<CThing>                                 HandEffect2;                    // +0xBC
    unsigned long                                               LoopingSound;                   // +0xC4
    CIntelligentPointer<CThing>                                 PObstruction;                   // +0xC8
};

CTCUnholyPower::CTCUnholyPower(CThing& thing)
    : CTCBase(thing)
    , PItemRemoved(0)
    , UnholyPowerActionInitialised(false)
    , Timer(HERO_ABILITY_UNHOLY_POWER_SPELL, GetParentThing())
    , SpellLevel(0)
    , NumFramesBetweenDamageHit(0)
    , DurationOfSpellFrames(0)
    , NumFramesForEnvChange(0)
    , CurFramesForEnvChange(0)
    , Finished(false)
    , NumFramesToWaitForFadeOut(0)
    , CutSceneCancel(false)
    , PTarget(0)
    , IsZTargetting(false)
    , PerformedReleaseAction(false)
    , PerFrameStaminaModify(0)
    , AccModifyStamina(0)
    , StaminaModify(0)
    , BuildUpEffect(0)
    , DarkFacesEffect(0)
    , HeroDefaultChannel(0, 0, 0)
    , SpellExclusionChannel(5, 5, 0xFF)
    , LoopingSound(0)
{
    CThing& parent = GetParentThing();
    if (parent.HasTC(TCI_SPECIAL_ABILITIES)) {
        CTCSpecialAbilities& special_abilities = static_cast<CTCSpecialAbilities&>(parent.GetTC(TCI_SPECIAL_ABILITIES));
        special_abilities.LockAbilitiyMutex(HERO_ABILITY_UNHOLY_POWER_SPELL);
    }
    long def_index = GetDefinitionManager().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_UNHOLY_POWER_DEF", 1);
    GetDefinitionManager().GetDef(def_index, &PDef);
    const CTCInventoryAbilities* inventory_abilities;
    if (parent.PeekTC(TCI_HERO_ABILITIES, &inventory_abilities))
        SpellLevel = inventory_abilities->GetAbilityLevel(HERO_ABILITY_UNHOLY_POWER_SPELL);
    NumFramesBetweenDamageHit = (long)(CMainGameComponent::GetConstantFPS() * PDef->NumSecsBetweenDamageHit[SpellLevel]);
    DurationOfSpellFrames = (long)(CMainGameComponent::GetConstantFPS() * PDef->DurationOfSpellSecs[SpellLevel]);
    NumFramesForEnvChange = (long)(CMainGameComponent::GetConstantFPS() * PDef->EnvChangeSecs[SpellLevel]);
    long theme_index = GetDefinitionManager().GetDefGlobalIndexFromName("ENVIRONMENT_UNHOLY_POWER");
    if (theme_index > 0) {
        for (long channel = 0; channel < 6; ++channel) {
            if (channel != 5)
                GetWorld().GetEnvironment().TransitionToTheme(channel, 2, theme_index, PDef->EnvChangeSecs[SpellLevel]);
        }
    }
    if (NGraphicAppearance::CTCGraphicAppearance* appearance = parent.GetTCGraphicAppearance()) {
        HeroDefaultChannel = appearance->PeekLightingChannel();
        appearance->SetLightingChannel(SpellExclusionChannel, true, true);
    }
    CTCVolumeContainmentTracker* tracker;
    if (parent.GetTC(TCI_VOLUME_CONTAINMENT_TRACKER, &tracker))
        tracker->SetUpdateLightingChannels(false);
    float stamina_per_frame = PDef->StaminaCostPerSec[SpellLevel] / CMainGameComponent::GetConstantFPS();
    StaminaModify = (long)stamina_per_frame;
    PerFrameStaminaModify = stamina_per_frame - StaminaModify;
    CDefPointer<const CSpecialEffectsDef> effects;
    long small_ring = 0;
    long large_ring = 0;
    if (PDef->GetSubDef(&effects)) {
        {
            if (effects->GetEffectFromString("SMALL_RING_EFFECT", small_ring) && effects->GetEffectFromString("LARGE_RING_EFFECT", large_ring)) {
                float blend = (PDef->RadiusOfEffect[SpellLevel] - PDef->SmallRingRadius) / (PDef->LargeRingRadius - PDef->SmallRingRadius);
                BuildUpEffect = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(small_ring, large_ring, blend, parent.GetPos(), false));
                if (IS_THING_ALIVE(BuildUpEffect)) {
                    CTCDParticleEmitter* emitter = 0;
                    BuildUpEffect->GetTC(TCI_PARTICLE_EMITTER, &emitter);
                    emitter->AttachToThing(parent, 8, "", 0, 0);
                    emitter->SetAlwaysUpdate(true);
                }
            }
        }
        {
            long hand_effect = 0;
            if (effects->GetEffectFromString("HAND_EFFECT", hand_effect)) {
                HandEffect1 = CIntelligentPointer<CThing>(CTCSpecialAbilities::CreateParticleEffect(hand_effect, GetParentThing().GetPos()));
                HandEffect2 = CIntelligentPointer<CThing>(CTCSpecialAbilities::CreateParticleEffect(hand_effect, GetParentThing().GetPos()));
                if (IS_THING_ALIVE(HandEffect1)) {
                    CTCDParticleEmitter* emitter = 0;
                    HandEffect1->GetTC(TCI_PARTICLE_EMITTER, &emitter);
                    if (emitter)
                        emitter->AttachToThing(GetParentThing(), 10, g_RightHandAttachmentName, 0, 0);
                }
                if (IS_THING_ALIVE(HandEffect2)) {
                    CTCDParticleEmitter* emitter = 0;
                    HandEffect2->GetTC(TCI_PARTICLE_EMITTER, &emitter);
                    emitter->AttachToThing(GetParentThing(), 10, g_LeftHandAttachmentName, 0, 0);
                }
            }
        }
        {
            long dark_faces_effect = 0;
            if (effects->GetEffectFromString("DARK_FACES_EFFECT", dark_faces_effect))
                DarkFacesEffect = CIntelligentPointer<CThing>(CTCSpecialAbilities::CreateParticleEffect(dark_faces_effect, GetParentThing().GetPos()));
        }
    }
    PlaySoundW(PDef->StartPowerupSound[SpellLevel], false);
    LoopingSound = PlaySoundW(PDef->LoopingPowerupSound[SpellLevel], false);
}