#ifndef FABLE_ENGINE_CTCKRAKEN_H
#define FABLE_ENGINE_CTCKRAKEN_H
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CKrakenDef.h"
class CTCBoss;
// Layout: complete original Ego316b1/FW34363; native104 after two list reductions.
class CTCKraken:public CTCBase {
public:
#ifdef _MSC_VER
 enum EKrakenBossPhase;enum EKrakenMode;
#else
 enum EKrakenBossPhase{};enum EKrakenMode{};
#endif
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return TCI_KRAKEN;}
 virtual ETCInterfaceType GetInterfaceType() const;
 static CTCBase * FABLE_FASTCALL Alloc(CThing & p0);
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
 virtual CCharString GetClassTypeName() const;
 CTCKraken(const CTCKraken & p0);
 CTCKraken(CThing & p0);
 virtual void OnInitialActivate();
 virtual void OnDie();
 virtual void FrameUpdate();
 virtual bool HasFrameUpdate() const;
 void IncrementTentacleStrikeCounter();
 long GetTentacleStrikeCounter() const;
 CThing * GetPTarget() const;
 CThing * GetPCentreMarker() const;
 bool HaveAllTentaclesBeenHit() const;
 bool IsTriggered() const;
 long SetLastStrikeTick() const;
 bool HasTargetBeenHitByKraken();
 void RaiseSunkenTentacles();
 void StartHeadAttackPhase();
 void SetHeadTracking(bool p0);
 bool HeadReadyToStrike();
 void SetReturnHeadToWater(bool p0);
 bool ShouldReturnHeadToWater() const;
 void StartLowAttackBeam();
 void StartHighAttackBeam();
 void EndAttackBeam();
 void StartHeadEffects();
 void DoDieEffects();
 void EndHeadEffects();
 bool DoesHighAttackAfterHit() const;
 void SetHeadHasBeenHit(bool p0);
 bool GetHeadHasBeenHit() const;
 const CKrakenDef & GetKrakenDef() const {return *PKrakenDef;}
 void AddStrikeExplosion(CThing * p0);
private:
 void UpdateUnderwater();
 void UpdateTentacles();
 void UpdateTentaclesOnly();
 void UpdateTentaclesAndHead();
 void DoHeadTracking();
 void UpdateBeamExplosion();
 void StartBeamEffect();
 void KillBeamEffect();
 void CreateTentacles();
 void KillTentacles();
 void CreateHeadEffect(const CCharString & p0);
 void SetPhase(CTCKraken::EKrakenBossPhase p0);
 void SetMode(CTCKraken::EKrakenMode p0);
 void InitTentaclesOnly();
 void InitTentaclesAndHead();
 bool GetClosestTentacleToTarget(std::list<CIntelligentPointer<CThing> >::iterator & p0);
 void GetCentreMarker();
 CDefPointer<CKrakenDef const > PKrakenDef;
 CIntelligentPointer<CThing> PChamber;
 CIntelligentPointer<CThing> PCentreMarker;
 CIntelligentPointer<CThing> PTarget;
 CIntelligentPointer<CThing> PHeadEffect;
 std::list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing> > > ExplosionList;
 CIntelligentPointer<CThing> PBeamWeaponEffect;
 long DelayBeforeBeam;
 CTCBoss * PTCBoss;
 std::list<CIntelligentPointer<CThing>,std::allocator<CIntelligentPointer<CThing> > > Tentacles;
 std::list<CIntelligentPointer<CThing> >::iterator AttackingTentacleIterator;
 long TentacleStrikeCounter;
 bool TentacleReadyToStrike;
 bool HeadTrackTarget;
 bool HeadHasBeenHit;
 long LastStrikeTick;
 long KillBeamCounter;
 bool ReturnHeadToWater;
 CTCKraken::EKrakenBossPhase Phase;
 CTCKraken::EKrakenMode Mode;
public:
 virtual ~CTCKraken();
 CTCKraken & operator=(const CTCKraken & p0);
};
FABLE_STATIC_ASSERT(sizeof(CTCKraken)==104);
#endif
