// Grade: DIFFER(5189v5195) -- full body, every call target and branch in retail order; 267 register-masked diff lines (hist.py), 1649 vs 1646 instructions, calls 175 vs 182
// Residue: c2 register colouring: retail colours this=ebp / hitter=ebx early (ebp=hitter late, this spilled to [esp+24h]), mine this=ebx / hitter=ebp (c2prio: this split edi/ebp/esi/ebx, hitter split 7 pieces); frame 0xA4 vs 0xA0; six late HasTC sites retail calls std::bitset<274>::test 0x00410DE0 out of line (0x68,0x42,0x84,0x77,0x10,0x7F) and GetPHitMaterialDef out of line only at the sound site (/d2inlS3/S8/S15/T20/T50/T60 none reproduces: T<=50 outlines 25-27 sites); return tails: retail cross-jumps the electrocute return into the battle-charge tail (0x83AD60), mine into the normal end; sound block keeps 0 in ebx in retail
// Next: c2prio --trace the early this/hitter pieces and find a spelling that colours hitter before this (e.g. the hit-location block as a helper, hitter re-read from hit); per-site inline budget for the six late test() calls (scoped #pragma inline_depth / a non-inline HasTC overload at those sites only, once the allocation matches)
// CThingGameObject::OnHit(CHitParameters& hit) -- apply a hit to a game object: debug lines, hit location and material, perception, door / electrocution / explosion / fire reactions, damage and kill credit, hero stats, entity events, rumble, collision sound, hit effect
// Leaf: ?OnHit@CThingGameObject@@UAEXAAVCHitParameters@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_LOADSINGLE
#define FABLE_RETAIL_CTHING_VIRTUALS
#define FABLE_RETAIL_CTHINGPHYSICAL_VIRTUALS
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CHITPARAMETERS_EFFECTONHIT
#define FABLE_RETAIL_CHITPARAMETERS_ONHIT
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHINGMANAGER_COLLISIONDEBUG
#define FABLE_RETAIL_CPLAYER_GETNUMBER
#define FABLE_RETAIL_CTHINGCREATUREBASE_BLOCKEDREACTION
#define FABLE_RETAIL_CTHINGGAMEOBJECT_HITQUERIES
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCDEXPLOSION_ONHIT
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CCLASSFACTORY
#include "engine/CThingCreatureBase.h"
#include "engine/CHitParameters.h"
#include "engine/CDefClassBase.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitive.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CIEngine.h"
#include "engine/CDisplayEngine.h"
#include "engine/CMainGameComponent.h"
#include "engine/CThingManager.h"
#include "engine/EHeroExperienceStatCategory.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CCombatManager.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCDExplosion.h"
#include "engine/CTCOwnedEntity.h"
#include "stlport/fable_map.h"

extern const float DELTA_NOTIONAL_ZERO;                                  // 0x0129BA3C
template<class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing) { return thing && !thing->IsDead(); }   // 0x0040DCE0

class CEnginePrimitiveLine : public CEnginePrimitive {
public:
    CEnginePrimitiveLine(unsigned __int64 id, const C3DVector& start, const C3DVector& end, const CRGBColour& colour, float width,
                         long blendMode, unsigned long flags, bool depthTest, long interpolation, long blendOp);   // 0x004C3090
    unsigned char Body[0x30];
};
namespace {
CEnginePrimitiveHandle hit_pos;                                          // 0x013BAF44 (Ego_r / FableWin "dynamic initializer for 'hit_pos'")
CEnginePrimitiveHandle hit_dir;                                          // 0x013BAF4C
}

class CMaterialDef : public CDefClassBase {
public:
    unsigned char _pad[0x44 - sizeof(CDefClassBase)];
    float OnHitRumbleLevel;                                              // +0x44
    float OnHitRumbleFalloff;                                            // +0x48
};
class CArmourDef : public CDefClassBase {
public:
    unsigned char _pad[0x64 - sizeof(CDefClassBase)];
    long DamageMaterial;                                                 // +0x64
    long BlockMaterial;                                                  // +0x68
};
class CCreatureStatsDef;
class CHeroStatIncreaseData {
public:
    std::map<EHeroExperienceStatCategory,float> StatIncreases;
};
class CHeroExperienceDef : public CDefClassBase {
public:
    unsigned char _pad[0x9c - sizeof(CDefClassBase)];
    std::map<EDamageAttribute,CHeroStatIncreaseData> StatIncreasePerHitType;   // +0x9C
};
class CCreatureDef : public CDefClassBase {
public:
    unsigned char _pad[0x40 - sizeof(CDefClassBase)];
    CCreatureStatsDef* PStats;                                           // +0x40 CCountedPointer<CCreatureStatsDef> data word
    const CCreatureStatsDef& GetStats() const { return *PStats; }
};
namespace NHitLocations {
class CHitLocation {
    CThing* POwnerThing;                                                 // +0x00
    CDefPointer<const CDefClassBase> PDef;                               // +0x04 CDefPointer<const CHitLocationDef>
    CDefPointer<const CArmourDef> PArmourDef;                            // +0x08
public:
    const CDefPointer<const CArmourDef>& GetArmourDef() const { return PArmourDef; }   // FableWin ?GetArmourDef@CHitLocation@NHitLocations@@QBEABV?$CDefPointer@$$CBVCArmourDef@@@@XZ
    bool IsToPlayGetHitResponse() const;                                 // 0x00815270
    void ProcessHitLocationCode(CThing& thing, const CHitParameters& hit);   // 0x00815E80
};
}
namespace NThingShot {
class CTCShotDefinitionBase : public CTCBase { public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SHOT_DEFINITION; }
    CThing* GetPWeapon() const;                                          // 0x007BEFF0
};
}
#define TC_CLASS(Name, Id) class Name : public CTCBase { public: static ETCInterfaceType __fastcall StatGetInterfaceType() { return Id; }
TC_CLASS(CTCHitLocations, TCI_HIT_LOCATIONS)  NHitLocations::CHitLocation* GetPDefaultHitLocation(); };             // 0x00814BE0
TC_CLASS(CTCPerceiveThing, TCI_PERCEIVE_THING)       void OnHit(const CThing& hitter); };                         // 0x00744CB0
TC_CLASS(CTCElectrocuteOnTouch, TCI_ELECTROCUTE_ON_TOUCH) void OnHit(CThing& hitter); };                          // 0x007E2600
TC_CLASS(CTCHeroExperience, TCI_HERO_EXPERIENCE)
    bool IsAbleToBreakDownDoors() const;                                 // 0x006D6370
    void AddSuccessfulBlockParry();                                      // 0x006D7550
    void SetAsHit();                                                     // 0x006D80E0
    void AddSuccessfulStrikeWithHitParams(const CHitParameters& hit);    // 0x006D8D00
};
TC_CLASS(CTCWeapon, TCI_WEAPON)
    long GetExplosionType() const;                                       // 0x005DA830
    void IncrementNumThingsKilled(CThing* thing);                        // 0x005DA680
};
TC_CLASS(CTCFlammable, TCI_FLAMMABLE)  bool IsOnFire() const; void SetOnFire(); };                                 // 0x00780400 / 0x00780410
TC_CLASS(CTCBerserk, TCI_BERSERK)      void IncreaseBerserkDamageToMeMultiplier(); };                              // 0x0078BEC0
TC_CLASS(CTCCombat, TCI_COMBAT)
    void SetStruck(const NHitLocations::CHitLocation& location, CHitParameters& hit);   // 0x00734C60
    bool IsBlocking() const;                                             // 0x007331E0
    void UnsetBlocking();                                                // 0x007338A0
    void OnHit(const CThing& hitter);                                    // 0x007332D0
};
class CTCEmoteIcon : public CTCBase { public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_EMOTEICON; }
    enum EEmotePriority { };
    void SetEmoteIcon(EEmotePriority priority, long icon, float time, float scale, unsigned char alpha);   // 0x0071AAB0
};
class CSpecialAbilitiesMultiStrikeDef; class CSummonableCreatureDef;
TC_CLASS(CTCMultiStrike, TCI_MULTI_STRIKE)
    bool IsStrikeStarted() const { return StrikeStarted; }               // FableWin, inline (+0x14)
    void StartMultiHit(const CHitParameters& hit, CThing* target);       // 0x007CCB00
private:
    CDefPointer<const CSpecialAbilitiesMultiStrikeDef> PDef;             // +0x0C
    long SpellLevel;                                                     // +0x10
    bool StrikeStarted;                                                  // +0x14
};
TC_CLASS(CTCGuard, TCI_GUARD)          bool IsAttackingToFineRatherThanKill() const; };                           // 0x007B2CF0
TC_CLASS(CTCHero, TCI_HERO)
    bool IsWeaponUseAllowed() const { return WeaponsUsable; }            // FableWin, inline (+0x0C)
private:
    bool WeaponsUsable;                                                  // +0x0C
};
class CTCVillage : public CTCBase { public:
    unsigned char _pad[0xb3 - 0xc];
    bool LaggyLimbo;                                                     // +0xB3
    bool IsLaggyLimbo() const { return LaggyLimbo; }
};
TC_CLASS(CTCVillageMember, TCI_VILLAGE_MEMBER) CTCVillage* GetPVillage() const; };                               // 0x007EE540
TC_CLASS(CTCAbsorbHits, TCI_ABSORB_HITS)
    long DecrementHitsRemaining() { return --HitsRemaining; }            // FableWin, inline
private:
    long HitsRemaining;                                                  // +0x0C
};
TC_CLASS(CTCWound, TCI_WOUND)          bool AddPossibleWoundGivenStrikePoint(const C3DVector& position); };       // 0x00757F90
class CTCDegradableBase : public CTCBase { public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_DEGRADABLE; }
    virtual void OnHitByMelee(const C3DVector&, const C3DVector&, CThing&);   // slot 28
    virtual void OnHitByShot(const C3DVector&, const C3DVector&, CThing&);    // slot 29
    virtual void OnHit(const C3DVector& position, const C3DVector& direction); // slot 30 (+0x78)
};
class CThingAICreature; class CFaction;
enum EOpinionDeedType { };
TC_CLASS(CTCHeroOpinionDeedLog, TCI_HERO_OPINION_DEED_LOG)
    enum EOpinionPostDeedType { };
    long PostDeed(EOpinionDeedType type, const C3DVector& position, const CThing* victim, CThingAICreature* witness,
                  EOpinionPostDeedType postType, bool a6, bool a7, const CFaction* faction, bool a9, long a10);   // 0x006C6990
};
TC_CLASS(CTCSummonableCreature, TCI_SUMMONABLE_CREATURE)
    bool GetHasBeenSummoned() const { return HasBeenSummoned; }          // FableWin, inline (+0x10)
private:
    CDefPointer<const CSummonableCreatureDef> PDef;                      // +0x0C
    bool HasBeenSummoned;                                                // +0x10
};
class CTCGhostSword { public: static bool __fastcall IsThingGhostSword(const CThing* thing); };   // 0x007C53F0
class CTCHeroStats;
namespace NHeroStatModifierEvent {
class CBase {
public:
    CBase();
    ~CBase() { }
    virtual void ModifyHeroStats(CTCHeroStats& stats) const = 0;
    const CCreatureStatsDef* PStats;                                     // +0x04
};
class CHitThing : public CBase {
public:
    CHitThing(const CCreatureStatsDef& stats);                           // 0x0083AE30
    virtual void ModifyHeroStats(CTCHeroStats& stats) const;
    void SetAsUnarmed(bool value) { Unarmed = value; }
    void SetStatToIncrement(EHeroExperienceStatCategory stat) { Stat = stat; }
    void SetStatIncrementAmount(float amount) { Amount = amount; }
private:
    bool Unarmed;                                                        // +0x08
    EHeroExperienceStatCategory Stat;                                    // +0x0C
    bool Flag10;                                                         // +0x10
    float Amount;                                                        // +0x14
};
}
TC_CLASS(CTCHeroStats, TCI_HERO_STATS) void InformOfEvent(const NHeroStatModifierEvent::CBase& event); };      // 0x0057A56C
namespace NEntityEvents {
class CEventBase { public: virtual ~CEventBase(); long WorldFrame; };
class CEventHitBy : public CEventBase { public: CEventHitBy(const CHitParameters& hit); unsigned char Body[8]; };   // 0x0083AF30
enum EHeroAbility { };
class CEventHitBySpecialAbility : public CEventBase { public:
    CEventHitBySpecialAbility(const CHitParameters& hit, EHeroAbility ability); unsigned char Body[16]; };          // 0x00811830
}
TC_CLASS(CTCEntityEvents, TCI_ENTITY_EVENTS) void AddEvent(NEntityEvents::CEventBase* event); };               // 0x008113C0
TC_CLASS(CTCSoundPlayer, TCI_SOUND_PLAYER)
    void PlayCollisionSound(const C3DVector& position, const CDefPointer<const CMaterialDef>& mine,
                            const CDefPointer<const CMaterialDef>& other);   // 0x0066A9E0
};
class CJoystick { public:
    class CInitRumble { public:
        CInitRumble(float maxLevel, float smoothness, float attackTime, float decayTime, bool cumulative);   // 0x006A92B0
        float MaxLevel, Smoothness, AttackTime, DecayTime; bool Cumulative; };
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8();
    virtual void v9(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void AddRumble(long id, const CInitRumble& rumble, bool replace);   // slot 17
};
class CInputManager { public: unsigned char _pad[0x2c]; CCountedPointer<CJoystick>* PJoysticks; };
extern CInputManager* PInputManager;                                     // 0x013B8388
class CPlayerGui { public: static CPlayerGui* PPlayerGui; void AddDamageChange(float change, bool mine); };   // 0x013B8790, 0x00437940
class CThingCreatureBaseView;
namespace NThingHelpers { void __fastcall CreateEffectOnHit(const CThing& thing, const CHitParameters& hit, const CWorldMap& map, float damage); }   // 0x00839420

void CThingGameObject::OnHit(CHitParameters& hit)
{
    // Debug: a red line up from the hit position, a green one along the hit direction.
    if (PeekPCurrentThingManager()->IsDrawCombatCollisionDebug() && hit.IsLocalised()) {
        C3DVector position = hit.GetHitPosition();
        {
            CIEngine& engine = CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine();
            engine.SetPersistentPrimitive(hit_pos,
                CEnginePrimitiveLine(0, position, C3DVector(position.X, position.Y, position.Z + 0.1f), CRGBColour(255, 0, 0, 255),
                                     0.05f, 0, 0, true, 0, 0), 0);
        }
        if (hit.IsDirectional()) {
            C3DVector direction = hit.GetHitDirection();
            CIEngine& engine = CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine();
            engine.SetPersistentPrimitive(hit_dir,
                CEnginePrimitiveLine(0, position, direction + position, CRGBColour(0, 255, 0, 255),
                                     0.05f, 0, 0, true, 0, 0), 0);
        }
    }
    RemoveTC(TCI_FLOURISH_TARGET);
    CThing* weapon = hit.GetPWeapon();
    CThing* hitter = hit.GetPThingHitting();
    NHitLocations::CHitLocation* location = hit.GetPHitLocation();
    if (!location) {
        CTCHitLocations* hitLocations;
        if (!GetTC(&hitLocations))
            return;
        location = hitLocations->GetPDefaultHitLocation();
        if (!location)
            return;
        hit.SetHitLocation(*location);
    }
    hit.SetHitLocation(*location);
    CDefPointer<const CMaterialDef> material = PeekPMaterial();
    if (GetTypeFlags() & 8) {
        long index = location->GetArmourDef()->DamageMaterial;
        if (index > 0)
            PeekDefinitionManager().GetDef(index, &material);
    }
    hit.SetPHitMaterialDef(material);
    CTCPerceiveThing* perceive;
    if (hitter && !hitter->IsDead() && GetTC(&perceive))
        perceive->OnHit(*hitter);
    if (hit.IsMeleeStrike()) {
        if (hitter && !hitter->IsDead()) {
            if (hitter->GetTypeFlags() & 8) {
                static_cast<CThingCreatureBase*>(hitter)->MakeNoise(CCharString("SIMPLE_NOISE", -1));
                CTCElectrocuteOnTouch* electrocute;
                if (GetTC(&electrocute)) {
                    electrocute->OnHit(*hitter);
                    hit.SetHitNegated(true);
                    return;
                }
            }
            if (!hitter->IsDead()) {
                bool door;
                if (HasTC(TCI_DOOR))
                    door = true;
                else {
                    const CTCBase* use;
                    if (PeekTC(TCI_ON_ACTION_USE, &use))
                        door = use->GetClassTypeName() == CCharString("CTCActionUseWindow", -1);
                    else
                        door = false;
                }
                const CTCHeroExperience* experience;
                if (door && hitter->PeekTC(&experience) && !experience->IsAbleToBreakDownDoors())
                    hit.SetHitNegated(true);
            }
        }
        if (weapon && !weapon->IsDead()) {
            CTCWeapon* weaponTC;
            if (hitter && !hitter->IsDead() && weapon->GetTC(&weaponTC)) {
                long type = weaponTC->GetExplosionType();
                if (type > 0) {
                    CThing* explosion = CTCDExplosion::Create(type, hitter->GetPos());
                    if (IS_THING_ALIVE(explosion)) {
                        CTCDExplosion* driver;
                        explosion->GetTCDriver(&driver);
                        driver->SetSpecificThingToHit(*this);
                    }
                }
            }
            CTCFlammable* mine;
            CTCFlammable* theirs;
            if (!weapon->IsDead() && GetTC(&mine) && weapon->GetTC(&theirs)) {
                if (mine->IsOnFire() && !theirs->IsOnFire())
                    theirs->SetOnFire();
                else if (!mine->IsOnFire() && theirs->IsOnFire())
                    mine->SetOnFire();
            }
        }
    }
    if (HasTC(TCI_BATTLE_CHARGE) || HasTC(TCI_ASSASSIN_RUSH)) {
        hit.SetPlayHitResponse(false);
        return;
    }
    CTCBerserk* berserk;
    if (GetTC(&berserk))
        berserk->IncreaseBerserkDamageToMeMultiplier();
    CTCCombat* combat = 0;
    if (GetTC(&combat))
        combat->SetStruck(*location, hit);
    float damage = 0.0f;
    if (hit.IsBlocked()) {
        CTCHeroExperience* experience;
        if (GetTC(&experience))
            experience->AddSuccessfulBlockParry();
    } else {
        if (IsToNegateAllHits()) {
            hit.SetHitNegated(true);
            if (GetTypeFlags() & 8) {
                long index = location->GetArmourDef()->BlockMaterial;
                if (index > 0) {
                    CDefPointer<const CMaterialDef> blockMaterial;
                    PeekDefinitionManager().GetDef(index, &blockMaterial);
                    hit.SetPHitMaterialDef(blockMaterial);
                }
            }
        }
        if (combat && combat->IsBlocking())
            combat->UnsetBlocking();
        CTCEmoteIcon* emote;
        if (GetTC(&emote))
            emote->SetEmoteIcon((CTCEmoteIcon::EEmotePriority)8, 0, 0.0f, 1.0f, 0xFF);
        if (!location->IsToPlayGetHitResponse())
            hit.SetPlayHitResponse(false);
        CDefPointer<const CArmourDef> armour = location->GetArmourDef();
        hit.SetAsModifiedByArmour(*this, armour);
        if (hit.IsHitNegated()) {
            hit.SetPlayHitResponse(false);
            hit.SetToCauseRecoil(true);
        }
        damage = hit.GetDamageToThing(*this);
        if (!hit.IsHitNegated()) {
            CTCMultiStrike* multiStrike;
            if (hit.IsMeleeStrike() && ((GetTypeFlags() & 8) || IsDamageable()) && IS_THING_ALIVE(weapon) && weapon != hitter
                && IS_THING_ALIVE(hitter) && hitter->GetTC(&multiStrike) && !multiStrike->IsStrikeStarted())
                multiStrike->StartMultiHit(hit, this);
            bool playResponse = true;
            CTCGuard* guard;
            const CTCHero* hero;
            if (GetType() == 2 && IS_THING_ALIVE(hitter) && hitter->GetTC(&guard)) {
                if (guard->IsAttackingToFineRatherThanKill())
                    playResponse = false;
            } else if ((GetTypeFlags() & 8) && IS_THING_ALIVE(hitter) && hitter->PeekTC(&hero) && !hero->IsWeaponUseAllowed())
                playResponse = false;
            CTCOwnedEntity* owned;
            if (HasTC(TCI_DOOR) && GetTC(&owned)) {
                CThing* owner = owned->GetPOwner();
                CTCVillageMember* member;
                if (owner && owner->GetTC(&member)) {
                    CTCVillage* village = member->GetPVillage();
                    if (village && village->IsLaggyLimbo())
                        playResponse = false;
                }
            }
            bool wasDead = GetHealth() < DELTA_NOTIONAL_ZERO;
            CTCAbsorbHits* absorb;
            if (!GetTC(&absorb) || absorb->DecrementHitsRemaining() < 0)
                ModifyHealth(damage, playResponse);
            if (!wasDead && GetHealth() < DELTA_NOTIONAL_ZERO) {
                CThing* killer = weapon;
                const NThingShot::CTCShotDefinitionBase* shot;
                if (IS_THING_ALIVE(killer) && killer->PeekTC(&shot))
                    killer = shot->GetPWeapon();
                CTCWeapon* killerWeapon;
                if (IS_THING_ALIVE(killer) && killer->GetTC(&killerWeapon))
                    killerWeapon->IncrementNumThingsKilled(this);
            }
            if ((GetTypeFlags() & 8) && static_cast<CThingCreatureBase*>(this)->IsInMeleeAsAttacker())
                static_cast<CThingCreatureBase*>(this)->GetCombatManager().DeregisterCombatant(*static_cast<CThingCreatureBase*>(this));
            if ((GetTypeFlags() & 8) || HasTC(TCI_CREATURE_GENERATOR)) {
                float change = -damage;
                if (IsDamageable() && change > DELTA_NOTIONAL_ZERO) {
                    if (GetPlayerManager().IsThingMainPlayer(*this))
                        CPlayerGui::PPlayerGui->AddDamageChange(change, true);
                    else if (IS_THING_ALIVE(hitter) && GetPlayerManager().IsThingMainPlayer(*hitter))
                        CPlayerGui::PPlayerGui->AddDamageChange(change, false);
                }
            }
            if (damage < 0.0f) {
                CTCHeroExperience* experience;
                if (GetTC(&experience))
                    experience->SetAsHit();
                if (hit.IsLocalised()) {
                    CTCWound* wound;
                    if (GetTC(&wound))
                        wound->AddPossibleWoundGivenStrikePoint(hit.GetHitPosition());
                    CTCDegradableBase* degradable;
                    if (hit.IsDirectional() && GetTC(&degradable))
                        degradable->OnHit(hit.GetHitPosition(), hit.GetHitDirection());
                }
            }
        } else {
            CTCHeroOpinionDeedLog* deeds;
            if (hit.IsMeleeStrike() && IS_THING_ALIVE(hitter) && !(GetTypeFlags() & 8) && hitter->GetTC(&deeds))
                deeds->PostDeed((EOpinionDeedType)0x49, GetPos(), this, 0, (CTCHeroOpinionDeedLog::EOpinionPostDeedType)2,
                                false, false, 0, false, 0);
        }
        if (hitter && !hitter->IsDead() && combat)
            combat->OnHit(*hitter);
        location->ProcessHitLocationCode(*this, hit);
    }
    CTCHeroStats* stats;
    if (!hit.IsHitNegated() && !hit.IsBlocked() && hitter && !hitter->IsDead() && hitter->GetTC(&stats) && IsGivingHeroStatChangesOnBeingHit()) {
        CDefPointer<const CCreatureDef> creatureDef;
        if (GetPBaseDef()->GetSubDef(&creatureDef)) {
            bool unarmed = true;
            if (IS_THING_ALIVE(weapon) && weapon != hitter)
                unarmed = false;
            CDefPointer<const CHeroExperienceDef> heroDef;
            if (hitter->GetPBaseDef()->GetSubDef(&heroDef)) {
                for (std::map<EDamageAttribute,float>::const_iterator it = hit.GetDamageMap().begin(); it != hit.GetDamageMap().end(); ++it) {
                    const EDamageAttribute attribute = it->first;
                    std::map<EDamageAttribute,CHeroStatIncreaseData>::const_iterator found = heroDef->StatIncreasePerHitType.find(attribute);
                    if (found != heroDef->StatIncreasePerHitType.end()) {
                        for (std::map<EHeroExperienceStatCategory,float>::const_iterator inc = found->second.StatIncreases.begin();
                             inc != found->second.StatIncreases.end(); ++inc) {
                            NHeroStatModifierEvent::CHitThing event(creatureDef->GetStats());
                            event.SetAsUnarmed(unarmed);
                            event.SetStatToIncrement(inc->first);
                            event.SetStatIncrementAmount(inc->second);
                            bool summoned = false;
                            CTCSummonableCreature* summonable;
                            if (GetTC(&summonable) && summonable->GetHasBeenSummoned())
                                summoned = true;
                            bool ghostSword = CTCGhostSword::IsThingGhostSword(this);
                            if (!summoned && !ghostSword)
                                stats->InformOfEvent(event);
                        }
                    }
                }
            }
        }
    }
    CTCHeroExperience* experience;
    if (!hit.IsHitNegated() && !hit.IsBlocked() && IS_THING_ALIVE(hitter) && hitter->GetTC(&experience) && IsAddToComboMultiplierOnHit())
        experience->AddSuccessfulStrikeWithHitParams(hit);
    CTCEntityEvents* events;
    if (GetTC(&events) && hitter && !hitter->IsDead()) {
        events->AddEvent(new NEntityEvents::CEventHitBy(hit));
        if (hitter->HasTC(TCI_BERSERK))
            events->AddEvent(new NEntityEvents::CEventHitBySpecialAbility(hit, (NEntityEvents::EHeroAbility)8));
    }
    if (hit.IsToCauseRecoil() && hitter && !hitter->IsDead() && (hitter->GetTypeFlags() & 8)
        && !(IS_THING_ALIVE(weapon) && weapon->HasTC(TCI_SHOT_DEFINITION)))
        static_cast<CThingCreatureBase*>(hitter)->PerformBlockedReaction(0, true);
    if (hit.IsMeleeStrike()) {
        CDefPointer<const CMaterialDef> hitMaterial = hit.GetPHitMaterialDef();
        if (hitMaterial.Get() && hitMaterial->OnHitRumbleLevel != 0.0f && IS_THING_ALIVE(hitter) && hitter->GetType() == 2
            && hitter->PeekPlayer().GetNumber() == PeekPlayerManager().GetMainPlayer().GetNumber())
            PInputManager->PJoysticks[hitter->GetJoystickDeviceNumber()]->AddRumble(3,
                CJoystick::CInitRumble(hitMaterial->OnHitRumbleLevel, 0.5f, 0.0f, hitMaterial->OnHitRumbleFalloff, false), true);
    }
    CTCSoundPlayer* sound;
    if (GetTC(&sound) && hit.IsLocalised()) {
        CDefPointer<const CMaterialDef> other;
        if (IS_THING_ALIVE(weapon) && (weapon->GetTypeFlags() & 4))
            other = static_cast<CThingPhysical*>(weapon)->PeekPMaterial();
        if (!other.Get() && IS_THING_ALIVE(hitter) && (hitter->GetTypeFlags() & 4)) {
            other = static_cast<CThingPhysical*>(hitter)->PeekPMaterial();
            if (hit.IsLightningStrike())
                PeekDefinitionManager().GetDef(CCharString("MATERIAL_LIGHTNING", -1), &other);
            else if (hit.IsDivineWrathStrike())
                PeekDefinitionManager().GetDef(CCharString("MATERIAL_DIVINE_WRATH", -1), &other);
            else if (hit.IsUnholyPowerStrike())
                PeekDefinitionManager().GetDef(CCharString("MATERIAL_UNHOLY_POWER", -1), &other);
            else {
                CTCHitLocations* hitLocations;
                if ((hitter->GetTypeFlags() & 8) && hitter->GetTC(&hitLocations))
                    PeekDefinitionManager().GetDef(hitLocations->GetPDefaultHitLocation()->GetArmourDef()->DamageMaterial, &other);
            }
        }
        CDefPointer<const CMaterialDef> mine = hit.GetPHitMaterialDef();
        if (mine.Get())
            sound->PlayCollisionSound(hit.GetHitPosition(), mine, other);
    }
    NThingHelpers::CreateEffectOnHit(*this, hit, PeekWorldMap(), damage);
}
