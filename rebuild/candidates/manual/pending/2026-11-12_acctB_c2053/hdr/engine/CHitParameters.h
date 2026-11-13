#ifndef FABLE_ENGINE_CHITPARAMETERS_H
#define FABLE_ENGINE_CHITPARAMETERS_H
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
namespace NHitLocations { class CHitLocation; }
class CMaterialDef;class CThing;
// Original Ego_r TPI0x2af3a; native OnHit proves offsets0,1,2,e,1a..28,48,50,58.
// STLport replaces both 16-byte donor containers with12-byte containers: native sizeof60.
#if defined(FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP) || defined(FABLE_RETAIL_CHITPARAMETERS_LOCATIONCTOR)
#include "engine/EDamageAttribute.h"
#endif
#ifdef FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP
#include "stlport/fable_map.h"
#endif
class CHitParameters {
#ifdef FABLE_ONHIT_TEST_ACCESS
    friend class COnHitFixture;
#endif

private:
    bool Localised,Directional;
    unsigned char Position[12]; // Original C3DVector, deliberately unaligned at+2.
    unsigned char Direction[12]; // Original C3DVector, deliberately unaligned at+e.
    bool KnockDown,Decapitate,Blockable,Flourish,EpicSpell,BlockCounter;
    bool PlayHitResponse,PlayHitResponseOverrideSet,MoveBack;
    bool CreateParticleEffectOnHit,CreateDustParticleEffectOnHit,GuaranteeHit;
    bool Blocked,HitNegated,CauseRecoil;
    float ShotBuildupTime;
#ifdef FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP
    std::map<EDamageAttribute,float> DamageMap;
#else
    unsigned char DamageMap[12]; // Original std::map<EDamageAttribute,float>.
#endif
    unsigned char Augmentations[12]; // Original std::vector<EObjectAugmentationType>.
    NHitLocations::CHitLocation* PHitLocation;
    CDefPointer<const CMaterialDef> PHitMaterialDef;
    CIntelligentPointer<CThing> PThingHitting,PWeapon;
public:
#ifdef FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP
    const std::map<EDamageAttribute,float>& GetDamageMap()const{return DamageMap;}
#endif
    const C3DVector& GetHitPosition() const { return *reinterpret_cast<const C3DVector*>(Position); }
    const C3DVector& GetHitDirection() const { return *reinterpret_cast<const C3DVector*>(Direction); }
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
#ifdef FABLE_RETAIL_CHITPARAMETERS_LOCATIONCTOR
    // Opt-in (chunk 2053, row 0x0087F3A0): FableWin.pdb ??0CHitParameters@@QAE@PAVCHitLocation@NHitLocations@@PAVCThing@@1@Z (out of line,
    // retail 0x0087FF60, landed; FableWin lists it right after CCombatAction_KickThingOnGround::PerformAction, its first user) and Ego_r.pdb
    // ?AddDamageWithAttribute@CHitParameters@@QAEXW4EDamageAttribute@@M@Z (retail 0x00868CA0). Declarations only: no layout change.
    CHitParameters(NHitLocations::CHitLocation* location, CThing* thing_hitting, CThing* weapon);   // 0x0087FF60
    void AddDamageWithAttribute(EDamageAttribute attribute, float damage);   // 0x00868CA0
#endif
#ifdef FABLE_RETAIL_CHITPARAMETERS_CTOR
    // Retail opt-in (chunk 794, row 0x007C4430 CTCHealLife::StartHealingEffect): Ego_r.pdb ??0CHitParameters@@QAE@XZ /
    // ??1CHitParameters@@QAE@XZ / ?SetThingHitting@CHitParameters@@QAEXAAVCThing@@@Z, out of line in retail (0x007A3BC0 /
    // 0x00789358 / 0x008685F0; landed callers 0x007CCE20 and chunk 759 bind the same addresses). Declarations only: no layout change.
    CHitParameters();                                                    // 0x007A3BC0
    ~CHitParameters();                                                   // 0x00789358
    void SetThingHitting(CThing& thing);                                 // 0x008685F0
#endif
    void SetKnockDown(bool value) { KnockDown=value; }
    void SetDecapitate(bool value) { Decapitate=value; }
    void SetPlayHitResponse(bool value) { PlayHitResponseOverrideSet=true;PlayHitResponse=value; }
    void SetMoveBackOnHit(bool value) { MoveBack=value; }
    bool IsMeleeStrike() const;
    bool IsExplosionStrike() const;
    bool IsDrainStrike() const;
    bool IsPlayGetHitResponse(const CThing*) const;
};
FABLE_STATIC_ASSERT(sizeof(CHitParameters)==0x60);

#endif
