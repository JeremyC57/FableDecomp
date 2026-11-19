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
#ifdef FABLE_RETAIL_CTCKRAKEN_SETHEADTRACKING_INLINE
 // NF98 opt-in (round 8, q11262, row 0x00924930 CCreatureAction_KrakenHighAttackLoop::FinishAction): FableWin.pdb 0x02C5B750
 // ?SetHeadTracking@CTCKraken@@QAEX_N@Z is debug only (no Ego_r.pdb copy): inline in release, 0x009249FC stores the bool into
 // HeadTrackTarget +0x51 in place (`mov byte ptr [ebx+51h],0`). Same declaration with its body; no layout change.
 void SetHeadTracking(bool p0) { HeadTrackTarget = p0; }
#else
 void SetHeadTracking(bool p0);
#endif
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
FABLE_STATIC_ASSERT_AT(95, sizeof(CTCKraken)==104);
#ifdef FABLE_RETAIL_CTCKRAKEN_EMPTY_UPDATES
// Per-TU opt-in (agentC32/C34, row 0x007FC9D0 CTCKraken::FrameUpdate): the three private per-phase updates are EMPTY functions
// (Ego_r.pdb 16-byte bodies ?UpdateUnderwater 0x007F6220 / ?UpdateTentacles 0x007F6230 / ?UpdateTentaclesAndHead 0x007F64F0;
// FableWin 0x02777E30 / 0x02777E40 / 0x02778220) with no out-of-line copy in retail: FrameUpdate's Phase switch keeps a jump-table
// entry for case 0 (0x007FCC6C) although its call expands to nothing. Inline empty bodies; no layout change.
inline void CTCKraken::UpdateUnderwater(){}
inline void CTCKraken::UpdateTentacles(){}
inline void CTCKraken::UpdateTentaclesAndHead(){}
#endif
#endif
