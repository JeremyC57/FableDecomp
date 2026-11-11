#ifndef FABLE_ENGINE_CTCDIVINEWRATH_H
#define FABLE_ENGINE_CTCDIVINEWRATH_H
// Complete original owner: Ego0x2ed75/FableWin0x3189e; native size200.
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CWillButtonHelperTimer.h"
#include "engine/C3DVector.h"
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
#include "stlport/fable_map.h"
#include "stlport/fable_containers.h"
class CSpecialAbilitiesDivineWrathDef;class CThingCreatureBase;
class CTCDivineWrath:public CTCBase{
private:
 struct PerCreatureInfo{
public:
 CIntelligentPointer<CThingCreatureBase> ActualCreature;
 long FramesToGetHit;
 CIntelligentPointer<CThing> Effect;
 CEnginePrimitiveThemeChannelDesc DefaultChannel;
 PerCreatureInfo(const PerCreatureInfo & p0);
 PerCreatureInfo();
 ~PerCreatureInfo();
 PerCreatureInfo & operator=(const PerCreatureInfo & p0);
 };
 struct PerEffectInfo{
public:
 C3DVector Position;
 CIntelligentPointer<CThing> Effect;
 PerEffectInfo(const PerEffectInfo & p0);
 PerEffectInfo();
 ~PerEffectInfo();
 PerEffectInfo & operator=(const PerEffectInfo & p0);
 };
 typedef std::map<CThingCreatureBase*,PerCreatureInfo> MapOfCreaturesToPerCreatureInfo;
 typedef std::vector<PerEffectInfo> VectorOfCreaturesToPerEffectInfo;
public:
 static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType();
 virtual ETCInterfaceType GetInterfaceType() const;
 static CTCBase * FABLE_FASTCALL Alloc(CThing & p0);
 static CCharString FABLE_FASTCALL StatGetClassTypeName();
 virtual CCharString GetClassTypeName() const;
 CTCDivineWrath(const CTCDivineWrath & p0);
 CTCDivineWrath(CThing & p0);
 virtual ~CTCDivineWrath();
 virtual bool HasFrameUpdate() const;
 virtual void OnCreate();
 virtual void FrameUpdate();
 virtual void OnKill();
 virtual void OnRemove();
 void FinishMove();
 void CancelMove();
 void CancelForCutscene();
 void SetDivineWrathActionInitialised(bool p0);
 bool HasDivineWrathActionInitialised() const;
 void SetPItemRemoved(CIntelligentPointer<CThing> p0);
 float GetDamageMultiplier() const;
 bool IsCharging() const;
private:
 void FinalCleanup();
 bool Attackable(CThingCreatureBase * p0);
 void ResetCreatureHitState(CIntelligentPointer<CThingCreatureBase> p0);
 void UnsheatheWeapon();
 unsigned long PlaySoundW(const CCharString & p0, bool p1);
 void StopSound(unsigned long p0);
 void SetPTarget();
 CDefPointer<CSpecialAbilitiesDivineWrathDef const > PDef;
 CIntelligentPointer<CThing> PItemRemoved;
 bool DivineWrathActionInitialised;
 CWillButtonHelperTimer Timer;
 long SpellLevel;
 long NumFramesBetweenDamageHit;
 long DurationOfSpellFrames;
 long NumFramesForEnvChange;
 long CurFramesForEnvChange;
 bool Finished;
 long NumFramesToWaitForFadeOut;
 bool CutSceneCancel;
 CIntelligentPointer<CThingCreatureBase> PTarget;
 bool IsZTargetting;
 bool PerformedReleaseAction;
 std::map<CThingCreatureBase *,CTCDivineWrath::PerCreatureInfo,std::less<CThingCreatureBase *>,std::allocator<std::pair<CThingCreatureBase * const,CTCDivineWrath::PerCreatureInfo> > > LastFrameCreaturesHit;
 std::vector<CTCDivineWrath::PerEffectInfo,std::allocator<CTCDivineWrath::PerEffectInfo> > HitEffects;
 float PerFrameStaminaModify;
 float AccModifyStamina;
 long StaminaModify;
 CIntelligentPointer<CThing> BuildUpEffect;
 CEnginePrimitiveThemeChannelDesc HeroDefaultChannel;
 CEnginePrimitiveThemeChannelDesc SpellExclusionChannel;
 CIntelligentPointer<CThing> HandEffect1;
 CIntelligentPointer<CThing> HandEffect2;
 unsigned long LoopingSound;
 CIntelligentPointer<CThing> PObstruction;
public:
 CTCDivineWrath & operator=(const CTCDivineWrath & p0);
};
FABLE_STATIC_ASSERT(sizeof(CTCDivineWrath)==200);
#endif
