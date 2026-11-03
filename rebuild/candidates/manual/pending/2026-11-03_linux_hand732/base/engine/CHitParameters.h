#ifndef FABLE_ENGINE_CHITPARAMETERS_H
#define FABLE_ENGINE_CHITPARAMETERS_H
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
namespace NHitLocations { class CHitLocation; }
class CMaterialDef;class CThing;
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
