// CTCSummonSpell::CreateMinion(const C3DVector& pos, const C3DVector* direction) -- the summon spell's creature: play the appear sound,
//   make a noise, create the summon-creature AI creature at pos for the caster's player, move it to a valid teleport position facing
//   along direction (or away from the caster, fast-normalised), name it, drop bullet-time / decapitation, join it to FACTION_HERO as the
//   caster's ally and follower, give it its summonable brain, the appearing / alive / on-die / glowing-eyes effects from the spell's
//   CSpecialEffectsDef, the summon tint (alpha 0) on it and everything it carries.
// Leaf: ?CreateMinion@CTCSummonSpell@@QAEXABVC3DVector@@PBV2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079ACC0, 2442 bytes (public, `ret 8`).
// Callees: CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00, CThingCreatureBase::MakeNoise 0x00662110, CThingAICreature::Create 0x00833800,
//   CTCSummonSpell::GetValidTeleportPos 0x0079AA90, CThing::GetPos 0x004C73D0, CThing::RemoveTC 0x004C9840, CThing::AddTC 0x004C9D60,
//   CTCEnemy::SetFaction 0x0076C810 / AddAllyThing 0x0076D270, CTCRegionFollower::AddFollower 0x006AEDC0, CTCSummonableCreature::
//   GetBrainName 0x0079C1C0, CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410,
//   CDefinitionManager::GetDef<CBrainDef> 0x0079BD80 (same TU, after this row), CThingAICreature::SetNewBrain 0x00833010,
//   CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580, CSpecialEffectsDef::GetEffectFromString 0x00669D30 (both header inline,
//   kept out of line: the COMDAT copies the linker kept), CTCSpecialAbilities::CreateParticleEffect 0x0072AB70, CTCDParticleEmitter::
//   AttachToThing 0x006E0BE0, NGraphicAppearance::CTCGraphicAppearance::HasDummyObject 0x004BE2C0 / SetTintColour 0x004C00B0 / SetAlpha
//   0x004BFAC0, CThingGameObject::SetAddToComboMultiplierOnHit 0x008389A0, CTCCarrying::GetAllCarriedThings 0x006A2220,
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020, CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor 0x00A01B10 /
//   0x00A01B50 / 0x00A01B90 / 0x00A01C10, CCharString 0x0099EBF0 / 0x0099EC30 / operator= 0x0099EFB0 / ~ 0x0099EAE0, _free 0x00BFEA14,
//   __ftol2 0x00BFEA70; globals CTCSummonSpell::SummonCreatureGlobalDefNum 0x0138306C, CMainGameComponent::ConstantFPS 0x01375550,
//   GFastOneOverSqrtTable 0x013CE558.
// Names: Ego_r.pdb ?CreateMinion@CTCSummonSpell@@QAEXABVC3DVector@@PBV2@@Z and ?GetValidTeleportPos@CTCSummonSpell@@AAE?AVC3DVector@@PBVCThing@@@Z
//   (the 0x0079AA90 callee: private, returns C3DVector by hidden pointer, one const CThing* argument; the brief's stand-in
//   CTCSummonSpell_GetValidSummonPos is wrong); FableWin.pdb ?SetEffect / ?SetTimer / ?SetSoundEffect / ?SetAlphaFadeOutTimer@CTCEffectOnDie
//   (inline in retail: the +0x10 / +0x14 / +0x1C / +0x18 stores; SetSoundEffect's reference parameter is why the CCharString temporary
//   is pushed by `lea` at 0x0079B36E, not by the constructor's eax).
// Layout: CTCSummonSpell (Ego_r.pdb, packed) PDef +0x0c, FadeInTimer / CurFadeInTimer / SpellLevel / CurTimer, Minion; CTCEffectOnDie
//   Effect +0x10, Timer +0x14, FadeOutTimer +0x18, DeathSoundCriteria +0x1c; CSpecialAbilitiesSummonSpellDef TimeOfSummon +0x3c,
//   FadeOutTime +0x50, SummonTintColour +0x58, EyeLeft/RightDummyPointName +0x64 / +0x68.
// Shape: the same-TU __declspec(noinline) GetSubDef / GetEffectFromString / GetDef<CBrainDef> bodies let VC7.1 see that the
//   CDefPointer locals do not escape (retail's `dec [ecx+4]` release and shared stack slots); the velocity Normalise needs the
//   X, Y, Z squared-magnitude spelling (FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ, hdr/README.md).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_BULLETTIMEOVERRIDE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_COMBOMULTIPLIER
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FLAGSETTERS
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_SETNEWBRAIN
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_SETALPHA
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_TINT
#define FABLE_RETAIL_CTCCARRYING_ALLCARRIED
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCCarrying.h"
#include "engine/CDefClassBase.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CRGBColour.h"
#include "fable_game.h"

template<> __declspec(noinline) bool CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<const CSpecialEffectsDef>* out) const
{
    const CSubDefInfo* index;
    { CCharString name("CSpecialEffectsDef",-1); index=GetPSubDefInfo(name); }
    if(index) {
        CDefPointer<const CSpecialEffectsDef> result(PDefManager->GetPDefFromGlobalIndex(index->DefIndex));
        *out=result;
        return true;
    }
    return false;
}
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name,long& result) const
{
    unsigned long key=name.GetCRC();
    std::map<unsigned long,long>::const_iterator item=SpecialEffectsMap.ContainmentMap.find(key);
    if(item!=SpecialEffectsMap.ContainmentMap.end()) { result=item->second;return true; }
    return false;
}

class CBrainDef;
class CThingBuilding;

// Same-TU instance (retail 0x0079BD80 follows this row in the TU): CDefinitionManager::GetDef<CBrainDef>, out of line.
template<> __declspec(noinline) bool CDefinitionManager::GetDef<CBrainDef>(long index, CDefPointer<const CBrainDef>* out) const
{
    if (index > 0) {
        CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
        if (def.Get()) {
            out->SetDynamicCast(def);
            return true;
        }
    }
    return false;
}

// CThingAICreatureInit: Ego_r.pdb record (28 bytes) with the PDB ctor ??0CThingAICreatureInit@@QAE@PAVCThingPhysical@@0_NABVCCharString@@21@Z
// (out of line at 0x004FACD0 in another TU; expanded here, its defaults building the two "" temporaries).
class CThingAICreatureInit {
public:
    CThingPhysical* OriginalThing;                      // +0x00
    CThingPhysical* PreviousThing;                      // +0x04
    CCharString ScriptName;                             // +0x08
    CCharString OverridingBrainName;                    // +0x0c
    bool PlayerCharacter;                               // +0x10
    bool Drawable;                                      // +0x11
    bool PlaceAtEndOfList;                              // +0x12
    bool EnableCreatureAutoPlacing;                     // +0x13
    CThingBuilding* HomeBuilding;                       // +0x14
    CThingBuilding* WorkBuilding;                       // +0x18
    CThingAICreatureInit(CThingPhysical* original = 0, CThingPhysical* previous = 0, bool playerCharacter = false,
                         const CCharString& scriptName = "", const CCharString& overridingBrainName = "", bool drawable = true)
        : OriginalThing(original), PreviousThing(previous), ScriptName(scriptName), OverridingBrainName(overridingBrainName),
          PlayerCharacter(playerCharacter), Drawable(drawable), PlaceAtEndOfList(true), EnableCreatureAutoPlacing(false),
          HomeBuilding(0), WorkBuilding(0) {}
};

class CSpecialAbilitiesSummonSpellDef : public CDefClassBase {
    unsigned char _pad_0x25[0x3c - 0x25];
public:
    std::vector<float> TimeOfSummon;                    // +0x3c
    unsigned char BaseSummonCreatureName[4];            // +0x48 CDefString
    float FadeInTime;                                   // +0x4c
    float FadeOutTime;                                  // +0x50
    long ResetCreatureManaCost;                         // +0x54
    std::vector<long> SummonTintColour;                 // +0x58
    CCharString EyeLeftDummyPointName;                  // +0x64
    CCharString EyeRightDummyPointName;                 // +0x68
};

class CTCSummonableCreature : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SUMMONABLE_CREATURE; }
    CCharString GetBrainName() const;                   // 0x0079C1C0
    unsigned char PDef[4];                              // +0x0c
    bool HasBeenSummoned;                               // +0x10
    CIntelligentPointer<CThingCreatureBase> Summoner;   // +0x11
};

class CTCEnemy : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_ENEMY; }
    void SetFaction(const CCharString& faction);        // 0x0076C810
    void AddAllyThing(const CThing& thing);             // 0x0076D270
};

class CTCRegionFollower : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_REGION_FOLLOWER; }
    void AddFollower(CThing& thing);                    // 0x006AEDC0
};

class CTCSpecialAbilities {
public:
    static CThing* __fastcall CreateParticleEffect(long effect, const C3DVector& pos);   // 0x0072AB70
};

class CTCDParticleEmitter : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_PARTICLE_EMITTER; }
    void AttachToThing(CThing& thing, long priority, const CCharString& dummy = "", long flags = 0, float delay = 0.0f);   // 0x006E0BE0
};

class CTCEffectOnDie : public CTCBase {
public:
    // FableWin.pdb inline setters (?SetEffect@CTCEffectOnDie@@QAEXJ@Z, ?SetTimer@..., ?SetSoundEffect@...@@QAEXABVCCharString@@@Z,
    // ?SetAlphaFadeOutTimer@...); inline in retail.
    void SetEffect(long effect) { Effect = effect; }
    void SetTimer(long timer) { Timer = timer; }
    void SetSoundEffect(const CCharString& criteria) { DeathSoundCriteria = criteria; }
    void SetAlphaFadeOutTimer(long timer) { FadeOutTimer = timer; }
private:
    bool StartedNonRecoverableFadeout;                  // +0x0c
    long Effect;                                        // +0x10
    long Timer;                                         // +0x14
    long FadeOutTimer;                                  // +0x18
    CCharString DeathSoundCriteria;                     // +0x1c
};

#pragma pack(push, 1)
class CTCSummonSpell : public CTCBase {
public:
    void CreateMinion(const C3DVector& pos, const C3DVector* direction);
    static long SummonCreatureGlobalDefNum;                               // 0x0138306C (bound by 0x0079A6E0 / 0x0079B650)
private:
    C3DVector GetValidTeleportPos(const CThing* thing);                     // 0x0079AA90
    CDefPointer<const CSpecialAbilitiesSummonSpellDef> PDef;              // +0x0c
    bool JustReleased;                                                    // +0x10
    CIntelligentPointer<CThingAICreature> Minion;                         // +0x11
    unsigned char _pad_0x19[3];
    long FadeInTimer;                                                     // +0x1c
    long CurFadeInTimer;                                                  // +0x20
    long SpellLevel;                                                      // +0x24
    long CurTimer;                                                        // +0x28
    CIntelligentPointer<CThing> PItemRemoved;                             // +0x2c
    bool CutSceneCancelled;                                               // +0x34
};
#pragma pack(pop)

void CTCSummonSpell::CreateMinion(const C3DVector& pos, const C3DVector* direction)
{
    CTCSoundPlayer* soundPlayer;
    if (GetParentThing().GetTC(&soundPlayer))
        soundPlayer->PlayAttachedCriteria("HERO_ABILITY_SUMMON_CREATURE_APPEAR", true, false, false, -1);
    CThingCreatureBase* caster = static_cast<CThingCreatureBase*>(&GetParentThing());
    caster->MakeNoise("SIMPLE_NOISE");
    CThingAICreatureInit init;
    Minion = CThingAICreature::Create(SummonCreatureGlobalDefNum, pos, caster->GetPlayerNumber(), init);
    C3DVector teleportPos = GetValidTeleportPos(Minion.Get());
    C3DVector velocity;
    if (direction) {
        velocity = *direction;
    } else {
        velocity = teleportPos - GetParentThing().GetPos();
        velocity.Normalise();
    }
    CTCPhysicsBase* physics = Minion->GetPhysicsTC();
    if (physics) {
        physics->SetPosition(teleportPos);
        physics->SetFacingAnglesFromRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), -velocity));
    }
    Minion->SetScriptName("HeroSummonnedCreature");
    Minion->SetIsAffectedByBulletTimeOverride(false);
    Minion->RemoveTC(TCI_DECAPITATION);
    CurFadeInTimer = FadeInTimer;
    CTCEnemy* enemy;
    Minion->GetTC(&enemy);
    enemy->SetFaction("FACTION_HERO");
    enemy->AddAllyThing(*caster);
    CTCSummonableCreature* summonable;
    Minion->GetTC(&summonable);
    summonable->HasBeenSummoned = true;
    summonable->Summoner = caster;
    CTCRegionFollower* follower;
    if (caster->GetTC(&follower))
        follower->AddFollower(*Minion.Get());
    Minion->SetAsFollowingHeroThroughTeleporters(true);
    if (!Minion->HasTC(TCI_PARTICLE_ATTACHMENT_TRACKER))
        Minion->AddTC("CTCParticleAttachmentTracker", false, 0);
    CCharString brainName = summonable->GetBrainName();
    CGameDefinitionManager& defs = CGameDefinitionManager::Get();
    CDefPointer<const CBrainDef> brain;
    defs.GetDef(defs.GetDefGlobalIndexFromName(brainName), &brain);
    Minion->SetNewBrain(brain);
    CDefPointer<const CSpecialEffectsDef> effects;
    long effect = 0;
    if (PDef->GetSubDef(&effects)) {
        if (effects->GetEffectFromString("CREATURE_APPEARING", effect))
            CTCSpecialAbilities::CreateParticleEffect(effect, teleportPos);
        if (effects->GetEffectFromString("CREATURE_ALIVE_EFFECT", effect)) {
            CThing* particle = CTCSpecialAbilities::CreateParticleEffect(effect, teleportPos);
            if (particle && !particle->IsDead()) {
                CTCDParticleEmitter* emitter;
                particle->GetTC(&emitter);
                if (emitter)
                    emitter->AttachToThing(*Minion.Get(), 0x2a);
            }
        }
        if (effects->GetEffectFromString("CREATURE_ON_DIE_EFFECT", effect)) {
            CTCEffectOnDie* onDie = (CTCEffectOnDie*)Minion->AddTC("CTCEffectOnDie", true, 0);
            onDie->SetEffect(effect);
            onDie->SetTimer(CMainGameComponent::ConvertSecondsToWF(PDef->TimeOfSummon[SpellLevel]) - CurTimer);
            onDie->SetSoundEffect("HERO_ABILITY_SUMMON_CREATURE_DISAPPEAR");
            onDie->SetAlphaFadeOutTimer(CMainGameComponent::ConvertSecondsToWF(PDef->FadeOutTime));
        }
        if (effects->GetEffectFromString("GLOWING_EYES", effect)) {
            NGraphicAppearance::CTCGraphicAppearance* appearance = Minion->GetTCGraphicAppearance();
            if (appearance) {
                if (appearance->HasDummyObject(PDef->EyeLeftDummyPointName)) {
                    CThing* particle = CTCSpecialAbilities::CreateParticleEffect(effect, Minion->GetPos());
                    if (particle && !particle->IsDead()) {
                        CTCDParticleEmitter* emitter;
                        particle->GetTC(&emitter);
                        emitter->AttachToThing(*Minion.Get(), 10, PDef->EyeLeftDummyPointName);
                    }
                }
                if (appearance->HasDummyObject(PDef->EyeRightDummyPointName)) {
                    CThing* particle = CTCSpecialAbilities::CreateParticleEffect(effect, Minion->GetPos());
                    if (particle && !particle->IsDead()) {
                        CTCDParticleEmitter* emitter;
                        particle->GetTC(&emitter);
                        emitter->AttachToThing(*Minion.Get(), 10, PDef->EyeRightDummyPointName);
                    }
                }
            }
        }
    }
    Minion->SetLeaveDeadBodyOverride(false);
    Minion->SetLeaveExperienceOrbsOverride(false);
    Minion->SetAddToComboMultiplierOnHit(false);
    CRGBColour tint(PDef->SummonTintColour[0], PDef->SummonTintColour[1], PDef->SummonTintColour[2], PDef->SummonTintColour[3]);
    NGraphicAppearance::CTCGraphicAppearance* appearance = Minion->GetTCGraphicAppearance();
    if (appearance) {
        appearance->SetTintColour(tint, this);
        appearance->SetAlpha(0);
    }
    CTCCarrying* carrying;
    if (Minion->GetTC(&carrying)) {
        std::vector<CThing*> carried;
        carrying->GetAllCarriedThings(carried);
        for (std::vector<CThing*>::iterator it = carried.begin(), end = carried.end(); it != end; ++it) {
            NGraphicAppearance::CTCGraphicAppearance* carriedAppearance = (*it)->GetTCGraphicAppearance();
            if (carriedAppearance)
                carriedAppearance->SetTintColour(tint, this);
        }
    }
}