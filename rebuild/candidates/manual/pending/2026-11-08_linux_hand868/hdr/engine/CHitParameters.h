#ifndef FABLE_ENGINE_CHITPARAMETERS_H
#define FABLE_ENGINE_CHITPARAMETERS_H
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
namespace NHitLocations { class CHitLocation; }
class CMaterialDef;class CThing;
#ifdef FABLE_RETAIL_CHITPARAMETERS_LIGHTNING
enum EDamageAttribute;
#endif
// Original Ego_r TPI0x2af3a; native OnHit proves offsets0,1,2,e,1a..28,48,50,58.
// STLport replaces both 16-byte donor containers with12-byte containers: native sizeof60.
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
    unsigned char DamageMap[12]; // Original std::map<EDamageAttribute,float>.
    unsigned char Augmentations[12]; // Original std::vector<EObjectAugmentationType>.
    NHitLocations::CHitLocation* PHitLocation;
    CDefPointer<const CMaterialDef> PHitMaterialDef;
    CIntelligentPointer<CThing> PThingHitting,PWeapon;
public:
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
#ifdef FABLE_RETAIL_CHITPARAMETERS_CTOR
    // Retail opt-in (chunk 794, row 0x007C4430 CTCHealLife::StartHealingEffect): Ego_r.pdb ??0CHitParameters@@QAE@XZ /
    // ??1CHitParameters@@QAE@XZ / ?SetThingHitting@CHitParameters@@QAEXAAVCThing@@@Z, out of line in retail (0x007A3BC0 /
    // 0x00789358 / 0x008685F0; landed callers 0x007CCE20 and chunk 759 bind the same addresses). Declarations only: no layout change.
    CHitParameters();                                                    // 0x007A3BC0
    ~CHitParameters();                                                   // 0x00789358
    void SetThingHitting(CThing& thing);                                 // 0x008685F0
#endif
#ifdef FABLE_RETAIL_CHITPARAMETERS_LIGHTNING
    // Retail opt-in (chunk 868, row 0x007A1B20 CTCWillLightning::CLightningBeam::FrameUpdate; the landed sibling models of
    // 0x007E2600 CTCElectrocuteOnTouch::OnHit): the nine-argument constructor 0x00789238 (FableWin.pdb
    // ??0CHitParameters@@QAE@PAVCHitLocation@NHitLocations@@PAVCThing@@1ABVC3DVector@@2_N333@Z, out of line in retail),
    // ?AddDamageWithAttribute@CHitParameters@@QAEXW4EDamageAttribute@@M@Z (0x00868CA0, labels) and the inline
    // ?SetCreateDustParticleEffectOnHit@CHitParameters@@QAEX_N@Z (`mov byte ptr [esp+..+24h],0`). EDamageAttribute is the Ego_r.pdb
    // enum of the damage map key; this header declares it forward (the row passes its own value).
    CHitParameters(NHitLocations::CHitLocation* location, CThing* hitting, CThing* weapon, const C3DVector& position,
                   const C3DVector& direction, bool knockDown, bool blockable, bool flourish, bool blockCounter);   // 0x00789238
    void AddDamageWithAttribute(EDamageAttribute attribute, float damage);   // 0x00868CA0
    void SetCreateDustParticleEffectOnHit(bool value) { CreateDustParticleEffectOnHit=value; }
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
