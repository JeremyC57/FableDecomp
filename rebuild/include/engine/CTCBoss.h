#ifndef FABLE_ENGINE_CTCBOSS_H
#define FABLE_ENGINE_CTCBOSS_H
#include "engine/CTCBase.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CBossDef.h"
class CThingAICreature;
// Full original Ego_r0x2fa7d data; original private composites retained.
class CTCBoss:public CTCBase {
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType(){return (ETCInterfaceType)173;}
 virtual ETCInterfaceType GetInterfaceType()const;
 virtual CCharString GetClassTypeName()const;
 virtual bool HasFrameUpdate()const;
 virtual void FrameUpdate();
 long GetBossPhase()const;
 void SetNextBossPhase(long,bool);
#ifdef FABLE_RETAIL_JACK_STAGE1_UPDATE
 bool IsReadyForPhaseChange()const;
#endif

 bool HasBossDef()const{return !PBossDef.IsNull();}
 const CBossDef& GetBossDef()const{return *PBossDef;}
#ifdef FABLE_RETAIL_CTCBOSS_EFFECTS
 // Retail opt-in (chunk Q2, row 0x008E0A10 NCreatureMode::CModeScorpionKingVulnerable::FrameUpdate): Ego_r.pdb / FableWin.pdb
 // ?IncrementBossPhase@CTCBoss@@QAEXXZ (0x007AED20), ?IsReadyForPhaseChange@CTCBoss@@QBE_NXZ (0x007AEE90), ?CountEffects@CTCBoss@@QAEJXZ
 // (0x007AEF10), ?ClearEffects@CTCBoss@@QAEXXZ (0x007AEF60), ?AddEffect@CTCBoss@@QAEXAAVCThing@@@Z (0x007AF6C0). Declarations only.
 void IncrementBossPhase();
 bool IsReadyForPhaseChange() const;
 long CountEffects();
 void ClearEffects();
 void AddEffect(CThing&);
#endif
#ifdef FABLE_RETAIL_CTCBOSS_SETHELPER
 // Retail opt-in (chunk Q17, row 0x0095B820): Ego_r.pdb ?SetHelperCreature@CTCBoss@@QAEXAAVCThingAICreature@@@Z (out of line at
 // 0x007AF310). Declaration only.
 void SetHelperCreature(CThingAICreature& helper);                       // 0x007AF310
#endif
#ifdef FABLE_RETAIL_CTCBOSS_KILLHELPERS
 // Retail opt-in (wf12, row 0x007AF490): Ego_r.pdb ?KillHelpers@CTCBoss@@QAEXXZ (retail 0x007AF490). Declaration only.
 void KillHelpers();                                                     // 0x007AF490
#endif
#ifdef FABLE_RETAIL_CTCBOSS_LIVINGHELPERS
 // Retail opt-in (chunk Q3, rows 0x008E20F0 / 0x008E24F0 CModeGuildmasterPowerUpShield / Speed::FrameUpdate): Ego_r.pdb
 // ?CountLivingHelpers@CTCBoss@@QAEJXZ (retail 0x007AEEC0; the landed Heal FrameUpdate 0x008DFFD0 calls it too). Declaration only.
 long CountLivingHelpers();
#endif
private:
 CDefPointer<const CBossDef> PBossDef;
 long GenericCounter,StrikesSinceHit,currentBossPhase,NextWeapon;
 bool BCountStrikes,EnablePhaseChanges;
 C3DVector PhaseStartPos; // Original C3DVector+22, shared opt-in alignment1.
 std::vector<CIntelligentPointer<CThingAICreature> > HelperCreatures;
 std::vector<CIntelligentPointer<CThing> > Effects;
};
FABLE_STATIC_ASSERT(sizeof(CTCBoss)==72);
#endif
