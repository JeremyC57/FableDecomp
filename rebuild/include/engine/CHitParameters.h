#ifndef FABLE_ENGINE_CHITPARAMETERS_H
#define FABLE_ENGINE_CHITPARAMETERS_H
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
namespace NHitLocations { class CHitLocation; }
class CMaterialDef;class CThing;
#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT
class CArmourDef;class CThingGameObject;
#endif
// Original Ego_r TPI0x2af3a; native OnHit proves offsets0,1,2,e,1a..28,48,50,58.
// STLport replaces both 16-byte donor containers with12-byte containers: native sizeof60.
#ifdef FABLE_RETAIL_CHITPARAMETERS_DAMAGEMAP
#include "engine/EDamageAttribute.h"
#include "stlport/fable_map.h"
#endif
#ifdef FABLE_RETAIL_CHITPARAMETERS_AUGMENTATIONS
#include "engine/EObjectAugmentationType.h"
#include "stlport/fable_containers.h"
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
void SetHitLocation(NHitLocations::CHitLocation& location);
void SetPHitMaterialDef(CDefPointer<const CMaterialDef> material);
void SetHitNegated(bool value){HitNegated=value;}
void SetToCauseRecoil(bool value){CauseRecoil=value;}
bool IsToCauseRecoil()const{return CauseRecoil;}
void SetAsModifiedByArmour(const CThing&,CDefPointer<const CArmourDef>&);
float GetDamageToThing(const CThingGameObject&)const;
bool IsLightningStrike()const;bool IsDivineWrathStrike()const;bool IsUnholyPowerStrike()const;
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
#ifdef FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
// Evidence: both original PDB types and donor026fd4f0; native007c1000 accesses.
public:
void SetHitNegated(bool value){HitNegated=value;}
void SetCreateParticleEffectOnHit(bool value){CreateParticleEffectOnHit=value;}
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
#ifdef FABLE_RETAIL_CHITPARAMETERS_CTOR
    // Retail opt-in (chunk 794, row 0x007C4430 CTCHealLife::StartHealingEffect): Ego_r.pdb ??0CHitParameters@@QAE@XZ /
    // ??1CHitParameters@@QAE@XZ / ?SetThingHitting@CHitParameters@@QAEXAAVCThing@@@Z, out of line in retail (0x007A3BC0 /
    // 0x00789358 / 0x008685F0; landed callers 0x007CCE20 and chunk 759 bind the same addresses). Declarations only: no layout change.
    CHitParameters();                                                    // 0x007A3BC0
    ~CHitParameters();                                                   // 0x00789358
    void SetThingHitting(CThing& thing);                                 // 0x008685F0
#endif
#ifdef FABLE_RETAIL_CHITPARAMETERS_SETWEAPON
    // Retail opt-in (row 0x00868CE0): Ego_r.pdb ?SetWeapon@CHitParameters@@QAEXAAVCThing@@@Z, out of line in retail (callers
    // 0x00838BED / 0x00838FB2). Declaration only: no layout change.
    void SetWeapon(CThing& weapon);                                      // 0x00868CE0
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
