#ifndef FABLE_ENGINE_CHITPARAMETERS_H
#define FABLE_ENGINE_CHITPARAMETERS_H
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#ifdef FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR_BODY
// The visible constructor body needs the real member types it initialises (see the opt-in below).
#ifndef FABLE_RETAIL_CHITPARAMETERS_VECTOR_MEMBERS
#define FABLE_RETAIL_CHITPARAMETERS_VECTOR_MEMBERS
#endif
#ifndef FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP
#define FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP
#endif
#ifndef FABLE_RETAIL_CHITPARAMETERS_AUGMENTATIONS
#define FABLE_RETAIL_CHITPARAMETERS_AUGMENTATIONS
#endif
#endif
#ifdef FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR
#include "engine/EDamageAttribute.h"
#endif
namespace NHitLocations { class CHitLocation; }
class CMaterialDef;class CThing;
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
class CArmourDef;class CThingGameObject;
#endif
#if defined(FABLE_RETAIL_CHITPARAMETERS_SETASMODIFIEDBYARMOUR) && !defined(FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT)
class CArmourDef;                          // NF67 opt-in below: SetAsModifiedByArmour's parameter type
#endif
// Original Ego_r TPI0x2af3a; native OnHit proves offsets0,1,2,e,1a..28,48,50,58.
// STLport replaces both 16-byte donor containers with12-byte containers: native sizeof60.
#if defined(FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP) || defined(FABLE_RETAIL_CHITPARAMETERS_SPECIALABILITY)
#include "engine/EDamageAttribute.h"
#endif
#if defined(FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP) || defined(FABLE_RETAIL_CHITPARAMETERS_LOCATIONCTOR) || defined(FABLE_RETAIL_CHITPARAMETERS_ADDDAMAGE)
#include "engine/EDamageAttribute.h"
#endif
#ifdef FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP
#include "stlport/fable_map.h"
#endif
#ifdef FABLE_RETAIL_CHITPARAMETERS_AUGMENTATIONS
#include "engine/EObjectAugmentationType.h"
#include "stlport/fable_containers.h"
#endif
#ifdef FABLE_RETAIL_CHITPARAMETERS_VECTOR_MEMBERS
// Retail opt-in (row 0x00854DD0, for the visible body of the constructor 0x00789238): Position / Direction as the real C3DVector
// members (Ego_r.pdb CHitParameters: C3DVector Position +2, Direction +0x0E; landed row CHitParameters_CHitParameters_00789238.cpp
// models them the same way). They sit at unaligned offsets, so the record is byte-packed with the explicit three pad bytes
// before ShotBuildupTime (+0x2C); every other offset and sizeof 0x60 are unchanged (CheckRetailVectorLayout below).
#pragma pack(push, 1)
#endif
class CHitParameters {
#ifdef FABLE_ONHIT_TEST_ACCESS
    friend class COnHitFixture;
#endif

private:
    bool Localised,Directional;
#ifdef FABLE_RETAIL_CHITPARAMETERS_VECTOR_MEMBERS
    C3DVector Position;         // +0x02
    C3DVector Direction;        // +0x0E
#else
    unsigned char Position[12]; // Original C3DVector, deliberately unaligned at+2.
    unsigned char Direction[12]; // Original C3DVector, deliberately unaligned at+e.
#endif
    bool KnockDown,Decapitate,Blockable,Flourish,EpicSpell,BlockCounter;
    bool PlayHitResponse,PlayHitResponseOverrideSet,MoveBack;
    bool CreateParticleEffectOnHit,CreateDustParticleEffectOnHit,GuaranteeHit;
    bool Blocked,HitNegated,CauseRecoil;
#ifdef FABLE_RETAIL_CHITPARAMETERS_VECTOR_MEMBERS
    unsigned char _pad29[3];    // natural alignment of ShotBuildupTime, explicit under pack(1)
#endif
    float ShotBuildupTime;
#ifdef FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP
    std::map<EDamageAttribute,float> DamageMap;
#else
    unsigned char DamageMap[12]; // Original std::map<EDamageAttribute,float>.
#endif
#ifdef FABLE_RETAIL_CHITPARAMETERS_AUGMENTATIONS
    // Retail opt-in (row 0x00868CE0 CHitParameters::SetWeapon): the real member type, Ego_r.pdb
    // std::vector<EObjectAugmentationType> (12-byte STLport vector at +0x3C; push_back expanded inline at 0x00868DCC..0x00868DFC).
    std::vector<EObjectAugmentationType> Augmentations;
#else
    unsigned char Augmentations[12]; // Original std::vector<EObjectAugmentationType>.
#endif
    NHitLocations::CHitLocation* PHitLocation;
    CDefPointer<const CMaterialDef> PHitMaterialDef;
    CIntelligentPointer<CThing> PThingHitting,PWeapon;
public:
#ifdef FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP
    const std::map<EDamageAttribute,float>& GetDamageMap()const{return DamageMap;}
#endif
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
// Evidence: both original PDB type records and donor OnHit/helper disassembly.
public:
#ifdef FABLE_RETAIL_COMBAT_SETSTRUCK_PASS29
bool IsDrainHealStrike() const;
bool IsShotStrike() const;
void SetBlocked(bool value){Blocked=value;}
void SetBlockCounter(bool value){BlockCounter=value;}
void SetGuaranteeHitResponse(bool value){GuaranteeHit=value;}
#endif
void SetHitLocation(NHitLocations::CHitLocation& location);
void SetPHitMaterialDef(CDefPointer<const CMaterialDef> material);
void SetHitNegated(bool value){HitNegated=value;}
void SetToCauseRecoil(bool value){CauseRecoil=value;}
bool IsToCauseRecoil()const{return CauseRecoil;}
void SetAsModifiedByArmour(const CThing&,CDefPointer<const CArmourDef>&);
float GetDamageToThing(const CThingGameObject&)const;
bool IsLightningStrike()const;bool IsDivineWrathStrike()const;bool IsUnholyPowerStrike()const;
#endif
#ifdef FABLE_RETAIL_CHITPARAMETERS_VECTOR_MEMBERS
    const C3DVector& GetHitPosition() const { return Position; }
    const C3DVector& GetHitDirection() const { return Direction; }
    static void CheckRetailVectorLayout() {
        FABLE_STATIC_ASSERT_AT(102, offsetof(CHitParameters,Position)==0x02);
        FABLE_STATIC_ASSERT_AT(103, offsetof(CHitParameters,Direction)==0x0e);
        FABLE_STATIC_ASSERT_AT(104, offsetof(CHitParameters,KnockDown)==0x1a);
        FABLE_STATIC_ASSERT_AT(105, offsetof(CHitParameters,ShotBuildupTime)==0x2c);
        FABLE_STATIC_ASSERT_AT(106, offsetof(CHitParameters,DamageMap)==0x30);
        FABLE_STATIC_ASSERT_AT(107, offsetof(CHitParameters,Augmentations)==0x3c);
        FABLE_STATIC_ASSERT_AT(108, offsetof(CHitParameters,PHitLocation)==0x48);
        FABLE_STATIC_ASSERT_AT(109, offsetof(CHitParameters,PThingHitting)==0x50);
        FABLE_STATIC_ASSERT_AT(110, offsetof(CHitParameters,PWeapon)==0x58);
        FABLE_STATIC_ASSERT_AT(111, sizeof(CHitParameters)==0x60);
    }
#else
    const C3DVector& GetHitPosition() const { return *reinterpret_cast<const C3DVector*>(Position); }
    const C3DVector& GetHitDirection() const { return *reinterpret_cast<const C3DVector*>(Direction); }
#endif
    CThing* GetPThingHitting() const { return PThingHitting.Get(); }
    CThing* GetPWeapon() const { return PWeapon.Get(); }
    NHitLocations::CHitLocation* GetPHitLocation() const { return PHitLocation; }
    bool IsLocalised() const { return Localised; }
    bool IsDirectional() const { return Directional; }
    bool IsKnockDown() const { return KnockDown; }
    bool IsDecapitate() const { return Decapitate; }
    bool IsFlourish() const { return Flourish; }
    bool IsEpicSpell() const { return EpicSpell; }
    bool IsBlockCounter() const { return BlockCounter; }
    bool IsBlocked() const { return Blocked; }
    bool IsHitNegated() const { return HitNegated; }
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
// Evidence: both original PDB types and donor026fd4f0; native007c1000 accesses.
public:
void SetHitNegated(bool value){HitNegated=value;}
void SetCreateParticleEffectOnHit(bool value){CreateParticleEffectOnHit=value;}
#endif
#if defined(FABLE_RETAIL_CHITPARAMETERS_SETASMODIFIEDBYARMOUR) && !defined(FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT)
    // NF67 opt-in (round 7, row 0x00869020 itself): Ego_r.pdb / FableWin.pdb
    // ?SetAsModifiedByArmour@CHitParameters@@QAEXABVCThing@@AAV?$CDefPointer@$$CBVCArmourDef@@@@@Z (public, the same declaration
    // FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT carries, without that opt-in's other members). Declaration only: no layout change.
    void SetAsModifiedByArmour(const CThing&,CDefPointer<const CArmourDef>&);   // 0x00869020
#endif
    bool IsMoveBackOnHit() const { return MoveBack; }
    bool IsCreateDustParticleEffectOnHit() const { return CreateDustParticleEffectOnHit; }
#ifdef FABLE_RETAIL_CHITPARAMETERS_EFFECTONHIT
    // Retail opt-in (chunk 778, row 0x00839420 NThingHelpers::CreateEffectOnHit): FableWin.pdb
    // ?IsCreateParticleEffectOnHit@CHitParameters@@QBE_NXZ and ?GetPHitMaterialDef@CHitParameters@@QBE?AV?$CDefPointer@$$CBVCMaterialDef@@@@XZ,
    // inline in retail (`mov al,[esi+23h]`; the by-value CDefPointer copy `mov ebx,[esi+4Ch]; inc [ebx+4]`). Bodies only: no layout change.
    bool IsCreateParticleEffectOnHit() const { return CreateParticleEffectOnHit; }
    CDefPointer<const CMaterialDef> GetPHitMaterialDef() const { return PHitMaterialDef; }
#endif
#ifdef FABLE_RETAIL_CHITPARAMETERS_BLOCKABLE
    // Retail opt-in (chunk 794, row 0x007039A0 CThingObject::OnHit): FableWin.pdb ?IsBlockable@CHitParameters@@QBE_NXZ, inline in retail
    // (the +0x1C byte read in place, passed to CTCShotDefinitionBase::OnHitByMelee). Body only: no layout change.
    bool IsBlockable() const { return Blockable; }
#endif
#ifdef FABLE_RETAIL_CHITPARAMETERS_SETBLOCKABLE
    // Retail opt-in (Codex pass 24): FableWin donor 0x0271EDE0 and
    // the expanded store in CTCHitOnCollision::OnCollision.
    void SetBlockable(bool blockable) { Blockable = blockable; }
#endif
#ifdef FABLE_RETAIL_CHITPARAMETERS_LOCATIONCTOR
    // Opt-in (chunk 2053, row 0x0087F3A0): FableWin.pdb ??0CHitParameters@@QAE@PAVCHitLocation@NHitLocations@@PAVCThing@@1@Z (out of line,
    // retail 0x0087FF60, landed; FableWin lists it right after CCombatAction_KickThingOnGround::PerformAction, its first user) and Ego_r.pdb
    // ?AddDamageWithAttribute@CHitParameters@@QAEXW4EDamageAttribute@@M@Z (retail 0x00868CA0). Declarations only: no layout change.
    CHitParameters(NHitLocations::CHitLocation* location, CThing* thing_hitting, CThing* weapon);   // 0x0087FF60
#ifndef FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR
    void AddDamageWithAttribute(EDamageAttribute attribute, float damage);   // 0x00868CA0
#endif
#endif
#ifdef FABLE_RETAIL_CHITPARAMETERS_CTOR
    // Retail opt-in (chunk 794, row 0x007C4430 CTCHealLife::StartHealingEffect): Ego_r.pdb ??0CHitParameters@@QAE@XZ /
    // ??1CHitParameters@@QAE@XZ / ?SetThingHitting@CHitParameters@@QAEXAAVCThing@@@Z, out of line in retail (0x007A3BC0 /
    // 0x00789358 / 0x008685F0; landed callers 0x007CCE20 and chunk 759 bind the same addresses). Declarations only: no layout change.
    CHitParameters();                                                    // 0x007A3BC0
    ~CHitParameters();                                                   // 0x00789358
    void SetThingHitting(CThing& thing);                                 // 0x008685F0
#endif
#ifdef FABLE_RETAIL_CHITPARAMETERS_COPY_CTOR
    // NF78 opt-in (round 7, row 0x00812170): Ego_r.pdb ??0CHitParameters@@QAE@ABV0@@Z (0x00807C70; retail 0x00812170, called by
    // NEntityEvents::CEventHitBase's constructor 0x00811760): the memberwise copy, out of line. Declaration only: no layout change.
    // Needs FABLE_RETAIL_CHITPARAMETERS_VECTOR_MEMBERS + _DAMAGEMAP + _AUGMENTATIONS (the real member types).
public:
    CHitParameters(const CHitParameters& other);                         // 0x00812170
#endif
#ifdef FABLE_RETAIL_CHITPARAMETERS_CLEARALLDAMAGE
    // NF81 opt-in (round 7, q10986 binder, row 0x00868A40): Ego_r.pdb 0x0083EC50 / FableWin.pdb 0x02810E60 ?ClearAllDamage@CHitParameters@@QAEXXZ
    // (public; retail 0x00868A40, out of line: `DamageMap.clear()`). Needs FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP. Declaration only: no layout change.
public:
    void ClearAllDamage();                                               // 0x00868A40
#endif
#ifdef FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR
    // Retail opt-in (wf17, row 0x00854DD0 CCreatureAction_SpiritAttack::PerformAction): Ego_r.pdb
    // ??0CHitParameters@@QAE@PAVCHitLocation@NHitLocations@@PAVCThing@@1ABVC3DVector@@2_N333@Z (0x00789238, ret 24h) and
    // ?AddDamageWithAttribute@CHitParameters@@QAEXW4EDamageAttribute@@M@Z (0x00868CA0), both out of line in retail (landed rows).
    // Declarations only: no layout change.
#ifdef FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR_BODY
    // Retail opt-in (row 0x00854DD0 CCreatureAction_SpiritAttack::PerformAction): the REAL body of the nine-argument constructor
    // 0x00789238, spelled as the landed row CHitParameters_CHitParameters_00789238.cpp spells it (every member an initialiser in
    // declaration order, Position(position) / Direction(direction) included; DamageMap, Augmentations and PHitMaterialDef are
    // default-constructed), defined in the class and kept out of line. Retail calls it out of line, but a caller's TU that sees
    // the body lets VC7.1 use the callee's mod/ref: the two `const C3DVector&` arguments are only read, so the caller's vectors
    // do not count as escaped (retail 0x00854F14..0x00854FEF builds both X first and keeps the unscaled X/Y on the x87 stack).
    // Implies FABLE_RETAIL_CHITPARAMETERS_VECTOR_MEMBERS + _DAMAGEMAP + _AUGMENTATIONS (the real member types).
    __declspec(noinline) CHitParameters(NHitLocations::CHitLocation* hit_location, CThing* thing_hitting, CThing* weapon, const C3DVector& position,
                   const C3DVector& direction, bool knock_down, bool blockable, bool flourish, bool block_counter)
        : Localised(true)
        , Directional(true)
        , Position(position)
        , Direction(direction)
        , KnockDown(knock_down)
        , Decapitate(false)
        , Blockable(blockable)
        , Flourish(flourish)
        , EpicSpell(false)
        , BlockCounter(block_counter)
        , PlayHitResponse(true)
        , PlayHitResponseOverrideSet(false)
        , MoveBack(true)
        , CreateParticleEffectOnHit(true)
        , CreateDustParticleEffectOnHit(true)
        , GuaranteeHit(false)
        , Blocked(false)
        , HitNegated(false)
        , CauseRecoil(false)
        , ShotBuildupTime(0.0f)
        , PHitLocation(hit_location)
        , PThingHitting(thing_hitting)
        , PWeapon(weapon)
    {
    }
#else
    CHitParameters(NHitLocations::CHitLocation* hit_location, CThing* thing_hitting, CThing* weapon, const C3DVector& position,
                   const C3DVector& direction, bool knock_down, bool blockable, bool flourish, bool block_counter); // 0x00789238
#endif
    void AddDamageWithAttribute(EDamageAttribute attribute, float damage);   // 0x00868CA0
#endif
#ifdef FABLE_RETAIL_CHITPARAMETERS_EPICSPELL_SETTERS
    // agent wfE10 (rows 0x00795300 / 0x0078FD60, with HIT_CTOR + HIT_CTOR_BODY instead of SPECIALABILITY): the two inline setters
    // SPECIALABILITY declares, FableWin.pdb ?SetEpicSpell@CHitParameters@@QAEX_N@Z (+0x1E) / ?SetCreateParticleEffectOnHit@CHitParameters@@QAEX_N@Z
    // (+0x23). No layout change.
    void SetEpicSpell(bool value) { EpicSpell=value; }
    void SetCreateParticleEffectOnHit(bool value) { CreateParticleEffectOnHit=value; }
#endif
#ifdef FABLE_RETAIL_CHITPARAMETERS_SETWEAPON
    // Retail opt-in (row 0x00868CE0): Ego_r.pdb ?SetWeapon@CHitParameters@@QAEXAAVCThing@@@Z, out of line in retail (callers
    // 0x00838BED / 0x00838FB2). Declaration only: no layout change.
    void SetWeapon(CThing& weapon);                                      // 0x00868CE0
#endif
#if defined(FABLE_RETAIL_CHITPARAMETERS_ADDDAMAGE) && !defined(FABLE_RETAIL_CHITPARAMETERS_HIT_CTOR) && !defined(FABLE_RETAIL_CHITPARAMETERS_LOCATIONCTOR)
    // (HIT_CTOR / LOCATIONCTOR declare the same member.)
    // Retail opt-in (w22 agent2042, row 0x007F2DC0 CTCLightningNode::FrameUpdate): Ego_r.pdb / FableWin.pdb
    // ?AddDamageWithAttribute@CHitParameters@@QAEXW4EDamageAttribute@@M@Z, out of line in retail at 0x00868CA0 (the bound spelling;
    // FrameUpdate pushes the def's DamagePerCycle then 2 = DAMAGE_LIGHTNING, 0x007F3455..0x007F3465). Declaration only: no layout change.
    void AddDamageWithAttribute(EDamageAttribute attribute, float damage);   // 0x00868CA0
#endif
#ifdef FABLE_RETAIL_CHITPARAMETERS_SPECIALABILITY
    // Retail opt-in (row 0x00795300 CTCUnholyPower::FrameUpdate): FableWin.pdb
    // ??0CHitParameters@@QAE@PAVCHitLocation@NHitLocations@@PAVCThing@@1ABVC3DVector@@2_N333@Z (out of line 0x00789238),
    // ??1CHitParameters@@QAE@XZ (0x00789358), ?AddDamageWithAttribute@CHitParameters@@QAEXW4EDamageAttribute@@M@Z (0x00868CA0) and the
    // inline setters ?SetEpicSpell@CHitParameters@@QAEX_N@Z (+0x1E) / ?SetCreateParticleEffectOnHit@CHitParameters@@QAEX_N@Z (+0x23).
    CHitParameters(NHitLocations::CHitLocation* location,CThing* hitter,CThing* weapon,const C3DVector& position,const C3DVector& direction,
                   bool knock_down,bool blockable,bool flourish,bool block_counter);   // 0x00789238
    ~CHitParameters();                                                   // 0x00789358
    void AddDamageWithAttribute(EDamageAttribute attribute,float damage);   // 0x00868CA0
    void SetEpicSpell(bool value) { EpicSpell=value; }
#ifndef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
    void SetCreateParticleEffectOnHit(bool value) { CreateParticleEffectOnHit=value; }
#endif
#endif
    void SetKnockDown(bool value) { KnockDown=value; }
    void SetDecapitate(bool value) { Decapitate=value; }
    void SetPlayHitResponse(bool value) { PlayHitResponseOverrideSet=true;PlayHitResponse=value; }
    void SetMoveBackOnHit(bool value) { MoveBack=value; }
#ifdef FABLE_RETAIL_CHITPARAMETERS_SETCREATEDUST
    // agent wfE10 (row 0x007A1B20): FableWin.pdb ?SetCreateDustParticleEffectOnHit@CHitParameters@@QAEX_N@Z (inline: retail byte store
    // 0x007A21AC into the temporary's CreateDustParticleEffectOnHit). No layout change.
    void SetCreateDustParticleEffectOnHit(bool value) { CreateDustParticleEffectOnHit=value; }
#endif
    bool IsMeleeStrike() const;
    bool IsExplosionStrike() const;
    bool IsDrainStrike() const;
    bool IsPlayGetHitResponse(const CThing*) const;
};
#ifdef FABLE_RETAIL_CHITPARAMETERS_VECTOR_MEMBERS
#pragma pack(pop)
#endif
FABLE_STATIC_ASSERT_AT(236, sizeof(CHitParameters)==0x60);

#ifdef FABLE_RETAIL_FLASH_HIT_CTOR_OWNER_PASS30
#if defined(_MSC_VER)
// Fidelity: scaffolding, not retail. The 00789238 owner uses /Os and a frame pointer.
// The reset below ends with optimize("", on). Set row-specific pragmas after this header.
#pragma optimize("y",off)
#pragma optimize("s",on)
#endif
inline CHitParameters::CHitParameters(NHitLocations::CHitLocation* hit_location, CThing* thing_hitting, CThing* weapon, const C3DVector& position,
                   const C3DVector& direction, bool knock_down, bool blockable, bool flourish, bool block_counter)
        : Localised(true)
        , Directional(true)
        , Position(position)
        , Direction(direction)
        , KnockDown(knock_down)
        , Decapitate(false)
        , Blockable(blockable)
        , Flourish(flourish)
        , EpicSpell(false)
        , BlockCounter(block_counter)
        , PlayHitResponse(true)
        , PlayHitResponseOverrideSet(false)
        , MoveBack(true)
        , CreateParticleEffectOnHit(true)
        , CreateDustParticleEffectOnHit(true)
        , GuaranteeHit(false)
        , Blocked(false)
        , HitNegated(false)
        , CauseRecoil(false)
        , ShotBuildupTime(0.0f)
        , PHitLocation(hit_location)
        , PThingHitting(thing_hitting)
        , PWeapon(weapon)
    {
    }
#if defined(_MSC_VER)
#pragma optimize("",on)
#endif
#endif
#endif
